#include <lsp/lsp_request_parser.h>

namespace LCompilers::LanguageServer {

  RequestParser::RequestParser(lsl::Logger &logger)
    : logger(logger)
  {
    // empty
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
