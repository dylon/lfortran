#ifndef LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
#define LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H

#include <lsp/specification.h>
#include <lsp/lsp_language_server.h>
#include <lsp/lsp_serializer.h>

namespace LCompilers::LanguageServerProtocol {

  class LFortranLspLanguageServer : public LspLanguageServer {
  protected:
    InitializeResult initialize(const InitializeParams &params);
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
