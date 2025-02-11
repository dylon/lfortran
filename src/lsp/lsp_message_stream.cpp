#include <cctype>
#include <cstddef>
#include <stdexcept>
#include <string_view>

#include <lsp/lsp_exception.h>
#include <lsp/lsp_message_stream.h>

namespace LCompilers::LanguageServerProtocol {

  LspMessageStream::LspMessageStream(std::istream &istream, lsl::Logger &logger)
    : istream(istream)
    , logger(logger)
  {
    message.reserve(16384);
    position = 0;
  }

  auto LspMessageStream::nextUpper() -> char {
    char c;
    istream.get(c);
    if (c == '\0') {
      throw std::runtime_error("Terminated while reading from stdin.");
    }
    c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    message.push_back(c);
    ++position;
    return c;
  }

  auto LspMessageStream::nextChar() -> char {
    char c;
    istream.get(c);
    if (c == '\0') {
      throw std::runtime_error("Terminated while reading from stdin.");
    }
    message.push_back(c);
    ++position;
    return c;
  }

  auto LspMessageStream::logEscaped(char c) -> void {
    switch (c) {
    case '\n': {
      logger << "\\n";
      break;
    }
    case '\t': {
      logger << "\\t";
      break;
    }
    case '\b': {
      logger << "\\b";
      break;
    }
    case '\r': {
      logger << "\\r";
      break;
    }
    case '\f': {
      logger << "\\f";
      break;
    }
    default: {
      logger << c;
    }
    }
  }

  auto LspMessageStream::next() -> std::string {
    std::size_t numBytes;
    bool hasContentLength = false;
    std::size_t start = 0;
    std::size_t length;
    std::string_view view;
    char c;
  parse_header_name:
    start = position;
    do {
      switch (nextUpper()) {
      case '\r': {
        length = (position - start) - 1;
        view = std::string_view(message.data() + start, length);
        c = nextUpper();
        if (c != '\n') {
          std::unique_lock<std::mutex> loggerLock(logger.mutex());
          logger << "Expected \\r to be followed by \\n, not '";
          logEscaped(c);
          logger << "\'" << std::endl;
          goto parse_header_name;
        }
        if ((view.length() == 0) && hasContentLength) {
          goto parse_body;
        }
        {
          std::unique_lock<std::mutex> loggerLock(logger.mutex());
          logger
            << "Reached out-of-sequence newline while parsing header name."
            << std::endl;
          return "";
        }
        goto parse_header_name;
      }
      case '\n': {
        length = (position - start) - 1;
        view = std::string_view(message.data() + start, length);
        {
          std::unique_lock<std::mutex> loggerLock(logger.mutex());
          logger
            << "Reached out-of-sequence newline while parsing header name."
            << std::endl;
          return "";
        }
        goto parse_header_name;
      }
      case ':': {
        length = (position - start) - 1;
        view = std::string_view(message.data() + start, length);
        hasContentLength = (view == "CONTENT-LENGTH");
        goto parse_header_value;
      }
      default: {
        // empty
      }
      }
    } while (true);
  parse_header_value:
    start = position;
    c = nextChar();
    switch (c) { // ignore leading whitespace
    case ' ': // fallthrough
    case '\t': {
      goto parse_header_value;
    }
    }
    do {
      switch (c) {
      case '\r': {
        length = (position - start) - 1;
        view = std::string_view(message.data() + start, length);
        c = nextChar();
        if (c != '\n') {
          {
            std::unique_lock<std::mutex> loggerLock(logger.mutex());
            logger << "Expected \\r to be followed by \\n, not '";
            logEscaped(c);
            logger << "\': " << view << std::endl;
          }
          goto parse_header_name;
        }
        if (hasContentLength) {
          numBytes = std::stoull(std::string(view));
        }
        goto parse_header_name;
      }
      case '\n': {
        length = (position - start) - 1;
        view = std::string_view(message.data() + start, length);
        {
          std::unique_lock<std::mutex> loggerLock(logger.mutex());
          logger
            << "Reached out-of-sequence newline while parsing header value."
            << std::endl;
        }
        goto parse_header_name;
      }
      default: {
        // empty
      }
      }
      c = nextChar();
    } while (true);
  parse_body:
    start = position;
    for (std::size_t readBytes = 0; readBytes < numBytes; ++readBytes) {
      nextChar();
    }
    length = (position - start);
    std::string body = message.substr(start, length);
    {
      std::unique_lock<std::mutex> loggerLock(logger.mutex());
      logger
        << std::endl
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl
        << "<<<<<<<<<<<<<<  INCOMING  <<<<<<<<<<<<<<" << std::endl
        << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
      std::string::size_type position = 0;
      std::string::size_type index = message.find('\n', position);
      if (index != std::string::npos) {
        std::string::size_type length = (index - position) - 1;
        std::string_view line(message.data() + position, length);
        logger << "<<< " << line << std::endl;
        position = index + 1;
        while ((index = message.find('\n', position)) != std::string::npos) {
          length = (index - position) - 1;
          line = std::string_view(message.data() + position, length);
          logger << "<<< " << line << std::endl;
          position = index + 1;
        }
        length = (message.length() - position);
        if (length > 0) {
          line = std::string_view(message.data() + position, length);
          logger << "<<< " << line << std::endl;
        }
      } else {
        logger << "<<< " << message << std::endl;
      }
    }
    message.clear();
    position = 0;
    return body;
  }

} // namespace LCompilers::LanguageServerProtocol
