// -----------------------------------------------------------------------------
// NOTE: This file was generated from Microsoft's Language Server Protocol (LSP)
// specification. Please do not edit it by hand.
// -----------------------------------------------------------------------------

#pragma once

#include <atomic>
#include <cstddef>
#include <map>
#include <optional>
#include <shared_mutex>

#include <lsp/language_server.h>
#include <lsp/logger.h>
#include <lsp/lsp_serializer.h>
#include <lsp/lsp_transformer.h>
#include <lsp/specification.h>
#include <lsp/text_document.h>

namespace LCompilers::LanguageServerProtocol {
  namespace ls = LCompilers::LanguageServer;
  namespace lsl = LCompilers::LanguageServer::Logging;

  const std::string JSON_RPC_VERSION = "2.0";

  class LspLanguageServer : public ls::LanguageServer {
  public:
    LspLanguageServer(
      ls::MessageQueue &outgoingMessages,
      lsl::Logger &logger
    );
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
    std::unique_ptr<InitializeParams> _initializeParams;
    std::shared_mutex readWriteMutex;
    std::atomic_bool _initialized = false;
    std::atomic_bool _shutdown = false;
    std::atomic_bool _exit = false;
    std::atomic_int serialId = 0;

    auto nextId() -> RequestId;

    auto initializeParams() const -> const InitializeParams &;

    auto assertInitialized() -> void;

    auto assertRunning() -> void;

    auto dispatch(
      ResponseMessage &response,
      RequestMessage &request
    ) -> void;

    auto dispatch(
      ResponseMessage &response,
      NotificationMessage &notification
    ) -> void;

    void prepare(
      std::ostream &os,
      const std::string &response
    ) const override;

    void prepare(
      std::stringstream &ss,
      const std::string &response
    ) const override;

    auto requireMessageParams(
      RequestMessage &request
    ) const -> MessageParams &;

    auto requireMessageParams(
      NotificationMessage &notification
    ) const -> MessageParams &;

    // ================= //
    // Incoming Requests //
    // ================= //

    /**
     * A request to resolve the implementation locations of a symbol at a given text
     * document position. The request's parameter is of type {@link TextDocumentPositionParams}
     * the response is of type {@link Definition} or a Thenable that resolves to such.
     */
    auto handleTextDocumentImplementation(
      ImplementationParams &params
    ) -> TextDocumentImplementationResult;

    /**
     * A request to resolve the type definition locations of a symbol at a given text
     * document position. The request's parameter is of type {@link TextDocumentPositionParams}
     * the response is of type {@link Definition} or a Thenable that resolves to such.
     */
    auto handleTextDocumentTypeDefinition(
      TypeDefinitionParams &params
    ) -> TextDocumentTypeDefinitionResult;

    /**
     * A request to list all color symbols found in a given text document. The request's
     * parameter is of type {@link DocumentColorParams} the
     * response is of type {@link ColorInformation ColorInformation[]} or a Thenable
     * that resolves to such.
     */
    auto handleTextDocumentDocumentColor(
      DocumentColorParams &params
    ) -> TextDocumentDocumentColorResult;

    /**
     * A request to list all presentation for a color. The request's
     * parameter is of type {@link ColorPresentationParams} the
     * response is of type {@link ColorInformation ColorInformation[]} or a Thenable
     * that resolves to such.
     */
    auto handleTextDocumentColorPresentation(
      ColorPresentationParams &params
    ) -> TextDocumentColorPresentationResult;

    /**
     * A request to provide folding ranges in a document. The request's
     * parameter is of type {@link FoldingRangeParams}, the
     * response is of type {@link FoldingRangeList} or a Thenable
     * that resolves to such.
     */
    auto handleTextDocumentFoldingRange(
      FoldingRangeParams &params
    ) -> TextDocumentFoldingRangeResult;

    /**
     * A request to resolve the type definition locations of a symbol at a given text
     * document position. The request's parameter is of type {@link TextDocumentPositionParams}
     * the response is of type {@link Declaration} or a typed array of {@link DeclarationLink}
     * or a Thenable that resolves to such.
     */
    auto handleTextDocumentDeclaration(
      DeclarationParams &params
    ) -> TextDocumentDeclarationResult;

    /**
     * A request to provide selection ranges in a document. The request's
     * parameter is of type {@link SelectionRangeParams}, the
     * response is of type {@link SelectionRange SelectionRange[]} or a Thenable
     * that resolves to such.
     */
    auto handleTextDocumentSelectionRange(
      SelectionRangeParams &params
    ) -> TextDocumentSelectionRangeResult;

