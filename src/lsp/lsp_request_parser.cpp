#include <format>

#include "lsp/request_parser.h"

#include <lsp/lsp_request_parser.h>

namespace LCompilers::LanguageServerProtocol {

  LspRequestParser::LspRequestParser(bool interactive)
    : interactive(interactive)
  {
    // empty
  }

  void LspRequestParser::finish() {
    _state = ls::RequestParserState::ERROR;
    _error = "Finished prematurely.";
  }

  auto LspRequestParser::parse(unsigned char c) -> bool {
    bool done = false;
    switch (_state) {
    case ls::RequestParserState::PARSING_START_LINE: {
      if ((startLineState != ls::RequestStartLineParserState::INITIAL)
          || (c != '{')) {
        done = parseStartLine(c);
      } else {
        _state = ls::RequestParserState::PARSING_BODY;
        done = parseBody(c);
      }
      break;
    }
    case ls::RequestParserState::PARSING_HEADER: {
      if ((headerState != ls::RequestHeaderParserState::INITIAL)
          || (c != '{')) {
        done = parseHeader(c);
      } else {
        _state = ls::RequestParserState::PARSING_BODY;
        done = parseBody(c);
      }
      break;
    }
    case ls::RequestParserState::PARSING_SEPARATOR: {
      _state = ls::RequestParserState::PARSING_BODY;
      if ((c == '\n') || (escaped && (c == 'n'))) {
        escaped = false;
        break;
      } else if ((c == '\\') && interactive && !escaped) {
        escaped = true;
      }
    } // fallthrough
    case ls::RequestParserState::PARSING_BODY: {
      done = parseBody(c);
      break;
    }
    case ls::RequestParserState::ERROR: /*{
      _state = ls::RequestParserState::RESET;
      goto reset;
    }*/ // fallthrough
    case ls::RequestParserState::COMPLETE: /*{
      _state = ls::RequestParserState::RESET;
    }*/ // fallthrough
    case ls::RequestParserState::RESET: {
    // reset:
      line = 0;
      column = 0;
      ss.str("");
      headerState = ls::RequestHeaderParserState::RESET;
      startLineState = ls::RequestStartLineParserState::RESET;
      bodyState = ls::RequestBodyParserState::RESET;
      // _state = ls::RequestParserState::INITIAL;
    } // fallthrough
    case ls::RequestParserState::INITIAL: {
      if (c == '{') {
        if (numBytes == 0) {
          _state = ls::RequestParserState::COMPLETE;
          done = true;
        } else {
          _state = ls::RequestParserState::PARSING_BODY;
          done = parseBody(c);
        }
      } else {
        _state = ls::RequestParserState::PARSING_START_LINE;
        done = parseStartLine(c);
      }
    }
    }

    switch (c) {
    case '\n': {
      if (b != '\r') {
        ++line;
        column = 0;
      }
      break;
    }
    case '\r': {
      ++line;
      column = 0;
      break;
    }
    default: {
      ++column;
    }
    }
    b = c;

    return done;
  }

