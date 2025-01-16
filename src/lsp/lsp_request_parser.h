#ifndef LCOMPILERS_LSP_TCP_LSP_REQUEST_PARSER_H
#define LCOMPILERS_LSP_TCP_LSP_REQUEST_PARSER_H

#include <cstddef>
#include <sstream>

#include <lsp/request_parser.h>

namespace LCompilers::LanguageServerProtocol {

  namespace ls = LCompilers::LanguageServer;

  class LspRequestParser : public ls::RequestParser {
  public:
    LspRequestParser(bool interactive);
    bool parse(unsigned char c) override;
  private:
    bool interactive;
    std::size_t numBytes = 0;
    std::size_t readBytes = 0;
    std::stringstream ss;
    std::string header = "";

    bool escaped = false;

    auto parseHeader(unsigned char c) -> bool;
    auto parseBody(unsigned char c) -> bool;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_TCP_LSP_REQUEST_PARSER_H
