#ifndef LCOMPILERS_LSP_TCP_LSP_REQUEST_PARSER_H
#define LCOMPILERS_LSP_TCP_LSP_REQUEST_PARSER_H

#include <cstddef>
#include <sstream>

#include <lsp/logger.h>
#include <lsp/request_parser.h>

namespace LCompilers::LanguageServerProtocol {

  namespace ls = LCompilers::LanguageServer;
  namespace lsl = LCompilers::LanguageServer::Logging;

  class LspRequestParser : public ls::RequestParser {
  public:
    LspRequestParser(bool interactive, lsl::Logger &logger);
    bool parse(unsigned char c) override;
  private:
    bool interactive;
    std::size_t numBytes = 0;
    std::size_t readBytes = 0;
    std::stringstream ss;
    std::string header = "";

    std::size_t line = 0;
    std::size_t column = 0;
    unsigned char b = '\0';

    bool escaped = false;

    auto parseStartLine(unsigned char c) -> bool;
    auto parseHeader(unsigned char c) -> bool;
    auto parseBody(unsigned char c) -> bool;
  };

  class LspRequestParserFactory : public ls::RequestParserFactory {
  public:
    LspRequestParserFactory(bool interactive, lsl::Logger &logger);
    std::unique_ptr<ls::RequestParser> build() override;
  private:
    bool interactive;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_TCP_LSP_REQUEST_PARSER_H
