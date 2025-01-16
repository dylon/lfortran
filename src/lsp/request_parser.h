#ifndef LCOMPILERS_LS_TCP_REQUEST_PARSER_H
#define LCOMPILERS_LS_TCP_REQUEST_PARSER_H

#include <map>
#include <string>

namespace LCompilers::LanguageServer {

  enum class RequestParserState {
    INITIAL,
    PARSING_HEADER,
    PARSING_BODY,
    COMPLETE,
    RESET,
    ERROR,
  };

  enum class RequestHeaderParserState {
    INITIAL,
    PARSING_NAME,
    PARSING_SEPARATOR,
    PARSING_VALUE,
    PARSING_NEWLINE,
    RESET,
  };

  class RequestParser {
  public:
    auto headers() -> const std::map<std::string, std::string> &;
    auto body() -> const std::string &;
    auto state() -> RequestParserState;

    void reset();

    virtual bool parse(unsigned char c) = 0;

  protected:
    std::map<std::string, std::string> _headers;
    std::string _body = "";
    RequestParserState _state = RequestParserState::INITIAL;
    RequestHeaderParserState headerState = RequestHeaderParserState::INITIAL;
  };

} // namespace LCompilers::LanguageServer

#endif // LCOMPILERS_LS_TCP_REQUEST_PARSER_H
