#include "lsp/request_parser.h"
#include <iostream>

#include <lsp/lsp_request_parser.h>

namespace LCompilers::LanguageServerProtocol {

  LspRequestParser::LspRequestParser(bool interactive)
    : interactive(interactive)
  {
    // empty
  }

  auto LspRequestParser::parse(unsigned char c) -> bool {
    switch (_state) {
    case ls::RequestParserState::PARSING_START_LINE: {
      if ((startLineState != ls::RequestStartLineParserState::INITIAL)
          || (c != '{')) {
        return parseStartLine(c);
      }
      _state = ls::RequestParserState::PARSING_BODY;
      return parseBody(c);
    }
    case ls::RequestParserState::PARSING_HEADER: {
      if ((headerState != ls::RequestHeaderParserState::INITIAL)
          || (c != '{')) {
        return parseHeader(c);
      }
      _state = ls::RequestParserState::PARSING_BODY;
      return parseBody(c);
    }
    case ls::RequestParserState::PARSING_SEPARATOR: {
      _state = ls::RequestParserState::PARSING_BODY;
      if (c == '\n') {
        break;
      }
    } // fallthrough
    case ls::RequestParserState::PARSING_BODY: {
      return parseBody(c);
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
          return true;
        }
        _state = ls::RequestParserState::PARSING_BODY;
        return parseBody(c);
      }
      _state = ls::RequestParserState::PARSING_START_LINE;
      return parseStartLine(c);
    }
    }
    // NOTE: Should not be reachable ...
    return false;
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
        std::cerr
          << "Reached non-newline character while parsing newline: "
          << c
          << std::endl;
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
    // std::cout << "headerState = " << static_cast<int>(headerState) << std::endl;
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
      readBytes = 0;
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
        if (ss.str().length() == 0) {
          _state = ls::RequestParserState::PARSING_SEPARATOR;
          headerState = ls::RequestHeaderParserState::COMPLETE;
          break;
        }
        std::cerr
          << "Reached newline character while parsing header name: "
          << ((c == '\r') ? "\\r" : "\\n")
          << std::endl;
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
        std::cerr
          << "Reached newline character while parsing header separator: "
          << ((c == '\r') ? "\\r" : "\\n")
          << std::endl;
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
        std::cerr
          << "Reached non-newline character while parsing newline: "
          << c
          << std::endl;
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
      // bodyState = ls::RequestBodyParserState::INITIAL;
    } // fallthrough
    case ls::RequestBodyParserState::INITIAL: {
      bodyState = ls::RequestBodyParserState::PARSING_BODY;
    } // fallthrough
    case ls::RequestBodyParserState::PARSING_BODY: {
      if (readBytes < numBytes) {
        ss << c;
        readBytes++;
      }
      if (readBytes == numBytes) {
        _body = ss.str();
        ss.str("");
        _state = ls::RequestParserState::COMPLETE;
        bodyState = ls::RequestBodyParserState::COMPLETE;
        return true;
      }
      return false;
    }
    }
    // NOTE: Should not be reachable.
    return false;
  }

} // namespace LCompilers::LanguageServerProtocol