    /**
     * A request to result a `CallHierarchyItem` in a document at a given position.
     * Can be used as an input to an incoming or outgoing call hierarchy.
     *
     * @since 3.16.0
     */
    auto handleTextDocumentPrepareCallHierarchy(
      CallHierarchyPrepareParams &params
    ) -> TextDocumentPrepareCallHierarchyResult;

    /**
     * A request to resolve the incoming calls for a given `CallHierarchyItem`.
     *
     * @since 3.16.0
     */
    auto handleCallHierarchyIncomingCalls(
      CallHierarchyIncomingCallsParams &params
    ) -> CallHierarchyIncomingCallsResult;

    /**
     * A request to resolve the outgoing calls for a given `CallHierarchyItem`.
     *
     * @since 3.16.0
     */
    auto handleCallHierarchyOutgoingCalls(
      CallHierarchyOutgoingCallsParams &params
    ) -> CallHierarchyOutgoingCallsResult;

    /**
     * @since 3.16.0
     */
    auto handleTextDocumentSemanticTokensFull(
      SemanticTokensParams &params
    ) -> TextDocumentSemanticTokensFullResult;

    /**
     * @since 3.16.0
     */
    auto handleTextDocumentSemanticTokensFullDelta(
      SemanticTokensDeltaParams &params
    ) -> TextDocumentSemanticTokensFullDeltaResult;

    /**
     * @since 3.16.0
     */
    auto handleTextDocumentSemanticTokensRange(
      SemanticTokensRangeParams &params
    ) -> TextDocumentSemanticTokensRangeResult;

    /**
     * A request to provide ranges that can be edited together.
     *
     * @since 3.16.0
     */
    auto handleTextDocumentLinkedEditingRange(
      LinkedEditingRangeParams &params
    ) -> TextDocumentLinkedEditingRangeResult;

    /**
     * The will create files request is sent from the client to the server before files are actually
     * created as long as the creation is triggered from within the client.
     *
     * The request can return a `WorkspaceEdit` which will be applied to workspace before the
     * files are created. Hence the `WorkspaceEdit` can not manipulate the content of the file
     * to be created.
     *
     * @since 3.16.0
     */
    auto handleWorkspaceWillCreateFiles(
      CreateFilesParams &params
    ) -> WorkspaceWillCreateFilesResult;

    /**
     * The will rename files request is sent from the client to the server before files are actually
     * renamed as long as the rename is triggered from within the client.
     *
     * @since 3.16.0
     */
    auto handleWorkspaceWillRenameFiles(
      RenameFilesParams &params
    ) -> WorkspaceWillRenameFilesResult;

    /**
     * The did delete files notification is sent from the client to the server when
     * files were deleted from within the client.
     *
     * @since 3.16.0
     */
    auto handleWorkspaceWillDeleteFiles(
      DeleteFilesParams &params
    ) -> WorkspaceWillDeleteFilesResult;

    /**
     * A request to get the moniker of a symbol at a given text document position.
     * The request parameter is of type {@link TextDocumentPositionParams}.
     * The response is of type {@link Moniker Moniker[]} or `null`.
     */
    auto handleTextDocumentMoniker(
      MonikerParams &params
    ) -> TextDocumentMonikerResult;

    /**
     * A request to result a `TypeHierarchyItem` in a document at a given position.
     * Can be used as an input to a subtypes or supertypes type hierarchy.
     *
     * @since 3.17.0
     */
    auto handleTextDocumentPrepareTypeHierarchy(
      TypeHierarchyPrepareParams &params
    ) -> TextDocumentPrepareTypeHierarchyResult;

    /**
     * A request to resolve the supertypes for a given `TypeHierarchyItem`.
     *
     * @since 3.17.0
     */
    auto handleTypeHierarchySupertypes(
      TypeHierarchySupertypesParams &params
    ) -> TypeHierarchySupertypesResult;

    /**
     * A request to resolve the subtypes for a given `TypeHierarchyItem`.
     *
     * @since 3.17.0
     */
    auto handleTypeHierarchySubtypes(
      TypeHierarchySubtypesParams &params
    ) -> TypeHierarchySubtypesResult;

    /**
     * A request to provide inline values in a document. The request's parameter is of
     * type {@link InlineValueParams}, the response is of type
     * {@link InlineValue InlineValue[]} or a Thenable that resolves to such.
     *
     * @since 3.17.0
     */
    auto handleTextDocumentInlineValue(
      InlineValueParams &params
    ) -> TextDocumentInlineValueResult;

