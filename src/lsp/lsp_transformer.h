#ifndef LCOMPILERS_LSP_TRANSFORMER_H
#define LCOMPILERS_LSP_TRANSFORMER_H

#include <cstddef>
#include <memory>

#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  class LspTransformer {
  public:

    template <typename T>
    auto lspToAny(
      const T &lsp
    ) const -> std::unique_ptr<LSPAny> {
      std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
      any->value = lspToObject(lsp);
      return any;
    }

    template <typename T>
    auto lspToAny(
      const std::vector<T> &lsps
    ) const -> std::unique_ptr<LSPAny> {
      std::unique_ptr<LSPArray> array = std::make_unique<LSPArray>();
      for (const T &lsp : lsps) {
        array->push_back(lspToAny(lsp));
      }
      return arrayToAny(array);
    }

    template <typename T>
    auto lspToAny(
      const std::vector<std::unique_ptr<T>> &lsps
    ) const -> std::unique_ptr<LSPAny> {
      std::unique_ptr<LSPArray> array = std::make_unique<LSPArray>();
      for (const std::unique_ptr<T> &lsp : lsps) {
        array->push_back(lspToAny(*lsp));
      }
      return arrayToAny(array);
    }

    auto asInitializeParams(
      const MessageParams &requestParams
    ) const -> InitializeParams;
    auto asWillSaveTextDocumentParams(
      const MessageParams &requestParams
    ) const -> WillSaveTextDocumentParams;
    auto asDeclarationParams(
      const MessageParams &requestParams
    ) const -> DeclarationParams;
    auto asDefinitionParams(
      const MessageParams &requestParams
    ) const -> DefinitionParams;
    auto asTypeDefinitionParams(
      const MessageParams &requestParams
    ) const -> TypeDefinitionParams;
    auto asImplementationParams(
      const MessageParams &requestParams
    ) const -> ImplementationParams;
    auto asReferenceParams(
      const MessageParams &requestParams
    ) const -> ReferenceParams;
    auto asCallHierarchyPrepareParams(
      const MessageParams &requestParams
    ) const -> CallHierarchyPrepareParams;
    auto asCallHierarchyIncomingCallsParams(
      const MessageParams &requestParams
    ) const -> CallHierarchyIncomingCallsParams;
    auto asCallHierarchyOutgoingCallsParams(
      const MessageParams &requestParams
    ) const -> CallHierarchyOutgoingCallsParams;
    auto asTypeHierarchyPrepareParams(
      const MessageParams &requestParams
    ) const -> TypeHierarchyPrepareParams;
    auto asTypeHierarchySupertypesParams(
      const MessageParams &requestParams
    ) const -> TypeHierarchySupertypesParams;
    auto asTypeHierarchySubtypesParams(
      const MessageParams &requestParams
    ) const -> TypeHierarchySubtypesParams;
    auto asDocumentHighlightParams(
      const MessageParams &requestParams
    ) const -> DocumentHighlightParams;
    auto asDocumentLinkParams(
      const MessageParams &requestParams
    ) const -> DocumentLinkParams;
    auto asDocumentLink(
      const MessageParams &requestParams
    ) const -> DocumentLink;
    auto asHoverParams(
      const MessageParams &requestParams
    ) const -> HoverParams;
    auto asCodeLensParams(
      const MessageParams &requestParams
    ) const -> CodeLensParams;
    auto asCodeLens(
      const MessageParams &requestParams
    ) const -> CodeLens;
    auto asFoldingRangeParams(
      const MessageParams &requestParams
    ) const -> FoldingRangeParams;
    auto asSelectionRangeParams(
      const MessageParams &requestParams
    ) const -> SelectionRangeParams;
    auto asDocumentSymbolParams(
      const MessageParams &requestParams
    ) const -> DocumentSymbolParams;
    auto asSemanticTokensParams(
      const MessageParams &requestParams
    ) const -> SemanticTokensParams;

    auto asCancelParams(
      const MessageParams &notificationParams
    ) const -> CancelParams;
    auto asSetTraceParams(
      const MessageParams &notificationParams
    ) const -> SetTraceParams;
    auto asInitializedParams(
      const std::optional<MessageParams> &notificationParams
    ) const -> InitializedParams;
    auto asDidOpenTextDocumentParams(
      const MessageParams &notificationParams
    ) const -> DidOpenTextDocumentParams;
    auto asDidChangeTextDocumentParams(
      const MessageParams &notificationParams
    ) const -> DidChangeTextDocumentParams;
    auto asDidSaveTextDocumentParams(
      const MessageParams &notificationParams
    ) const -> DidSaveTextDocumentParams;
    auto asDidCloseTextDocumentParams(
      const MessageParams &notificationParams
    ) const -> DidCloseTextDocumentParams;
    auto asDidOpenNotebookDocumentParams(
      const MessageParams &notificationParams
    ) const -> DidOpenNotebookDocumentParams;
    auto asDidChangeNotebookDocumentParams(
      const MessageParams &notificationParams
    ) const -> DidChangeNotebookDocumentParams;
    auto asDidSaveNotebookDocumentParams(
      const MessageParams &notificationParams
    ) const -> DidSaveNotebookDocumentParams;
    auto asDidCloseNotebookDocumentParams(
      const MessageParams &notificationParams
    ) const -> DidCloseNotebookDocumentParams;

    auto asMessageParams(
      const RegistrationParams &registrationParams
    ) const -> MessageParams;
    auto asMessageParams(
      const UnregistrationParams &unregistrationParams
    ) const -> MessageParams;
    auto asMessageParams(
      const ProgressParams &progressParams
    ) const -> MessageParams;
    auto asMessageParams(
      const LogTraceParams &logTraceParams
    ) const -> MessageParams;

    auto anyToTypeHierarchyItem(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchyItem>;
    auto anyToCallHierarchyItem(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyItem>;
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
    ) const -> OptionalFullCapabilities;
    auto anyToRangeCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<RangeCapabilities>;
    auto anyToOptionalRangeCapabilities(
      const LSPAny &any
    ) const -> OptionalRangeCapabilities;
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
    ) const -> ProgressToken;
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
    ) const -> RequestId;
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
    ) const -> TextDocumentContentChangeEvent;
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
    auto anyToCommand(
      const LSPAny &any
    ) const -> std::unique_ptr<Command>;

    auto arrayToAny(
      std::unique_ptr<LSPArray> &array
    ) const -> std::unique_ptr<LSPAny>;
    auto nullToAny(
      std::nullptr_t null
    ) const -> std::unique_ptr<LSPAny>;
    auto intToAny(
      int value
    ) const -> std::unique_ptr<LSPAny>;
    auto unsignedIntToAny(
      uinteger value
    ) const -> std::unique_ptr<LSPAny>;
    auto unsignedIntToAny(
      const std::vector<uinteger> &values
    ) const -> std::unique_ptr<LSPAny>;
    auto stringToAny(
      const string &value
    ) const -> std::unique_ptr<LSPAny>;
    auto boolToAny(
      bool value
    ) const -> std::unique_ptr<LSPAny>;

    auto unsignedIntToArray(
      const std::vector<uinteger> &values
    ) const -> std::unique_ptr<LSPArray>;

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
      const InitializeResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const WillSaveWaitUntilResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const GotoResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const ServerCapabilities &capabilities
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const TextDocumentSync &textDocumentSync
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const CallHierarchyIncomingCallsResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const CallHierarchyOutgoingCallsResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const TypeHierarchyResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const SymbolKind &kind
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const SymbolTag &tag
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const DocumentHighlightResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      DocumentHighlightKind kind
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const DocumentLinkResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const HoverResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const HoverContents &contents
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const MarkedString &markedString
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const CodeLensResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const FoldingRangeResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      FoldingRangeKind kind
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const SelectionRangeResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const DocumentSymbolResult &result
    ) const -> std::unique_ptr<LSPAny>;
    auto lspToAny(
      const SemanticTokensResult &result
    ) const -> std::unique_ptr<LSPAny>;

    auto lspToObject(
      const SemanticTokens &tokens
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const SemanticTokensPartialResult &tokens
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const SymbolInformation &info
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const DocumentSymbol &symbol
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const SelectionRange &selectionRange
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const FoldingRange &foldingRange
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const Command &command
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const CodeLens &codeLens
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const MarkupContent &markup
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const MarkedStringWithLanguage &markedString
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const Hover &hover
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const DocumentLink &link
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const DocumentHighlight &highlight
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const TypeHierarchyItem &item
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const CallHierarchyOutgoingCall &call
    ) const -> std::unique_ptr<LSPObject>;
    auto lspToObject(
      const CallHierarchyIncomingCall &call
    ) const -> std::unique_ptr<LSPObject>;
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
      const MessageParams &params,
      MessageParamsType type
    ) const -> void;

    auto assertNotificationType(
      const std::string &method,
      const MessageParams &params,
      MessageParamsType type
    ) const -> void;

    auto requireMessageParams(
      const RequestMessage &request
    ) const -> const MessageParams &;

    auto requireMessageParams(
      const NotificationMessage &notification
    ) const -> const MessageParams &;

    auto copy(
      const std::unique_ptr<LSPAny> &any
    ) const -> std::unique_ptr<LSPAny>;
    auto copy(
      const std::unique_ptr<LSPObject> &object
    ) const -> std::unique_ptr<LSPObject>;
    auto copy(
      const std::unique_ptr<LSPArray> &array
    ) const -> std::unique_ptr<LSPArray>;

    auto copyToAny(
      const std::vector<std::unique_ptr<LSPAny>> &source
    ) const -> std::unique_ptr<LSPAny>;
  };

} // namespace LCompilers::LanguageServerProtocol
#endif // LCOMPILERS_LSP_TRANSFORMER_H