  auto LspRequestParser::parseStartLine(unsigned char c) -> bool {
    switch (startLineState) {
    case ls::RequestStartLineParserState::ERROR: // fallthrough
    case ls::RequestStartLineParserState::COMPLETE: // fallthrough
    case ls::RequestStartLineParserState::RESET: {
      _startLine = "";
      startLineState = ls::RequestStartLineParserState::INITIAL;
    } // fallthrough
    case ls::RequestStartLineParserState::INITIAL: {
      startLineState = ls::RequestStartLineParserState::PARSING_START_LINE;
    } // fallthrough
    case ls::RequestStartLineParserState::PARSING_START_LINE: {
      switch (c) {
      case '\r': // fallthrough
      case '\n': {
        _startLine = ss.str();
        ss.str("");
        startLineState = ls::RequestStartLineParserState::PARSING_NEWLINE;
        goto parseNewline;
      }
      case ':': {
        _state = ls::RequestParserState::PARSING_HEADER;
        headerState = ls::RequestHeaderParserState::PARSING_NAME;
        return parseHeader(c);
      }
      case '\\': {
        if (interactive) {
          if (escaped) {
            ss << '\\';
          }
          escaped = !escaped;
          break;
        }
      } // fallthrough
      default: {
        if (escaped) {
          escaped = false;
          switch (c) {
          case 'n': {
            return parseStartLine('\n');
            break;
          }
          case 'r': {
            return parseStartLine('\r');
            break;
          }
          case 't': {
            ss << '\t';
            break;
          }
          case '\'': {
            ss << '\'';
            break;
          }
          case '"': {
            ss << '"';
            break;
          }
          case 'b': {
            ss << '\b';
            break;
          }
          case 'f': {
            ss << '\f';
            break;
          }
          default: {
            ss << (unsigned char) std::toupper(c);
          }
          }
        } else {
          ss << (unsigned char) std::toupper(c);
        }
      }
      }
      break;
    }
    case ls::RequestStartLineParserState::PARSING_NEWLINE: {
    parseNewline:
      switch (c) {
      case '\n': {
        _state = ls::RequestParserState::PARSING_HEADER;
        startLineState = ls::RequestStartLineParserState::COMPLETE;
        break;
      }
      case '\r': {
        break;
      }
      case '\\': {
        if (interactive && !escaped) {
          escaped = true;
          break;
        }
      } // fallthrough
      default: {
        if (escaped) {
          escaped = false;
          switch (c) {
          case 'n': {
            return parseHeader('\n');
            break;
          }
          case 'r': {
            return parseHeader('\r');
            break;
          }
          }
        }
        _error = std::format(
          "Reached non-newline character on line={}, column={} while parsing newline: {}",
          line, column, c
        );
        goto error;
      }
      }
      break;
    }
    }
    return false;
  error:
    _state = ls::RequestParserState::ERROR;
    startLineState = ls::RequestStartLineParserState::ERROR;
    return true;
  }

  auto LspRequestParser::parseHeader(unsigned char c) -> bool {
    switch (headerState) {
    case ls::RequestHeaderParserState::ERROR: /* {
      headerState = ls:RequestHeaderParserState::RESET;
      goto reset;
    }*/ // fallthrough
    case ls::RequestHeaderParserState::COMPLETE: /*{
      headerState = ls:RequestHeaderParserState::RESET;
    }*/ // fallthrough
    case ls::RequestHeaderParserState::RESET: {
    // reset:
      numBytes = 0;
      _headers.clear();
      header = "";
      // headerState = ls::RequestHeaderParserState::INITIAL;
    } // fallthrough
    case ls::RequestHeaderParserState::INITIAL: {
      headerState = ls::RequestHeaderParserState::PARSING_NAME;
    } // fallthrough
    case ls::RequestHeaderParserState::PARSING_NAME: {
      switch (c) {
      case '\r': // fallthrough
      case '\n': {
        if (column == 0) {
          _state = ls::RequestParserState::PARSING_SEPARATOR;
          headerState = ls::RequestHeaderParserState::COMPLETE;
          break;
        }
        _error = std::format(
          "Reached newline character on line={}, column={}, while parsing header name: {}",
          line, column, ((c == '\r') ? "\\r" : "\\n")
        );
        goto error;
      }
      case ':': {
        escaped = false;
        header = ss.str();
        ss.str("");
        headerState = ls::RequestHeaderParserState::PARSING_SEPARATOR;
        break;
      }
      case '\\': {
        if (interactive) {
          if (escaped) {
            ss << '\\';
          }
          escaped = !escaped;
          break;
        }
      } // fallthrough
      default: {
        if (escaped) {
          escaped = false;
          switch (c) {
          case 'n': {
            return parseHeader('\n');
            break;
          }
          case 'r': {
            return parseHeader('\r');
            break;
          }
          case 't': {
            ss << '\t';
            break;
          }
          case '\'': {
            ss << '\'';
            break;
          }
          case '"': {
            ss << '"';
            break;
          }
          case 'b': {
            ss << '\b';
            break;
          }
          case 'f': {
            ss << '\f';
            break;
          }
          default: {
            ss << (unsigned char) std::toupper(c);
          }
          }
        } else {
          ss << (unsigned char) std::toupper(c);
        }
      }
      }
      break;
    }
    case ls::RequestHeaderParserState::PARSING_SEPARATOR: {
      switch (c) {
      case ' ': // fallthrough
      case '\t': {
        break;
      }
      case '\n': // fallthrough
      case '\r': {
        _error = std::format(
          "Reached newline character on line={}, column={} while parsing header separator: {}",
          line, column, ((c == '\r') ? "\\r" : "\\n")
        );
        goto error;
      }
      default: {
        headerState = ls::RequestHeaderParserState::PARSING_VALUE;
        goto parseValue;
      }
      }
      break;
    }
    case ls::RequestHeaderParserState::PARSING_VALUE: {
    parseValue:
      switch (c) {
      case '\n': // fallthrough
      case '\r': {
        std::string value = ss.str();
        ss.str("");
        _headers[header] = value;
        if (header == "CONTENT-LENGTH") {
          numBytes = std::stoull(value);
        }
        header = "";
        headerState = ls::RequestHeaderParserState::PARSING_NEWLINE;
        goto parseNewline;
      }
      case '\\': {
        if (interactive) {
          if (escaped) {
            ss << '\\';
          }
          escaped = !escaped;
          break;
        }
      } // fallthrough
      default: {
        if (escaped) {
          escaped = false;
          switch (c) {
          case 'n': {
            return parseHeader('\n');
            break;
          }
          case 'r': {
            return parseHeader('\r');
            break;
          }
          case 't': {
            ss << '\t';
            break;
          }
          case '\'': {
            ss << '\'';
            break;
          }
          case '"': {
            ss << '"';
            break;
          }
          case 'b': {
            ss << '\b';
            break;
          }
          case 'f': {
            ss << '\f';
            break;
          }
          default: {
            ss << c;
          }
          }
        } else {
          ss << c;
        }
      }
      }
      break;
    }
    case ls::RequestHeaderParserState::PARSING_NEWLINE: {
    parseNewline:
      switch (c) {
      case '\n': {
        headerState = ls::RequestHeaderParserState::INITIAL;
        break;
      }
      case '\r': {
        break;
      }
      case '\\': {
        if (interactive && !escaped) {
          escaped = true;
          break;
        }
      } // fallthrough
      default: {
        if (escaped) {
          escaped = false;
          switch (c) {
          case 'n': {
            return parseHeader('\n');
            break;
          }
          case 'r': {
            return parseHeader('\r');
            break;
          }
          }
        }
        _error = std::format(
          "Reached non-newline character on line={}, column={} while parsing newline: {}",
          line, column, c
        );
        goto error;
      }
      }
      break;
    }
    default: {
      break;
    }
    }
    return false;
  error:
    _state = ls::RequestParserState::ERROR;
    headerState = ls::RequestHeaderParserState::ERROR;
    return true;
  }

