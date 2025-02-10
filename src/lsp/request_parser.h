#pragma once

#include <map>
#include <memory>
#include <string>

#include <lsp/logger.h>

namespace LCompilers::LanguageServer {
  namespace lsl = LCompilers::LanguageServer::Logging;

  enum class RequestParserState {
    INITIAL = 0,
    PARSING_START_LINE = 1,
    PARSING_HEADER = 2,
    PARSING_SEPARATOR = 3,
    PARSING_BODY = 4,
    COMPLETE = 5,
    ERROR = 6,
    RESET = 7,
  };

  enum class RequestStartLineParserState {
    INITIAL = 0,
    PARSING_START_LINE = 1,
    PARSING_NEWLINE = 2,
    COMPLETE = 3,
    ERROR = 4,
    RESET = 5,
  };

  enum class RequestHeaderParserState {
    INITIAL = 0,
    PARSING_NAME = 1,
    PARSING_SEPARATOR = 2,
    PARSING_VALUE = 3,
    PARSING_NEWLINE = 4,
    COMPLETE = 5,
    ERROR = 6,
    RESET = 7,
  };

  enum class RequestBodyParserState {
    INITIAL = 0,
    PARSING_BODY = 1,
    COMPLETE = 2,
    ERROR = 3,
    RESET = 4,
  };

  class RequestParser {
  public:
    RequestParser(lsl::Logger &logger);

    inline auto startLine() -> const std::string & {
      return _startLine;
    }

    inline auto headers() -> const std::map<std::string, std::string> & {
      return _headers;
    }

    inline auto body() -> const std::string & {
      return _body;
    }

    inline auto state() -> RequestParserState {
      return _state;
    }

    inline auto error() -> const std::string & {
      return _error;
    }

    void reset();
    void finish();
    virtual bool parse(unsigned char c) = 0;
  protected:
    lsl::Logger &logger;
    std::string _startLine = "";
    std::map<std::string, std::string> _headers;
    std::string _body = "";
    std::string _error = "";
    RequestParserState _state = RequestParserState::INITIAL;
    RequestStartLineParserState startLineState =
      RequestStartLineParserState::INITIAL;
    RequestHeaderParserState headerState = RequestHeaderParserState::INITIAL;
    RequestBodyParserState bodyState = RequestBodyParserState::INITIAL;
  };

  class RequestParserFactory {
  public:
    RequestParserFactory(lsl::Logger &logger);
    virtual std::unique_ptr<RequestParser> build() = 0;
  protected:
    lsl::Logger &logger;
  };

} // namespace LCompilers::LanguageServer