    /**
     * A request to provide inlay hints in a document. The request's parameter is of
     * type {@link InlayHintsParams}, the response is of type
     * {@link InlayHint InlayHint[]} or a Thenable that resolves to such.
     *
     * @since 3.17.0
     */
    auto handleTextDocumentInlayHint(
      InlayHintParams &params
    ) -> TextDocumentInlayHintResult;

    /**
     * A request to resolve additional properties for an inlay hint.
     * The request's parameter is of type {@link InlayHint}, the response is
     * of type {@link InlayHint} or a Thenable that resolves to such.
     *
     * @since 3.17.0
     */
    auto handleInlayHintResolve(
      InlayHint &params
    ) -> InlayHintResolveResult;

    /**
     * The document diagnostic request definition.
     *
     * @since 3.17.0
     */
    auto handleTextDocumentDiagnostic(
      DocumentDiagnosticParams &params
    ) -> TextDocumentDiagnosticResult;

    /**
     * The workspace diagnostic request definition.
     *
     * @since 3.17.0
     */
    auto handleWorkspaceDiagnostic(
      WorkspaceDiagnosticParams &params
    ) -> WorkspaceDiagnosticResult;

    /**
     * A request to provide inline completions in a document. The request's parameter is of
     * type {@link InlineCompletionParams}, the response is of type
     * {@link InlineCompletion InlineCompletion[]} or a Thenable that resolves to such.
     *
     * @since 3.18.0
     * @proposed
     */
    auto handleTextDocumentInlineCompletion(
      InlineCompletionParams &params
    ) -> TextDocumentInlineCompletionResult;

    /**
     * The initialize request is sent from the client to the server.
     * It is sent once as the request after starting up the server.
     * The requests parameter is of type {@link InitializeParams}
     * the response if of type {@link InitializeResult} of a Thenable that
     * resolves to such.
     */
    auto handleInitialize(
      InitializeParams &params
    ) -> InitializeResult;

    /**
     * A shutdown request is sent from the client to the server.
     * It is sent once when the client decides to shutdown the
     * server. The only notification that is sent after a shutdown request
     * is the exit event.
     */
    auto handleShutdown() -> ShutdownResult;

    /**
     * A document will save request is sent from the client to the server before
     * the document is actually saved. The request can return an array of TextEdits
     * which will be applied to the text document before it is saved. Please note that
     * clients might drop results if computing the text edits took too long or if a
     * server constantly fails on this request. This is done to keep the save fast and
     * reliable.
     */
    auto handleTextDocumentWillSaveWaitUntil(
      WillSaveTextDocumentParams &params
    ) -> TextDocumentWillSaveWaitUntilResult;

    /**
     * Request to request completion at a given text document position. The request's
     * parameter is of type {@link TextDocumentPosition} the response
     * is of type {@link CompletionItem CompletionItem[]} or {@link CompletionList}
     * or a Thenable that resolves to such.
     *
     * The request can delay the computation of the {@link CompletionItem.detail `detail`}
     * and {@link CompletionItem.documentation `documentation`} properties to the `completionItem/resolve`
     * request. However, properties that are needed for the initial sorting and filtering, like `sortText`,
     * `filterText`, `insertText`, and `textEdit`, must not be changed during resolve.
     */
    auto handleTextDocumentCompletion(
      CompletionParams &params
    ) -> TextDocumentCompletionResult;

    /**
     * Request to resolve additional information for a given completion item.The request's
     * parameter is of type {@link CompletionItem} the response
     * is of type {@link CompletionItem} or a Thenable that resolves to such.
     */
    auto handleCompletionItemResolve(
      CompletionItem &params
    ) -> CompletionItemResolveResult;

    /**
     * Request to request hover information at a given text document position. The request's
     * parameter is of type {@link TextDocumentPosition} the response is of
     * type {@link Hover} or a Thenable that resolves to such.
     */
    auto handleTextDocumentHover(
      HoverParams &params
    ) -> TextDocumentHoverResult;

    auto handleTextDocumentSignatureHelp(
      SignatureHelpParams &params
    ) -> TextDocumentSignatureHelpResult;

    /**
     * A request to resolve the definition location of a symbol at a given text
     * document position. The request's parameter is of type {@link TextDocumentPosition}
     * the response is of either type {@link Definition} or a typed array of
     * {@link DefinitionLink} or a Thenable that resolves to such.
     */
    auto handleTextDocumentDefinition(
      DefinitionParams &params
    ) -> TextDocumentDefinitionResult;