  auto LspRequestParser::parseBody(unsigned char c) -> bool {
    switch (bodyState) {
    case ls::RequestBodyParserState::ERROR: /*{
      bodyState = ls::RequestBodyParserState::RESET;
      goto reset;
    }*/ // fallthrough
    case ls::RequestBodyParserState::COMPLETE: /*{
      bodyState = ls::RequestBodyParserState::RESET;
    }*/ // fallthrough
    case ls::RequestBodyParserState::RESET: {
    // reset:
      _body = "";
      readBytes = 0;
      // bodyState = ls::RequestBodyParserState::INITIAL;
    } // fallthrough
    case ls::RequestBodyParserState::INITIAL: {
      bodyState = ls::RequestBodyParserState::PARSING_BODY;
    } // fallthrough
    case ls::RequestBodyParserState::PARSING_BODY: {
      if (numBytes == 0) {
        _error = std::format(
          "line={}, column={}: No bytes to read.",
          line, column
        );
        goto error;
      }
      if (readBytes < numBytes) {
        ss << c;
        readBytes++;
      }
      if (readBytes == numBytes) {
        _body = ss.str();
        ss.str("");
        _state = ls::RequestParserState::COMPLETE;
        bodyState = ls::RequestBodyParserState::COMPLETE;
        goto done;
      }
    }
    }
    return false;
  error:
    _state = ls::RequestParserState::ERROR;
    bodyState = ls::RequestBodyParserState::ERROR;
  done:
    return true;
  }

  LspRequestParserFactory::LspRequestParserFactory(bool interactive)
    : interactive(interactive)
  {
    // empty
  }

  std::unique_ptr<ls::RequestParser> LspRequestParserFactory::build() {
    return std::make_unique<LspRequestParser>(interactive);
  }

} // namespace LCompilers::LanguageServerProtocol
