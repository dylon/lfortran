#ifndef LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
#define LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H

#include "lsp/message_queue.h"
#include <lsp/specification.h>
#include <lsp/lsp_language_server.h>
#include <lsp/lsp_serializer.h>

namespace LCompilers::LanguageServerProtocol {

  class LFortranLspLanguageServer : public LspLanguageServer {
  public:
    LFortranLspLanguageServer(ls::MessageQueue &outgoingMessages);
  protected:
    auto handleInitialize(InitializeParams &params) -> InitializeResult;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