    /**
     * A request to resolve project-wide references for the symbol denoted
     * by the given text document position. The request's parameter is of
     * type {@link ReferenceParams} the response is of type
     * {@link Location Location[]} or a Thenable that resolves to such.
     */
    auto handleTextDocumentReferences(
      ReferenceParams &params
    ) -> TextDocumentReferencesResult;

    /**
     * Request to resolve a {@link DocumentHighlight} for a given
     * text document position. The request's parameter is of type {@link TextDocumentPosition}
     * the request response is an array of type {@link DocumentHighlight}
     * or a Thenable that resolves to such.
     */
    auto handleTextDocumentDocumentHighlight(
      DocumentHighlightParams &params
    ) -> TextDocumentDocumentHighlightResult;

    /**
     * A request to list all symbols found in a given text document. The request's
     * parameter is of type {@link TextDocumentIdentifier} the
     * response is of type {@link SymbolInformation SymbolInformation[]} or a Thenable
     * that resolves to such.
     */
    auto handleTextDocumentDocumentSymbol(
      DocumentSymbolParams &params
    ) -> TextDocumentDocumentSymbolResult;

    /**
     * A request to provide commands for the given text document and range.
     */
    auto handleTextDocumentCodeAction(
      CodeActionParams &params
    ) -> TextDocumentCodeActionResult;

    /**
     * Request to resolve additional information for a given code action.The request's
     * parameter is of type {@link CodeAction} the response
     * is of type {@link CodeAction} or a Thenable that resolves to such.
     */
    auto handleCodeActionResolve(
      CodeAction &params
    ) -> CodeActionResolveResult;

    /**
     * A request to list project-wide symbols matching the query string given
     * by the {@link WorkspaceSymbolParams}. The response is
     * of type {@link SymbolInformation SymbolInformation[]} or a Thenable that
     * resolves to such.
     *
     * @since 3.17.0 - support for WorkspaceSymbol in the returned data. Clients
     *  need to advertise support for WorkspaceSymbols via the client capability
     *  `workspace.symbol.resolveSupport`.
     *
     */
    auto handleWorkspaceSymbol(
      WorkspaceSymbolParams &params
    ) -> WorkspaceSymbolResult;

    /**
     * A request to resolve the range inside the workspace
     * symbol's location.
     *
     * @since 3.17.0
     */
    auto handleWorkspaceSymbolResolve(
      WorkspaceSymbol &params
    ) -> WorkspaceSymbolResolveResult;

    /**
     * A request to provide code lens for the given text document.
     */
    auto handleTextDocumentCodeLens(
      CodeLensParams &params
    ) -> TextDocumentCodeLensResult;

    /**
     * A request to resolve a command for a given code lens.
     */
    auto handleCodeLensResolve(
      CodeLens &params
    ) -> CodeLensResolveResult;

    /**
     * A request to provide document links
     */
    auto handleTextDocumentDocumentLink(
      DocumentLinkParams &params
    ) -> TextDocumentDocumentLinkResult;

    /**
     * Request to resolve additional information for a given document link. The request's
     * parameter is of type {@link DocumentLink} the response
     * is of type {@link DocumentLink} or a Thenable that resolves to such.
     */
    auto handleDocumentLinkResolve(
      DocumentLink &params
    ) -> DocumentLinkResolveResult;

    /**
     * A request to format a whole document.
     */
    auto handleTextDocumentFormatting(
      DocumentFormattingParams &params
    ) -> TextDocumentFormattingResult;

    /**
     * A request to format a range in a document.
     */
    auto handleTextDocumentRangeFormatting(
      DocumentRangeFormattingParams &params
    ) -> TextDocumentRangeFormattingResult;

    /**
     * A request to format ranges in a document.
     *
     * @since 3.18.0
     * @proposed
     */
    auto handleTextDocumentRangesFormatting(
      DocumentRangesFormattingParams &params
    ) -> TextDocumentRangesFormattingResult;

    /**
     * A request to format a document on type.
     */
    auto handleTextDocumentOnTypeFormatting(
      DocumentOnTypeFormattingParams &params
    ) -> TextDocumentOnTypeFormattingResult;

    /**
     * A request to rename a symbol.
     */
    auto handleTextDocumentRename(
      RenameParams &params
    ) -> TextDocumentRenameResult;

    /**
     * A request to test and perform the setup necessary for a rename.
     *
     * @since 3.16 - support for default behavior
     */
    auto handleTextDocumentPrepareRename(
      PrepareRenameParams &params
    ) -> TextDocumentPrepareRenameResult;

