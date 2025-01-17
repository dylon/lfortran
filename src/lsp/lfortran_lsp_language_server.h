#ifndef LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
#define LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H

#include <lsp/specification.h>
#include <lsp/language_server.h>
#include <lsp/lsp_serializer.h>

namespace LCompilers::LanguageServerProtocol {

  namespace ls = LCompilers::LanguageServer;

  class LFortranLspLanguageServer : public ls::LanguageServer {
  public:
    std::string serve(const std::string &request) override;
  private:
    JsonRpcLspSerializer serializer;
    JsonRpcLspDeserializer deserializer;

    auto dispatch(const RequestMessage &request) -> ResponseMessage;
    auto dispatch(const NotificationMessage &notification) -> ResponseMessage;

    auto initialize(const InitializeParams &params) -> InitializeResult;

    auto buildClientCapabilities(
      const LSPAny &lspAny
    ) const -> std::unique_ptr<ClientCapabilities>;
    auto buildInitializeParams(
      const RequestParams &requestParams
    ) const -> InitializeParams;
    auto buildResponseMessage(
      const InitializeResult &result
    ) const -> ResponseMessage;

    auto setResponseId(
      const RequestMessage &request,
      ResponseMessage &response
    ) const -> void;
    auto setResponseId(
      const NotificationMessage &notification,
      ResponseMessage &response
    ) const -> void;

    auto lspToAny(
      const InitializeResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const ServerCapabilities &capabilities
    ) const -> std::unique_ptr<LSPAny>;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
