#include <lsp/lsp_request_parser.h>

namespace LCompilers::LanguageServer {

  RequestParser::RequestParser(lsl::Logger &logger)
    : logger(logger)
  {
    // empty
  }

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

  void RequestParser::finish() {
    switch (_state) {
    case RequestParserState::INITIAL: break;
    case RequestParserState::COMPLETE: break;
    case RequestParserState::ERROR: break;
    default: {
      _state = RequestParserState::ERROR;
      _error = "Finished prematurely.";
    }
    }
  }

  RequestParserFactory::RequestParserFactory(lsl::Logger &logger)
    : logger(logger)
  {
    // empty
  }

} // LCompilers::LanguageServer
