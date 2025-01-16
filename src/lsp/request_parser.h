#ifndef LCOMPILERS_LS_TCP_REQUEST_PARSER_H
#define LCOMPILERS_LS_TCP_REQUEST_PARSER_H

#include <map>
#include <string>

namespace LCompilers::LanguageServer {

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
    auto startLine() -> const std::string &;
    auto headers() -> const std::map<std::string, std::string> &;
    auto body() -> const std::string &;
    auto state() -> RequestParserState;

    void reset();

    virtual bool parse(unsigned char c) = 0;

  protected:
    std::string _startLine = "";
    std::map<std::string, std::string> _headers;
    std::string _body = "";
    RequestParserState _state = RequestParserState::INITIAL;
    RequestStartLineParserState startLineState =
      RequestStartLineParserState::INITIAL;
    RequestHeaderParserState headerState = RequestHeaderParserState::INITIAL;
    RequestBodyParserState bodyState = RequestBodyParserState::INITIAL;
  };

} // namespace LCompilers::LanguageServer

#endif // LCOMPILERS_LS_TCP_REQUEST_PARSER_H
