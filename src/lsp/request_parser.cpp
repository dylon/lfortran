#include <lsp/lsp_request_parser.h>

namespace LCompilers::LanguageServer {

  auto RequestParser::startLine() -> const std::string & {
    return _startLine;
  }

  auto RequestParser::headers() -> const std::map<std::string, std::string> & {
    return _headers;
  }

  auto RequestParser::body() -> const std::string & {
    return _body;
  }

  auto RequestParser::state() -> RequestParserState {
    return _state;
  }

  auto RequestParser::error() -> const std::string & {
    return _error;
  }

  void RequestParser::reset() {
    _state = RequestParserState::RESET;
  }

} // LCompilers::LanguageServer
