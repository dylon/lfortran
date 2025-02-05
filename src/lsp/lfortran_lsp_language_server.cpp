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
    InitializeParams &params
  ) -> InitializeResult {
    InitializeResult result = LspLanguageServer::handleInitialize(params);
    // add additional function for lfortran
    return result;
  }

} // namespace LCompilers::LanguageServerProtocol