    /**
     * A request send from the client to the server to execute a command. The request might return
     * a workspace edit which the client will apply to the workspace.
     */
    auto handleWorkspaceExecuteCommand(
      ExecuteCommandParams &params
    ) -> WorkspaceExecuteCommandResult;

    // ====================== //
    // Incoming Notifications //
    // ====================== //

    /**
     * The `workspace/didChangeWorkspaceFolders` notification is sent from the client to the server when the workspace
     * folder configuration changes.
     */
    auto handleWorkspaceDidChangeWorkspaceFolders(
      DidChangeWorkspaceFoldersParams &params
    ) -> void;

    /**
     * The `window/workDoneProgress/cancel` notification is sent from  the client to the server to cancel a progress
     * initiated on the server side.
     */
    auto handleWindowWorkDoneProgressCancel(
      WorkDoneProgressCancelParams &params
    ) -> void;

    /**
     * The did create files notification is sent from the client to the server when
     * files were created from within the client.
     *
     * @since 3.16.0
     */
    auto handleWorkspaceDidCreateFiles(
      CreateFilesParams &params
    ) -> void;

    /**
     * The did rename files notification is sent from the client to the server when
     * files were renamed from within the client.
     *
     * @since 3.16.0
     */
    auto handleWorkspaceDidRenameFiles(
      RenameFilesParams &params
    ) -> void;

    /**
     * The will delete files request is sent from the client to the server before files are actually
     * deleted as long as the deletion is triggered from within the client.
     *
     * @since 3.16.0
     */
    auto handleWorkspaceDidDeleteFiles(
      DeleteFilesParams &params
    ) -> void;

    /**
     * A notification sent when a notebook opens.
     *
     * @since 3.17.0
     */
    auto handleNotebookDocumentDidOpen(
      DidOpenNotebookDocumentParams &params
    ) -> void;

    auto handleNotebookDocumentDidChange(
      DidChangeNotebookDocumentParams &params
    ) -> void;

    /**
     * A notification sent when a notebook document is saved.
     *
     * @since 3.17.0
     */
    auto handleNotebookDocumentDidSave(
      DidSaveNotebookDocumentParams &params
    ) -> void;

    /**
     * A notification sent when a notebook closes.
     *
     * @since 3.17.0
     */
    auto handleNotebookDocumentDidClose(
      DidCloseNotebookDocumentParams &params
    ) -> void;

    /**
     * The initialized notification is sent from the client to the
     * server after the client is fully initialized and the server
     * is allowed to send requests from the server to the client.
     */
    auto handleInitialized(
      InitializedParams &params
    ) -> void;

    /**
     * The exit event is sent from the client to the server to
     * ask the server to exit its process.
     */
    auto handleExit() -> void;

    /**
     * The configuration change notification is sent from the client to the server
     * when the client's configuration has changed. The notification contains
     * the changed configuration as defined by the language client.
     */
    auto handleWorkspaceDidChangeConfiguration(
      DidChangeConfigurationParams &params
    ) -> void;

    /**
     * The document open notification is sent from the client to the server to signal
     * newly opened text documents. The document's truth is now managed by the client
     * and the server must not try to read the document's truth using the document's
     * uri. Open in this sense means it is managed by the client. It doesn't necessarily
     * mean that its content is presented in an editor. An open notification must not
     * be sent more than once without a corresponding close notification send before.
     * This means open and close notification must be balanced and the max open count
     * is one.
     */
    auto handleTextDocumentDidOpen(
      DidOpenTextDocumentParams &params
    ) -> void;

    /**
     * The document change notification is sent from the client to the server to signal
     * changes to a text document.
     */
    auto handleTextDocumentDidChange(
      DidChangeTextDocumentParams &params
    ) -> void;

    /**
     * The document close notification is sent from the client to the server when
     * the document got closed in the client. The document's truth now exists where
     * the document's uri points to (e.g. if the document's uri is a file uri the
     * truth now exists on disk). As with the open notification the close notification
     * is about managing the document's content. Receiving a close notification
     * doesn't mean that the document was open in an editor before. A close
     * notification requires a previous open notification to be sent.
     */
    auto handleTextDocumentDidClose(
      DidCloseTextDocumentParams &params
    ) -> void;

    /**
     * The document save notification is sent from the client to the server when
     * the document got saved in the client.
     */
    auto handleTextDocumentDidSave(
      DidSaveTextDocumentParams &params
    ) -> void;

