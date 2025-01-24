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
    auto handleCallHierarchyIncomingCalls(
      const CallHierarchyIncomingCallsParams &params
    ) -> CallHierarchyIncomingCallsResult;
    auto handleCallHierarchyOutgoingCalls(
      const CallHierarchyOutgoingCallsParams &params
    ) -> CallHierarchyOutgoingCallsResult;
    auto handleCodeLensResolve(const CodeLens &codeLens) -> CodeLens;
    auto handleDocumentLinkResolve(const DocumentLink &params) -> DocumentLink;
    auto handleInitialize(const InitializeParams &params) -> InitializeResult;
    auto handleShutdown() -> void;
    auto handleTextDocumentCodeLens(const CodeLensParams &params) -> CodeLensResult;
    auto handleTextDocumentDeclaration(
      const DeclarationParams &params
    ) -> GotoResult;
    auto handleTextDocumentDefinition(
      const DefinitionParams &params
    ) -> GotoResult;
    auto handleTextDocumentDocumentHighlight(
      const DocumentHighlightParams &params
    ) -> DocumentHighlightResult;
    auto handleTextDocumentDocumentLink(
      const DocumentLinkParams &params
    ) -> DocumentLinkResult;
    auto handleTextDocumentDocumentSymbol(
      const DocumentSymbolParams &params
    ) -> DocumentSymbolResult;
    auto handleTextDocumentFoldingRange(
      const FoldingRangeParams &params
    ) -> FoldingRangeResult;
    auto handleTextDocumentHover(
      const HoverParams &params
    ) -> HoverResult;
    auto handleTextDocumentImplementation(
      const ImplementationParams &params
    ) -> GotoResult;
    auto handleTextDocumentPrepareCallHierarchy(
      const CallHierarchyPrepareParams &params
    ) -> PrepareCallHierarchyResult;
    auto handleTextDocumentPrepareTypeHierarchy(
      const TypeHierarchyPrepareParams &params
    ) -> TypeHierarchyResult;
    auto handleTextDocumentReferences(
      const ReferenceParams &params
    ) -> FindReferencesResult;
    auto handleTextDocumentSelectionRange(
      const SelectionRangeParams &params
    ) -> SelectionRangeResult;
    auto handleTextDocumentSemanticTokensFull(
      const SemanticTokensParams &params
    ) -> SemanticTokensResult;
    auto handleTextDocumentTypeDefinition(
      const TypeDefinitionParams &params
    ) -> GotoResult;
    auto handleTextDocumentWillSaveWaitUntil(
      const WillSaveTextDocumentParams &params
    ) -> WillSaveWaitUntilResult;
    auto handleTypeHierarchySubtypes(
      const TypeHierarchySubtypesParams &params
    ) -> TypeHierarchyResult;
    auto handleTypeHierarchySupertypes(
      const TypeHierarchySupertypesParams &params
    ) -> TypeHierarchyResult;
    auto handleWorkspaceCodeLensRefresh() -> void;

    // notification: client -> server
    auto handleCancelRequest(const CancelParams &params) -> void;
    auto handleExit() -> void;
    auto handleInitialized(const InitializedParams &params) -> void;
    auto handleNotebookDocumentDidChange(
      const DidChangeNotebookDocumentParams &params
    ) -> void;
    auto handleNotebookDocumentdidClose(
      const DidCloseNotebookDocumentParams &params
    ) -> void;
    auto handleNotebookDocumentDidOpen(
      const DidOpenNotebookDocumentParams &params
    ) -> void;
    auto handleNotebookDocumentDidSave(
      const DidSaveNotebookDocumentParams &params
    ) -> void;
    auto handleSetTrace(const SetTraceParams &params) -> void;
    auto handleTextDocumentDidChange(
      DidChangeTextDocumentParams &params
    ) -> void;
    auto handleTextDocumentDidClose(
      const DidCloseTextDocumentParams &params
    ) -> void;
    auto handleTextDocumentDidOpen(
      const DidOpenTextDocumentParams &params
    ) -> void;
    auto handleTextDocumentDidSave(
      const DidSaveTextDocumentParams &params
    ) -> void;

    // request: server -> client
    auto requestClientRegisterCapability(
      const RegistrationParams &params
    ) -> void;
    auto requestClientUnregisterCapability(
      const UnregistrationParams &params
    ) -> void;

    // notification: server -> client
    template <typename T>
    auto notify(const std::string &method, const T &params) -> void;
    auto notifyLogTrace(const LogTraceParams &params) -> void;
    auto notifyProgress(const ProgressParams &params) -> void;
    auto notifyTextDocumentPublishDiagnostics(
      const PublishDiagnosticsParams &params
    ) -> void;

    void assertInitialized();
    void assertRunning();
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_LFORTRAN_LANGUAGE_SERVER_H
