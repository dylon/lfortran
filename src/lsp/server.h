#ifndef LCOMPILERS_LSP_SERVER_H
#define LCOMPILERS_LSP_SERVER_H

#include <lsp/specification.h>

namespace LCompilers::LanguageServiceProvider {

  class LanguageServer {
  public:
    LanguageServer();
    ~LanguageServer();
    auto handle(const RequestMessage &request) -> ResponseMessage;
    auto handle(const NotificationMessage &notification) -> ResponseMessage;
    auto initialize() -> void;
    auto initialized() -> void;
  };

} // namespace LCompilers::LanguageServiceProvider::AST

#endif // LCOMPILERS_LSP_SERVER_H
