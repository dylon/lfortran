#pragma once

#include <istream>

#include <lsp/logger.h>
#include <lsp/message_stream.h>

namespace LCompilers::LanguageServerProtocol {
  namespace ls = LCompilers::LanguageServer;
  namespace lsl = LCompilers::LanguageServer::Logging;

  class LspMessageStream : public ls::MessageStream {
  public:
    LspMessageStream(std::istream &istream, lsl::Logger &logger);
    std::string next() override;
  private:
    std::istream &istream;
    lsl::Logger &logger;
    std::string message;
    std::size_t position;

    auto nextChar() -> char;
    auto nextUpper() -> char;
    auto logEscaped(char c) -> void;
  };

} // namespace LCompilers::LanguageServerProtocol
