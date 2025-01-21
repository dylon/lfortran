#ifndef LCOMPILERS_LSP_TRANSFORMER_H
#define LCOMPILERS_LSP_TRANSFORMER_H

#include <memory>

#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  class LspTransformer {
  public:
    auto asInitializeParams(
      const RequestParams &requestParams
    ) const -> InitializeParams;
    auto asWillSaveTextDocumentParams(
      const RequestParams &requestParams
    ) const -> WillSaveTextDocumentParams;
    auto asDeclarationParams(
      const RequestParams &requestParams
    ) const -> DeclarationParams;
    auto asDefinitionParams(
      const RequestParams &requestParams
    ) const -> DefinitionParams;
    auto asTypeDefinitionParams(
      const RequestParams &requestParams
    ) const -> TypeDefinitionParams;
    auto asImplementationParams(
      const RequestParams &requestParams
    ) const -> ImplementationParams;
    auto asReferenceParams(
      const RequestParams &requestParams
    ) const -> ReferenceParams;
    auto asCallHierarchyPrepareParams(
      const RequestParams &requestParams
    ) const -> CallHierarchyPrepareParams;

    auto asCancelParams(
      const NotificationParams &notificationParams
    ) const -> CancelParams;
    auto asSetTraceParams(
      const NotificationParams &notificationParams
    ) const -> SetTraceParams;
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
    auto asDidOpenNotebookDocumentParams(
      const NotificationParams &notificationParams
    ) const -> DidOpenNotebookDocumentParams;
    auto asDidChangeNotebookDocumentParams(
      const NotificationParams &notificationParams
    ) const -> DidChangeNotebookDocumentParams;
    auto asDidSaveNotebookDocumentParams(
      const NotificationParams &notificationParams
    ) const -> DidSaveNotebookDocumentParams;
    auto asDidCloseNotebookDocumentParams(
      const NotificationParams &notificationParams
    ) const -> DidCloseNotebookDocumentParams;

    auto asRequestParams(
      const RegistrationParams &registrationParams
    ) const -> std::unique_ptr<RequestParams>;
    auto asRequestParams(
      const UnregistrationParams &unregistrationParams
    ) const -> std::unique_ptr<RequestParams>;

    auto asNotificationParams(
      const ProgressParams &progressParams
    ) const -> std::unique_ptr<NotificationParams>;
    auto asNotificationParams(
      const LogTraceParams &logTraceParams
    ) const -> std::unique_ptr<NotificationParams>;

    auto anyToReferenceContext(
      const LSPAny &any
    ) const -> std::unique_ptr<ReferenceContext>;
    auto anyToPositionEncodingKind(
      const LSPAny &any
    ) const -> PositionEncodingKind;
    auto anyToMarkdownClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<MarkdownClientCapabilities>;
    auto anyToRegularExpressionsClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<RegularExpressionsClientCapabilities>;
    auto anyToStaleRequestSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<StaleRequestSupport>;
    auto anyToGeneralCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<GeneralCapabilities>;
    auto anyToShowDocumentClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowDocumentClientCapabilities>;
    auto anyToMessageActionItemCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<MessageActionItemCapabilities>;
    auto anyToShowMessageRequestClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowMessageRequestClientCapabilities>;
    auto anyToWindowCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WindowCapabilities>;
    auto anyToNotebookDocumentSyncClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentSyncClientCapabilities>;
    auto anyToNotebookDocumentClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentClientCapabilities>;
    auto anyToDiagnosticClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DiagnosticClientCapabilities>;
    auto anyToInlayHintClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintClientCapabilities>;
    auto anyToInlineValueClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueClientCapabilities>;
    auto anyToTypeHierarchyClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchyClientCapabilities>;
    auto anyToMonikerClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<MonikerClientCapabilities>;
    auto anyToTokenFormat(
      const LSPAny &any
    ) const -> TokenFormat;
    auto anyToFullCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<FullCapabilities>;
    auto anyToOptionalFullCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<OptionalFullCapabilities>;
    auto anyToRangeCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<RangeCapabilities>;
    auto anyToOptionalRangeCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<OptionalRangeCapabilities>;
    auto anyToSemanticTokensClientRequestCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensClientRequestCapabilities>;
    auto anyToSemanticTokensClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensClientCapabilities>;
    auto anyToCallHierarchyClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyClientCapabilities>;
    auto anyToLinkedEditingRangeClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<LinkedEditingRangeClientCapabilities>;
    auto anyToSelectionRangeClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SelectionRangeClientCapabilities>;
    auto anyToFoldingRangeCollapsedText(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeCollapsedText>;
    auto anyToFoldingRangeKind(
      const LSPAny &any
    ) const -> FoldingRangeKind;
    auto anyToOptionalFoldingRangeKindValueSet(
      const LSPAny &any
    ) const -> std::unique_ptr<OptionalValueSet<FoldingRangeKind>>;
    auto anyToFoldingRangeClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeClientCapabilities>;
    auto anyToDiagnosticTag(
      const LSPAny &any
    ) const -> DiagnosticTag;
    auto anyToDiagnosticTagValueSet(
      const LSPAny &any
    ) const -> std::unique_ptr<ValueSet<DiagnosticTag>>;
    auto anyToPublishDiagnosticsClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<PublishDiagnosticsClientCapabilities>;
    auto anyToPrepareSupportDefaultBehavior(
      const LSPAny &any
    ) const -> PrepareSupportDefaultBehavior;
    auto anyToRenameClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<RenameClientCapabilities>;
    auto anyToDocumentOnTypeFormattingClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentOnTypeFormattingClientCapabilities>;
    auto anyToDocumentFormattingClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentFormattingClientCapabilities>;
    auto anyToDocumentRangeFormattingClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentRangeFormattingClientCapabilities>;
    auto anyToDocumentColorClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentColorClientCapabilities>;
    auto anyToDocumentLinkClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentLinkClientCapabilities>;
    auto anyToCodeLensClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeLensClientCapabilities>;
    auto anyToResolveSupportCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ResolveSupportCapabilities>;
    auto anyToCodeActionKind(
      const LSPAny &any
    ) const -> CodeActionKind;
    auto anyToCodeActionKindValueSet(
      const LSPAny &any
    ) const -> std::unique_ptr<ValueSet<CodeActionKind>>;
    auto anyToCodeActionLiteralSupportCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionLiteralSupportCapabilities>;
    auto anyToCodeActionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionClientCapabilities>;
    auto anyToDocumentSymbolClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentSymbolClientCapabilities>;
    auto anyToDocumentHighlightClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentHighlightClientCapabilities>;
    auto anyToReferenceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ReferenceClientCapabilities>;
    auto anyToImplementationClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ImplementationClientCapabilities>;
    auto anyToTypeDefinitionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeDefinitionClientCapabilities>;
    auto anyToDefinitionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DefinitionClientCapabilities>;
    auto anyToDeclarationClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DeclarationClientCapabilities>;
    auto anyToParameterInformationCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ParameterInformationCapabilities>;
    auto anyToSignatureInformationCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureInformationCapabilities>;
    auto anyToSignatureHelpClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelpClientCapabilities>;
    auto anyToHoverClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<HoverClientCapabilities>;
    auto anyToCompletionListCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionListCapabilities>;
    auto anyToCompletionItemKind(
      const LSPAny &any
    ) const -> CompletionItemKind;
    auto anyToOptionalCompletionItemKindValueSet(
      const LSPAny &any
    ) const -> std::unique_ptr<OptionalValueSet<CompletionItemKind>>;
    auto anyToInsertTextMode(
      const LSPAny &any
    ) const -> InsertTextMode;
    auto anyToInsertTextModeValueSet(
      const LSPAny &any
    ) const -> std::unique_ptr<ValueSet<InsertTextMode>>;
    auto anyToCompletionItemTag(
      const LSPAny &any
    ) const -> CompletionItemTag;
    auto anyToCompletionItemTagValueSet(
      const LSPAny &any
    ) const -> std::unique_ptr<ValueSet<CompletionItemTag>>;
    auto anyToMarkupKind(
      const LSPAny &any
    ) const -> MarkupKind;
    auto anyToCompletionItemCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionItemCapabilities>;
    auto anyToCompletionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionClientCapabilities>;
    auto anyToTextDocumentSyncClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentSyncClientCapabilities>;
    auto anyToTextDocumentClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentClientCapabilities>;
    auto anyToDiagnosticWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DiagnosticWorkspaceClientCapabilities>;
    auto anyToInlayHintWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintWorkspaceClientCapabilities>;
    auto anyToInlineValueWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueWorkspaceClientCapabilities>;
    auto anyToFileCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<FileCapabilities>;
    auto anyToCodeLensWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeLensWorkspaceClientCapabilities>;
    auto anyToSemanticTokensWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensWorkspaceClientCapabilities>;
    auto anyToExecuteCommandClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ExecuteCommandClientCapabilities>;
    auto anyToSymbolTag(
      const LSPAny &any
    ) const -> SymbolTag;
    auto anyToSymbolTagValueSet(
      const LSPAny &any
    ) const -> std::unique_ptr<ValueSet<SymbolTag>>;
    auto anyToSymbolKind(
      const LSPAny &any
    ) const -> SymbolKind;
    auto anyToOptionalSymbolKindValueSet(
      const LSPAny &any
    ) const -> std::unique_ptr<OptionalValueSet<SymbolKind>>;
    auto anyToWorkspaceSymbolClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbolClientCapabilities>;
    auto anyToDidChangeWatchedFilesClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeWatchedFilesClientCapabilities>;
    auto anyToDidChangeConfigurationClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeConfigurationClientCapabilities>;
    auto anyToChangeAnnotationSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<ChangeAnnotationSupport>;
    auto anyToFailureHandlingKind(
      const LSPAny &any
    ) const -> FailureHandlingKind;
    auto anyToResourceOperationKind(
      const LSPAny &any
    ) const -> ResourceOperationKind;
    auto anyToWorkspaceEditClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceEditClientCapabilities>;
    auto anyToWorkspaceCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceCapabilities>;
    auto anyToWorkspaceFolder(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceFolder>;
    auto anyToClientInfo(
      const LSPAny &any
    ) const -> std::unique_ptr<ClientInfo>;
    auto anyToProgressToken(
      const LSPAny &any
    ) const -> std::unique_ptr<ProgressToken>;
    auto anyToNotebookCellArrayChange(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookCellArrayChange>;
    auto anyToNotebookDocumentChangeEventCellsStructure(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentChangeEventCellsStructure>;
    auto anyToNotebookDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentIdentifier>;
    auto anyToNotebookDocumentChangeEventCellsTextContent(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentChangeEventCellsTextContent>;
    auto anyToNotebookDocumentChangeEventCells(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentChangeEventCells>;
    auto anyToNotebookDocumentChangeEvent(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentChangeEvent>;
    auto anyToVersionedNotebookDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<VersionedNotebookDocumentIdentifier>;
    auto anyToExecutionSummary(
      const LSPAny &any
    ) const -> std::unique_ptr<ExecutionSummary>;
    auto anyToNotebookCell(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookCell>;
    auto anyToNotebookDocument(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocument>;
    auto anyToTextDocumentSaveReason(
      const LSPAny &any
    ) const -> TextDocumentSaveReason;
    auto anyToRequestId(
      const LSPAny &any
    ) const -> std::unique_ptr<RequestId>;
    auto anyToTraceValue(
      const LSPAny &any
    ) const -> TraceValue;
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
    auto anyToRange(
      const LSPAny &any
    ) const -> std::unique_ptr<Range>;
    auto anyToPosition(
      const LSPAny &any
    ) const -> std::unique_ptr<Position>;
    auto anyToString(
      const LSPAny &any
    ) const -> const std::string &;
    auto anyToInt(
      const LSPAny &any
    ) const -> int;
    auto anyToUnsignedInt(
      const LSPAny &any
    ) const -> unsigned int;
    auto anyToBool(
      const LSPAny &any
    ) const -> bool;

    auto lspToAny(
      const CallHierarchyItem &item
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const PrepareCallHierarchyResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const FindReferencesResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const ProgressToken &token
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const ProgressParams &params
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const InitializeResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const WillSaveWaitUntilResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const GotoDeclarationResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const GotoDefinitionResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const GotoTypeDefinitionResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const GotoImplementationResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const ServerCapabilities &capabilities
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const TextDocumentSync &textDocumentSync
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const TextEdit &edit
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const LocationLink &link
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const Location &location
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const Range &range
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const Position &position
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const Registration &registration
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const Unregistration &unregistration
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const LogTraceParams &logTraceParams
    ) const -> std::unique_ptr<LSPAny>;

    auto intToAny(
      int value
    ) const -> std::unique_ptr<LSPAny>;
    auto unsignedIntToAny(
      uinteger value
    ) const -> std::unique_ptr<LSPAny>;
    auto stringToAny(
      const string &value
    ) const -> std::unique_ptr<LSPAny>;

    auto lspToObject(
      const CallHierarchyItem &item
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const LogTraceParams &logTraceParams
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const Unregistration &unregistration
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const UnregistrationParams &unregistrationParams
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const Registration &registration
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const RegistrationParams &registrationParams
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const ProgressParams &params
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const TextEdit &edit
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const LocationLink &link
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const Location &location
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const Range &range
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const Position &position
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const TextDocumentSyncOptions &options
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const SaveOptions &options
    ) const -> std::unique_ptr<LSPObject>;

    auto assertAnyType(
      const std::string &name,
      const LSPAny &any,
      LSPAnyType type
    ) const -> void;

    auto assertRequestType(
      const std::string &method,
      const RequestParams &params,
      RequestParamsType type
    ) const -> void;

    auto assertNotificationType(
      const std::string &method,
      const NotificationParams &params,
      NotificationParamsType type
    ) const -> void;

    auto requireRequestParams(
      const RequestMessage &request
    ) const -> const RequestParams &;

    auto requireNotificationParams(
      const NotificationMessage &notification
    ) const -> const NotificationParams &;

    auto copy(
      const std::unique_ptr<LSPAny> &any
    ) const -> std::unique_ptr<LSPAny>;

    auto copy(
      const std::unique_ptr<LSPObject> &object
    ) const -> std::unique_ptr<LSPObject>;

    auto copy(
      const std::unique_ptr<LSPArray> &array
    ) const -> std::unique_ptr<LSPArray>;
  };

} // namespace LCompilers::LanguageServerProtocol

#endif // LCOMPILERS_LSP_TRANSFORMER_H
