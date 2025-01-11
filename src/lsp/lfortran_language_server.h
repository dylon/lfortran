#ifndef LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
#define LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H

#include <lsp/specification.h>

namespace LCompilers::LanguageServiceProtocol {

  class LFortranLanguageServer {
  public:
    LFortranLanguageServer();
    ~LFortranLanguageServer();
    auto handle(const RequestMessage &request) -> ResponseMessage;
    auto handle(const NotificationMessage &notification) -> ResponseMessage;
    auto initialize() -> void;
    auto initialized() -> void;
  };

} // namespace LCompilers::LanguageServiceProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
