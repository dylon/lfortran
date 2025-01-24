#include "lsp/lsp_language_server.h"
#include <lsp/specification.h>
#include <lsp/lfortran_lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  LFortranLspLanguageServer::LFortranLspLanguageServer(
    ls::MessageQueue &outgoingMessages
  ) : LspLanguageServer(outgoingMessages)
  {
    // empty
  }

  // request: "initialize"
  auto LFortranLspLanguageServer::handleInitialize(
    const InitializeParams &params
  ) -> InitializeResult {
    InitializeResult result = LspLanguageServer::handleInitialize(params);
    return result;
  }

} // namespace LCompilers::LanguageServerProtocol
