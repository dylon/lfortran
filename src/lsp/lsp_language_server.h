#ifndef LCOMPILERS_LSP_LANGUAGE_SERVER_H
#define LCOMPILERS_LSP_LANGUAGE_SERVER_H

#include <map>
#include <shared_mutex>

#include <lsp/language_server.h>
#include <lsp/lsp_serializer.h>
#include <lsp/specification.h>
#include <lsp/text_document.h>

namespace LCompilers::LanguageServerProtocol {

  namespace ls = LCompilers::LanguageServer;

  class LspLanguageServer : public ls::LanguageServer {
  public:
    std::string serve(const std::string &request) override;
  protected:
    JsonRpcLspSerializer serializer;
    JsonRpcLspDeserializer deserializer;
    std::map<DocumentUri, TextDocument> textDocuments;
    std::shared_mutex readWriteMutex;

    auto dispatch(
      ResponseMessage &response,
      const RequestMessage &request
    ) -> void;
    auto dispatch(
      ResponseMessage &response,
      const NotificationMessage &notification
    ) -> void;

    InitializeResult initialize(const InitializeParams &params);

    void initialized(const InitializedParams &params);
    void didOpenTextDocument(const DidOpenTextDocumentParams &params);
    void didChangeTextDocument(DidChangeTextDocumentParams &params);
    void didSaveTextDocument(const DidSaveTextDocumentParams &params);
    void didCloseTextDocument(const DidCloseTextDocumentParams &params);

    auto asInitializeParams(
      const RequestParams &requestParams
    ) const -> InitializeParams;

    auto asInitializedParams(
      const optional_ptr<NotificationParams> &notificationParams
    ) const -> InitializedParams;
    auto asDidOpenTextDocumentParams(
      const NotificationParams &notificationParams
    ) const -> DidOpenTextDocumentParams;
    auto asDidChangeTextDocumentParams(
      const NotificationParams &notificationParams
    ) const -> DidChangeTextDocumentParams;
    auto asDidSaveTextDocumentParams(
      const NotificationParams &notificationParams
    ) const -> DidSaveTextDocumentParams;
    auto asDidCloseTextDocumentParams(
      const NotificationParams &notificationParams
    ) const -> DidCloseTextDocumentParams;

    auto resultToResponseMessage(
      const InitializeResult &result
    ) const -> ResponseMessage;

    auto anyToClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ClientCapabilities>;
    auto anyToTextDocumentItem(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentItem>;
    auto anyToTextDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentIdentifier>;
    auto anyToVersionedTextDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<VersionedTextDocumentIdentifier>;
    auto anyToTextDocumentContentChangeEvent(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentContentChangeEvent>;
    auto anyToString(
      const LSPAny &any
    ) const -> const std::string &;
    auto anyToInt(
      const LSPAny &any
    ) const -> int;

    auto assertAnyType(
      const std::string &name,
      const LSPAny &any,
      LSPAnyType type
    ) const -> void;

    auto assertNotificationType(
      const std::string &method,
      const NotificationParams &params,
      NotificationParamsType type
    ) const -> void;

    auto lspToAny(
      const InitializeResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const ServerCapabilities &capabilities
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const TextDocumentSync &textDocumentSync
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToObject(
      const TextDocumentSyncOptions &options
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const SaveOptions &options
    ) const -> std::unique_ptr<LSPObject>;

    auto requireRequestParams(
      const RequestMessage &request
    ) const -> const RequestParams &;

    auto requireNotificationParams(
      const NotificationMessage &notification
    ) const -> const NotificationParams &;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
