#include "lsp/lsp_language_server.h"
#include <lsp/specification.h>
#include <lsp/lfortran_lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  // request: "initialize"
  auto LFortranLspLanguageServer::initialize(
    const InitializeParams &params
  ) -> InitializeResult {
    InitializeResult result = LspLanguageServer::initialize(params);
    return result;
  }

} // namespace LCompilers::LanguageServerProtocol
