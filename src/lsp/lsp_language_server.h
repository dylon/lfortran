#ifndef LCOMPILERS_LSP_LANGUAGE_SERVER_H
#define LCOMPILERS_LSP_LANGUAGE_SERVER_H

#include <atomic>
#include <cstddef>
#include <map>
#include <optional>
#include <shared_mutex>

#include <lsp/language_server.h>
#include <lsp/lsp_serializer.h>
#include <lsp/lsp_transformer.h>
#include <lsp/specification.h>
#include <lsp/text_document.h>

namespace LCompilers::LanguageServerProtocol {

  namespace ls = LCompilers::LanguageServer;

  const std::string JSON_RPC_VERSION = "2.0";

  class LspLanguageServer : public ls::LanguageServer {
  public:
    LspLanguageServer(ls::MessageQueue &outgoingMessages);
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
    std::optional<InitializeParams> _initializeParams;
    std::shared_mutex readWriteMutex;
    std::atomic_bool _initialized = false;
    std::atomic_bool _shutdown = false;
    std::atomic_bool _exit = false;
    std::atomic_int serialId = 0;

    auto nextId() -> RequestId;

    auto dispatch(
      ResponseMessage &response,
      const RequestMessage &request
    ) -> void;
    auto dispatch(
      ResponseMessage &response,
      const NotificationMessage &notification
    ) -> void;

    void prepare(
      std::ostream &os,
      const std::string &response
    ) const override;
    void prepare(
      std::stringstream &ss,
      const std::string &response
    ) const override;

    auto initializeParams() const -> const InitializeParams &;

    // request: client -> server
    auto initialize(const InitializeParams &params) -> InitializeResult;
    auto willSaveWaitUntil(
      const WillSaveTextDocumentParams &params
    ) -> WillSaveWaitUntilResult;
    auto gotoDeclaration(
      const DeclarationParams &params
    ) -> GotoResult;
    auto gotoDefinition(
      const DefinitionParams &params
    ) -> GotoResult;
    auto gotoTypeDefinition(
      const TypeDefinitionParams &params
    ) -> GotoResult;
    auto gotoImplementation(
      const ImplementationParams &params
    ) -> GotoResult;
    auto findReferences(
      const ReferenceParams &params
    ) -> FindReferencesResult;
    auto prepareCallHierarchy(
      const CallHierarchyPrepareParams &params
    ) -> PrepareCallHierarchyResult;
    auto callHierarchyIncomingCalls(
      const CallHierarchyIncomingCallsParams &params
    ) -> CallHierarchyIncomingCallsResult;
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
    auto registerCapability(const RegistrationParams &params) -> void;
    auto unregisterCapability(const UnregistrationParams &params) -> void;

    // notification: server -> client
    auto reportProgress(const ProgressParams &params) -> void;
    auto logTrace(const LogTraceParams &params) -> void;

    void assertInitialized();
    void assertRunning();
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