    /**
     * A document will save notification is sent from the client to the server before
     * the document is actually saved.
     */
    auto handleTextDocumentWillSave(
      WillSaveTextDocumentParams &params
    ) -> void;

    /**
     * The watched files notification is sent from the client to the server when
     * the client detects changes to file watched by the language client.
     */
    auto handleWorkspaceDidChangeWatchedFiles(
      DidChangeWatchedFilesParams &params
    ) -> void;

    auto handleSetTrace(
      SetTraceParams &params
    ) -> void;

    // ================= //
    // Outgoing Requests //
    // ================= //

    /**
     * The `workspace/workspaceFolders` is sent from the server to the client to fetch the open workspace folders.
     */
    auto requestWorkspaceWorkspaceFolders() -> void;

    /**
     * The 'workspace/configuration' request is sent from the server to the client to fetch a certain
     * configuration setting.
     *
     * This pull model replaces the old push model where the client signaled configuration change via an
     * event. If the server still needs to react to configuration changes (since the server caches the
     * result of `workspace/configuration` requests) the server should register for an empty configuration
     * change event and empty the cache if such an event is received.
     */
    auto requestWorkspaceConfiguration(
      ConfigurationParams &params
    ) -> void;

    /**
     * @since 3.18.0
     * @proposed
     */
    auto requestWorkspaceFoldingRangeRefresh() -> void;

    /**
     * The `window/workDoneProgress/create` request is sent from the server to the client to initiate progress
     * reporting from the server.
     */
    auto requestWindowWorkDoneProgressCreate(
      WorkDoneProgressCreateParams &params
    ) -> void;

    /**
     * @since 3.16.0
     */
    auto requestWorkspaceSemanticTokensRefresh() -> void;

    /**
     * A request to show a document. This request might open an
     * external program depending on the value of the URI to open.
     * For example a request to open `https://code.visualstudio.com/`
     * will very likely open the URI in a WEB browser.
     *
     * @since 3.16.0
     */
    auto requestWindowShowDocument(
      ShowDocumentParams &params
    ) -> void;

    /**
     * @since 3.17.0
     */
    auto requestWorkspaceInlineValueRefresh() -> void;

    /**
     * @since 3.17.0
     */
    auto requestWorkspaceInlayHintRefresh() -> void;

    /**
     * The diagnostic refresh request definition.
     *
     * @since 3.17.0
     */
    auto requestWorkspaceDiagnosticRefresh() -> void;

    /**
     * The `client/registerCapability` request is sent from the server to the client to register a new capability
     * handler on the client side.
     */
    auto requestClientRegisterCapability(
      RegistrationParams &params
    ) -> void;

    /**
     * The `client/unregisterCapability` request is sent from the server to the client to unregister a previously registered capability
     * handler on the client side.
     */
    auto requestClientUnregisterCapability(
      UnregistrationParams &params
    ) -> void;

    /**
     * The show message request is sent from the server to the client to show a message
     * and a set of options actions to the user.
     */
    auto requestWindowShowMessageRequest(
      ShowMessageRequestParams &params
    ) -> void;

    /**
     * A request to refresh all code actions
     *
     * @since 3.16.0
     */
    auto requestWorkspaceCodeLensRefresh() -> void;

    /**
     * A request sent from the server to the client to modified certain resources.
     */
    auto requestWorkspaceApplyEdit(
      ApplyWorkspaceEditParams &params
    ) -> void;

    // ====================== //
    // Outgoing Notifications //
    // ====================== //

    /**
     * The show message notification is sent from a server to a client to ask
     * the client to display a particular message in the user interface.
     */
    auto notifyWindowShowMessage(
      ShowMessageParams &params
    ) -> void;

    /**
     * The log message notification is sent from the server to the client to ask
     * the client to log a particular message.
     */
    auto notifyWindowLogMessage(
      LogMessageParams &params
    ) -> void;

    /**
     * The telemetry event notification is sent from the server to the client to ask
     * the client to log telemetry data.
     */
    auto notifyTelemetryEvent(
      LSPAny &params
    ) -> void;

    /**
     * Diagnostics notification are sent from the server to the client to signal
     * results of validation runs.
     */
    auto notifyTextDocumentPublishDiagnostics(
      PublishDiagnosticsParams &params
    ) -> void;

    auto notifyLogTrace(
      LogTraceParams &params
    ) -> void;

  }; // class LspLanguageServer

} // namespace LCompilers::LanguageServerProtocol
