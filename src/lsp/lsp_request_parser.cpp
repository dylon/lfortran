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
    case ls::RequestParserState::PARSING_HEADER: {
      if ((headerState != ls::RequestHeaderParserState::INITIAL)
          || (c != '{')) {
        return parseHeader(c);
      } else {
        _state = ls::RequestParserState::PARSING_BODY;
        // fallthrough
      }
    } // fallthrough
    case ls::RequestParserState::PARSING_BODY: {
      return parseBody(c);
    }
    case ls::RequestParserState::ERROR: // fallthrough
    case ls::RequestParserState::RESET: // fallthrough
    case ls::RequestParserState::COMPLETE: {
      _headers.clear();
      _body = "";
      _state = ls::RequestParserState::INITIAL;
      ss.str("");
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
      _state = ls::RequestParserState::PARSING_HEADER;
      return parseHeader(c);
    }
    }
    // NOTE: Should not be reachable ...
    return false;
  }

  auto LspRequestParser::parseHeader(unsigned char c) -> bool {
    switch (headerState) {
    case ls::RequestHeaderParserState::RESET: // fallthrough
    case ls::RequestHeaderParserState::INITIAL: {
      numBytes = 0;
      readBytes = 0;
      headerState = ls::RequestHeaderParserState::PARSING_NAME;
    } // fallthrough
    case ls::RequestHeaderParserState::PARSING_NAME: {
      switch (c) {
      case '\r': // fallthrough
      case '\n': {
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
      default: {
        if (escaped) {
          switch (c) {
          case 'n': {
            ss << '\n';
            break;
          }
          case 'r': {
            ss << '\r';
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
          escaped = false;
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
        goto startParseValueLoop;
      }
      }
      break;
    }
    case ls::RequestHeaderParserState::PARSING_VALUE: {
    startParseValueLoop:
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
        goto startParseNewlineLoop;
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
    startParseNewlineLoop:
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
    return true;
  }

  auto LspRequestParser::parseBody(unsigned char c) -> bool {
    if (readBytes < numBytes) {
      ss << c;
      readBytes++;
    }
    if (readBytes == numBytes) {
      _body = ss.str();
      ss.str("");
      _state = ls::RequestParserState::COMPLETE;
      return true;
    }
    return false;
  }

} // namespace LCompilers::LanguageServerProtocol
