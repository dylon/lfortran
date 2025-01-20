#ifndef LCOMPILERS_LSP_LANGUAGE_SERVER_H
#define LCOMPILERS_LSP_LANGUAGE_SERVER_H

#include "lsp/lsp_transformer.h"
#include <atomic>
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
    auto isInitialized() const -> bool;
    auto isShutdown() const -> bool;
    bool isTerminated() const override;
    auto isRunning() const -> bool;
  protected:
    JsonRpcLspSerializer serializer;
    JsonRpcLspDeserializer deserializer;
    LspTransformer transformer;
    std::map<DocumentUri, TextDocument> textDocuments;
    std::shared_mutex readWriteMutex;
    std::atomic_bool _initialized = false;
    std::atomic_bool _shutdown = false;
    std::atomic_bool _exit = false;

    auto dispatch(
      ResponseMessage &response,
      const RequestMessage &request
    ) -> void;
    auto dispatch(
      ResponseMessage &response,
      const NotificationMessage &notification
    ) -> void;

    // request: client -> server
    auto initialize(const InitializeParams &params) -> InitializeResult;
    auto willSaveWaitUntil(
      const WillSaveTextDocumentParams &params
    ) -> WillSaveWaitUntilResult;
    auto gotoDeclaration(
      const DeclarationParams &params
    ) -> GotoDeclarationResult;
    auto gotoDefinition(
      const DefinitionParams &params
    ) -> GotoDefinitionResult;
    auto shutdown() -> void;

    // notification: client -> server
    auto exit() -> void;
    auto cancelRequest(const CancelParams &params) -> void;
    auto initialized(const InitializedParams &params) -> void;
    auto setTrace(const SetTraceParams &params) -> void;
    auto didOpenTextDocument(const DidOpenTextDocumentParams &params) -> void;
    auto didChangeTextDocument(DidChangeTextDocumentParams &params) -> void;
    auto didSaveTextDocument(const DidSaveTextDocumentParams &params) -> void;
    auto didCloseTextDocument(const DidCloseTextDocumentParams &params) -> void;
    auto didOpenNotebookDocument(
      const DidOpenNotebookDocumentParams &params
    ) -> void;
    auto didChangeNotebookDocument(
      const DidChangeNotebookDocumentParams &params
    ) -> void;
    auto didSaveNotebookDocument(
      const DidSaveNotebookDocumentParams &params
    ) -> void;
    auto didCloseNotebookDocument(
      const DidCloseNotebookDocumentParams &params
    ) -> void;

    // request: server -> client

    // notification: server -> client

    void assertInitialized();
    void assertRunning();
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
