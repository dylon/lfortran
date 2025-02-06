#ifndef LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
#define LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H

#include <lsp/logger.h>
#include <lsp/lsp_language_server.h>
#include <lsp/lsp_serializer.h>
#include <lsp/message_queue.h>
#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {
  namespace lsl = LCompilers::LanguageServer::Logging;

  class LFortranLspLanguageServer : public LspLanguageServer {
  public:
    LFortranLspLanguageServer(
      ls::MessageQueue &outgoingMessages,
      lsl::Logger &logger
    );
  protected:
    auto handleInitialize(InitializeParams &params) -> InitializeResult;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
