// -----------------------------------------------------------------------------
// NOTE: This file was generated from Microsoft's Language Server Protocol (LSP)
// specification. Please do not edit it by hand.
// -----------------------------------------------------------------------------

#pragma once

#include <cstddef>
#include <memory>

#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  class LspTransformer {
  public:
    // ============ //
    // Copy Methods //
    // ============ //

    auto copy(const std::unique_ptr<LSPAny> &any) const -> std::unique_ptr<LSPAny>;
    auto copy(const LSPObject &object) const -> LSPObject;
    auto copy(const LSPArray &array) const -> LSPArray;

    // ===================================== //
    // LSPAny <-> LSP Enumeration Transforms //
    // ===================================== //

    auto anyToSemanticTokenTypes(
      const LSPAny &any
    ) const -> SemanticTokenTypes;
    auto semanticTokenTypesToAny(
      SemanticTokenTypes enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokenModifiers(
      const LSPAny &any
    ) const -> SemanticTokenModifiers;
    auto semanticTokenModifiersToAny(
      SemanticTokenModifiers enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentDiagnosticReportKind(
      const LSPAny &any
    ) const -> DocumentDiagnosticReportKind;
    auto documentDiagnosticReportKindToAny(
      DocumentDiagnosticReportKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToErrorCodes(
      const LSPAny &any
    ) const -> ErrorCodes;
    auto errorCodesToAny(
      ErrorCodes enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLSPErrorCodes(
      const LSPAny &any
    ) const -> LSPErrorCodes;
    auto lspErrorCodesToAny(
      LSPErrorCodes enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRangeKind(
      const LSPAny &any
    ) const -> FoldingRangeKind;
    auto foldingRangeKindToAny(
      FoldingRangeKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSymbolKind(
      const LSPAny &any
    ) const -> SymbolKind;
    auto symbolKindToAny(
      SymbolKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSymbolTag(
      const LSPAny &any
    ) const -> SymbolTag;
    auto symbolTagToAny(
      SymbolTag enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToUniquenessLevel(
      const LSPAny &any
    ) const -> UniquenessLevel;
    auto uniquenessLevelToAny(
      UniquenessLevel enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMonikerKind(
      const LSPAny &any
    ) const -> MonikerKind;
    auto monikerKindToAny(
      MonikerKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintKind(
      const LSPAny &any
    ) const -> InlayHintKind;
    auto inlayHintKindToAny(
      InlayHintKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMessageType(
      const LSPAny &any
    ) const -> MessageType;
    auto messageTypeToAny(
      MessageType enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentSyncKind(
      const LSPAny &any
    ) const -> TextDocumentSyncKind;
    auto textDocumentSyncKindToAny(
      TextDocumentSyncKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentSaveReason(
      const LSPAny &any
    ) const -> TextDocumentSaveReason;
    auto textDocumentSaveReasonToAny(
      TextDocumentSaveReason enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionItemKind(
      const LSPAny &any
    ) const -> CompletionItemKind;
    auto completionItemKindToAny(
      CompletionItemKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionItemTag(
      const LSPAny &any
    ) const -> CompletionItemTag;
    auto completionItemTagToAny(
      CompletionItemTag enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInsertTextFormat(
      const LSPAny &any
    ) const -> InsertTextFormat;
    auto insertTextFormatToAny(
      InsertTextFormat enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInsertTextMode(
      const LSPAny &any
    ) const -> InsertTextMode;
    auto insertTextModeToAny(
      InsertTextMode enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentHighlightKind(
      const LSPAny &any
    ) const -> DocumentHighlightKind;
    auto documentHighlightKindToAny(
      DocumentHighlightKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionKind(
      const LSPAny &any
    ) const -> CodeActionKind;
    auto codeActionKindToAny(
      CodeActionKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTraceValues(
      const LSPAny &any
    ) const -> TraceValues;
    auto traceValuesToAny(
      TraceValues enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMarkupKind(
      const LSPAny &any
    ) const -> MarkupKind;
    auto markupKindToAny(
      MarkupKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionTriggerKind(
      const LSPAny &any
    ) const -> InlineCompletionTriggerKind;
    auto inlineCompletionTriggerKindToAny(
      InlineCompletionTriggerKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPositionEncodingKind(
      const LSPAny &any
    ) const -> PositionEncodingKind;
    auto positionEncodingKindToAny(
      PositionEncodingKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileChangeType(
      const LSPAny &any
    ) const -> FileChangeType;
    auto fileChangeTypeToAny(
      FileChangeType enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWatchKind(
      const LSPAny &any
    ) const -> WatchKind;
    auto watchKindToAny(
      WatchKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnosticSeverity(
      const LSPAny &any
    ) const -> DiagnosticSeverity;
    auto diagnosticSeverityToAny(
      DiagnosticSeverity enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnosticTag(
      const LSPAny &any
    ) const -> DiagnosticTag;
    auto diagnosticTagToAny(
      DiagnosticTag enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionTriggerKind(
      const LSPAny &any
    ) const -> CompletionTriggerKind;
    auto completionTriggerKindToAny(
      CompletionTriggerKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelpTriggerKind(
      const LSPAny &any
    ) const -> SignatureHelpTriggerKind;
    auto signatureHelpTriggerKindToAny(
      SignatureHelpTriggerKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionTriggerKind(
      const LSPAny &any
    ) const -> CodeActionTriggerKind;
    auto codeActionTriggerKindToAny(
      CodeActionTriggerKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileOperationPatternKind(
      const LSPAny &any
    ) const -> FileOperationPatternKind;
    auto fileOperationPatternKindToAny(
      FileOperationPatternKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookCellKind(
      const LSPAny &any
    ) const -> NotebookCellKind;
    auto notebookCellKindToAny(
      NotebookCellKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToResourceOperationKind(
      const LSPAny &any
    ) const -> ResourceOperationKind;
    auto resourceOperationKindToAny(
      ResourceOperationKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFailureHandlingKind(
      const LSPAny &any
    ) const -> FailureHandlingKind;
    auto failureHandlingKindToAny(
      FailureHandlingKind enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPrepareSupportDefaultBehavior(
      const LSPAny &any
    ) const -> PrepareSupportDefaultBehavior;
    auto prepareSupportDefaultBehaviorToAny(
      PrepareSupportDefaultBehavior enumerator
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTokenFormat(
      const LSPAny &any
    ) const -> TokenFormat;
    auto tokenFormatToAny(
      TokenFormat enumerator
    ) const -> std::unique_ptr<LSPAny>;

    // =================================== //
    // LSPAny <-> LSP Structure Transforms //
    // =================================== //

    auto anyToMessage(
      const LSPAny &any
    ) const -> std::unique_ptr<Message>;
    auto messageToAny(
      const Message &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRequestMessage(
      const LSPAny &any
    ) const -> std::unique_ptr<RequestMessage>;
    auto requestMessageToAny(
      const RequestMessage &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotificationMessage(
      const LSPAny &any
    ) const -> std::unique_ptr<NotificationMessage>;
    auto notificationMessageToAny(
      const NotificationMessage &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToResponseError(
      const LSPAny &any
    ) const -> std::unique_ptr<ResponseError>;
    auto responseErrorToAny(
      const ResponseError &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToResponseMessage(
      const LSPAny &any
    ) const -> std::unique_ptr<ResponseMessage>;
    auto responseMessageToAny(
      const ResponseMessage &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToImplementationParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ImplementationParams>;
    auto implementationParamsToAny(
      const ImplementationParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLocation(
      const LSPAny &any
    ) const -> std::unique_ptr<Location>;
    auto locationToAny(
      const Location &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToImplementationRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<ImplementationRegistrationOptions>;
    auto implementationRegistrationOptionsToAny(
      const ImplementationRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeDefinitionParams(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeDefinitionParams>;
    auto typeDefinitionParamsToAny(
      const TypeDefinitionParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeDefinitionRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeDefinitionRegistrationOptions>;
    auto typeDefinitionRegistrationOptionsToAny(
      const TypeDefinitionRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceFolder(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceFolder>;
    auto workspaceFolderToAny(
      const WorkspaceFolder &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeWorkspaceFoldersParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeWorkspaceFoldersParams>;
    auto didChangeWorkspaceFoldersParamsToAny(
      const DidChangeWorkspaceFoldersParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToConfigurationParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ConfigurationParams>;
    auto configurationParamsToAny(
      const ConfigurationParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentColorParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentColorParams>;
    auto documentColorParamsToAny(
      const DocumentColorParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToColorInformation(
      const LSPAny &any
    ) const -> std::unique_ptr<ColorInformation>;
    auto colorInformationToAny(
      const ColorInformation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentColorRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentColorRegistrationOptions>;
    auto documentColorRegistrationOptionsToAny(
      const DocumentColorRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToColorPresentationParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ColorPresentationParams>;
    auto colorPresentationParamsToAny(
      const ColorPresentationParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToColorPresentation(
      const LSPAny &any
    ) const -> std::unique_ptr<ColorPresentation>;
    auto colorPresentationToAny(
      const ColorPresentation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkDoneProgressOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkDoneProgressOptions>;
    auto workDoneProgressOptionsToAny(
      const WorkDoneProgressOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentRegistrationOptions_documentSelector(
      const LSPAny &any
    ) const -> TextDocumentRegistrationOptions_documentSelector;
    auto textDocumentRegistrationOptions_documentSelectorToAny(
      const TextDocumentRegistrationOptions_documentSelector &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentRegistrationOptions>;
    auto textDocumentRegistrationOptionsToAny(
      const TextDocumentRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRangeParams(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeParams>;
    auto foldingRangeParamsToAny(
      const FoldingRangeParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRange(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRange>;
    auto foldingRangeToAny(
      const FoldingRange &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRangeRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeRegistrationOptions>;
    auto foldingRangeRegistrationOptionsToAny(
      const FoldingRangeRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeclarationParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DeclarationParams>;
    auto declarationParamsToAny(
      const DeclarationParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeclarationRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DeclarationRegistrationOptions>;
    auto declarationRegistrationOptionsToAny(
      const DeclarationRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSelectionRangeParams(
      const LSPAny &any
    ) const -> std::unique_ptr<SelectionRangeParams>;
    auto selectionRangeParamsToAny(
      const SelectionRangeParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSelectionRange(
      const LSPAny &any
    ) const -> std::unique_ptr<SelectionRange>;
    auto selectionRangeToAny(
      const SelectionRange &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSelectionRangeRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<SelectionRangeRegistrationOptions>;
    auto selectionRangeRegistrationOptionsToAny(
      const SelectionRangeRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkDoneProgressCreateParams(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkDoneProgressCreateParams>;
    auto workDoneProgressCreateParamsToAny(
      const WorkDoneProgressCreateParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkDoneProgressCancelParams(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkDoneProgressCancelParams>;
    auto workDoneProgressCancelParamsToAny(
      const WorkDoneProgressCancelParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyPrepareParams(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyPrepareParams>;
    auto callHierarchyPrepareParamsToAny(
      const CallHierarchyPrepareParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyItem(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyItem>;
    auto callHierarchyItemToAny(
      const CallHierarchyItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyRegistrationOptions>;
    auto callHierarchyRegistrationOptionsToAny(
      const CallHierarchyRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyIncomingCallsParams(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyIncomingCallsParams>;
    auto callHierarchyIncomingCallsParamsToAny(
      const CallHierarchyIncomingCallsParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyIncomingCall(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyIncomingCall>;
    auto callHierarchyIncomingCallToAny(
      const CallHierarchyIncomingCall &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyOutgoingCallsParams(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyOutgoingCallsParams>;
    auto callHierarchyOutgoingCallsParamsToAny(
      const CallHierarchyOutgoingCallsParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyOutgoingCall(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyOutgoingCall>;
    auto callHierarchyOutgoingCallToAny(
      const CallHierarchyOutgoingCall &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensParams(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensParams>;
    auto semanticTokensParamsToAny(
      const SemanticTokensParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokens(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokens>;
    auto semanticTokensToAny(
      const SemanticTokens &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensPartialResult(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensPartialResult>;
    auto semanticTokensPartialResultToAny(
      const SemanticTokensPartialResult &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensRegistrationOptions>;
    auto semanticTokensRegistrationOptionsToAny(
      const SemanticTokensRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensDeltaParams(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensDeltaParams>;
    auto semanticTokensDeltaParamsToAny(
      const SemanticTokensDeltaParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensDelta(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensDelta>;
    auto semanticTokensDeltaToAny(
      const SemanticTokensDelta &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensDeltaPartialResult(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensDeltaPartialResult>;
    auto semanticTokensDeltaPartialResultToAny(
      const SemanticTokensDeltaPartialResult &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensRangeParams(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensRangeParams>;
    auto semanticTokensRangeParamsToAny(
      const SemanticTokensRangeParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToShowDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowDocumentParams>;
    auto showDocumentParamsToAny(
      const ShowDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToShowDocumentResult(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowDocumentResult>;
    auto showDocumentResultToAny(
      const ShowDocumentResult &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLinkedEditingRangeParams(
      const LSPAny &any
    ) const -> std::unique_ptr<LinkedEditingRangeParams>;
    auto linkedEditingRangeParamsToAny(
      const LinkedEditingRangeParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLinkedEditingRanges(
      const LSPAny &any
    ) const -> std::unique_ptr<LinkedEditingRanges>;
    auto linkedEditingRangesToAny(
      const LinkedEditingRanges &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLinkedEditingRangeRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<LinkedEditingRangeRegistrationOptions>;
    auto linkedEditingRangeRegistrationOptionsToAny(
      const LinkedEditingRangeRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCreateFilesParams(
      const LSPAny &any
    ) const -> std::unique_ptr<CreateFilesParams>;
    auto createFilesParamsToAny(
      const CreateFilesParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceEdit_documentChanges(
      const LSPAny &any
    ) const -> WorkspaceEdit_documentChanges;
    auto workspaceEdit_documentChangesToAny(
      const WorkspaceEdit_documentChanges &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceEdit(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceEdit>;
    auto workspaceEditToAny(
      const WorkspaceEdit &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileOperationRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<FileOperationRegistrationOptions>;
    auto fileOperationRegistrationOptionsToAny(
      const FileOperationRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRenameFilesParams(
      const LSPAny &any
    ) const -> std::unique_ptr<RenameFilesParams>;
    auto renameFilesParamsToAny(
      const RenameFilesParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeleteFilesParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DeleteFilesParams>;
    auto deleteFilesParamsToAny(
      const DeleteFilesParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMonikerParams(
      const LSPAny &any
    ) const -> std::unique_ptr<MonikerParams>;
    auto monikerParamsToAny(
      const MonikerParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMoniker(
      const LSPAny &any
    ) const -> std::unique_ptr<Moniker>;
    auto monikerToAny(
      const Moniker &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMonikerRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<MonikerRegistrationOptions>;
    auto monikerRegistrationOptionsToAny(
      const MonikerRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeHierarchyPrepareParams(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchyPrepareParams>;
    auto typeHierarchyPrepareParamsToAny(
      const TypeHierarchyPrepareParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeHierarchyItem(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchyItem>;
    auto typeHierarchyItemToAny(
      const TypeHierarchyItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeHierarchyRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchyRegistrationOptions>;
    auto typeHierarchyRegistrationOptionsToAny(
      const TypeHierarchyRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeHierarchySupertypesParams(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchySupertypesParams>;
    auto typeHierarchySupertypesParamsToAny(
      const TypeHierarchySupertypesParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeHierarchySubtypesParams(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchySubtypesParams>;
    auto typeHierarchySubtypesParamsToAny(
      const TypeHierarchySubtypesParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueParams(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueParams>;
    auto inlineValueParamsToAny(
      const InlineValueParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueRegistrationOptions>;
    auto inlineValueRegistrationOptionsToAny(
      const InlineValueRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintParams(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintParams>;
    auto inlayHintParamsToAny(
      const InlayHintParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHint_label(
      const LSPAny &any
    ) const -> InlayHint_label;
    auto inlayHint_labelToAny(
      const InlayHint_label &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHint_tooltip(
      const LSPAny &any
    ) const -> InlayHint_tooltip;
    auto inlayHint_tooltipToAny(
      const InlayHint_tooltip &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHint(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHint>;
    auto inlayHintToAny(
      const InlayHint &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintRegistrationOptions>;
    auto inlayHintRegistrationOptionsToAny(
      const InlayHintRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentDiagnosticParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentDiagnosticParams>;
    auto documentDiagnosticParamsToAny(
      const DocumentDiagnosticParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentDiagnosticReportPartialResult_relatedDocuments(
      const LSPAny &any
    ) const -> DocumentDiagnosticReportPartialResult_relatedDocuments;
    auto documentDiagnosticReportPartialResult_relatedDocumentsToAny(
      const DocumentDiagnosticReportPartialResult_relatedDocuments &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentDiagnosticReportPartialResult(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentDiagnosticReportPartialResult>;
    auto documentDiagnosticReportPartialResultToAny(
      const DocumentDiagnosticReportPartialResult &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnosticServerCancellationData(
      const LSPAny &any
    ) const -> std::unique_ptr<DiagnosticServerCancellationData>;
    auto diagnosticServerCancellationDataToAny(
      const DiagnosticServerCancellationData &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnosticRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DiagnosticRegistrationOptions>;
    auto diagnosticRegistrationOptionsToAny(
      const DiagnosticRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceDiagnosticParams(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceDiagnosticParams>;
    auto workspaceDiagnosticParamsToAny(
      const WorkspaceDiagnosticParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceDiagnosticReport(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceDiagnosticReport>;
    auto workspaceDiagnosticReportToAny(
      const WorkspaceDiagnosticReport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceDiagnosticReportPartialResult(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceDiagnosticReportPartialResult>;
    auto workspaceDiagnosticReportPartialResultToAny(
      const WorkspaceDiagnosticReportPartialResult &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidOpenNotebookDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidOpenNotebookDocumentParams>;
    auto didOpenNotebookDocumentParamsToAny(
      const DidOpenNotebookDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeNotebookDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeNotebookDocumentParams>;
    auto didChangeNotebookDocumentParamsToAny(
      const DidChangeNotebookDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidSaveNotebookDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidSaveNotebookDocumentParams>;
    auto didSaveNotebookDocumentParamsToAny(
      const DidSaveNotebookDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidCloseNotebookDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidCloseNotebookDocumentParams>;
    auto didCloseNotebookDocumentParamsToAny(
      const DidCloseNotebookDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionParams(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineCompletionParams>;
    auto inlineCompletionParamsToAny(
      const InlineCompletionParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionList(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineCompletionList>;
    auto inlineCompletionListToAny(
      const InlineCompletionList &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionItem_insertText(
      const LSPAny &any
    ) const -> InlineCompletionItem_insertText;
    auto inlineCompletionItem_insertTextToAny(
      const InlineCompletionItem_insertText &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionItem(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineCompletionItem>;
    auto inlineCompletionItemToAny(
      const InlineCompletionItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineCompletionRegistrationOptions>;
    auto inlineCompletionRegistrationOptionsToAny(
      const InlineCompletionRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRegistrationParams(
      const LSPAny &any
    ) const -> std::unique_ptr<RegistrationParams>;
    auto registrationParamsToAny(
      const RegistrationParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToUnregistrationParams(
      const LSPAny &any
    ) const -> std::unique_ptr<UnregistrationParams>;
    auto unregistrationParamsToAny(
      const UnregistrationParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInitializeParams(
      const LSPAny &any
    ) const -> std::unique_ptr<InitializeParams>;
    auto initializeParamsToAny(
      const InitializeParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInitializeResult_serverInfo(
      const LSPAny &any
    ) const -> std::unique_ptr<InitializeResult_serverInfo>;
    auto initializeResult_serverInfoToAny(
      const InitializeResult_serverInfo &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInitializeResult(
      const LSPAny &any
    ) const -> std::unique_ptr<InitializeResult>;
    auto initializeResultToAny(
      const InitializeResult &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInitializeError(
      const LSPAny &any
    ) const -> std::unique_ptr<InitializeError>;
    auto initializeErrorToAny(
      const InitializeError &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInitializedParams(
      const LSPAny &any
    ) const -> std::unique_ptr<InitializedParams>;
    auto initializedParamsToAny(
      const InitializedParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeConfigurationParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeConfigurationParams>;
    auto didChangeConfigurationParamsToAny(
      const DidChangeConfigurationParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeConfigurationRegistrationOptions_section(
      const LSPAny &any
    ) const -> DidChangeConfigurationRegistrationOptions_section;
    auto didChangeConfigurationRegistrationOptions_sectionToAny(
      const DidChangeConfigurationRegistrationOptions_section &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeConfigurationRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeConfigurationRegistrationOptions>;
    auto didChangeConfigurationRegistrationOptionsToAny(
      const DidChangeConfigurationRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToShowMessageParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowMessageParams>;
    auto showMessageParamsToAny(
      const ShowMessageParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToShowMessageRequestParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowMessageRequestParams>;
    auto showMessageRequestParamsToAny(
      const ShowMessageRequestParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMessageActionItem(
      const LSPAny &any
    ) const -> std::unique_ptr<MessageActionItem>;
    auto messageActionItemToAny(
      const MessageActionItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLogMessageParams(
      const LSPAny &any
    ) const -> std::unique_ptr<LogMessageParams>;
    auto logMessageParamsToAny(
      const LogMessageParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidOpenTextDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidOpenTextDocumentParams>;
    auto didOpenTextDocumentParamsToAny(
      const DidOpenTextDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeTextDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeTextDocumentParams>;
    auto didChangeTextDocumentParamsToAny(
      const DidChangeTextDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentChangeRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentChangeRegistrationOptions>;
    auto textDocumentChangeRegistrationOptionsToAny(
      const TextDocumentChangeRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidCloseTextDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidCloseTextDocumentParams>;
    auto didCloseTextDocumentParamsToAny(
      const DidCloseTextDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidSaveTextDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidSaveTextDocumentParams>;
    auto didSaveTextDocumentParamsToAny(
      const DidSaveTextDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentSaveRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentSaveRegistrationOptions>;
    auto textDocumentSaveRegistrationOptionsToAny(
      const TextDocumentSaveRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWillSaveTextDocumentParams(
      const LSPAny &any
    ) const -> std::unique_ptr<WillSaveTextDocumentParams>;
    auto willSaveTextDocumentParamsToAny(
      const WillSaveTextDocumentParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextEdit(
      const LSPAny &any
    ) const -> std::unique_ptr<TextEdit>;
    auto textEditToAny(
      const TextEdit &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeWatchedFilesParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeWatchedFilesParams>;
    auto didChangeWatchedFilesParamsToAny(
      const DidChangeWatchedFilesParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeWatchedFilesRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeWatchedFilesRegistrationOptions>;
    auto didChangeWatchedFilesRegistrationOptionsToAny(
      const DidChangeWatchedFilesRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPublishDiagnosticsParams(
      const LSPAny &any
    ) const -> std::unique_ptr<PublishDiagnosticsParams>;
    auto publishDiagnosticsParamsToAny(
      const PublishDiagnosticsParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionParams(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionParams>;
    auto completionParamsToAny(
      const CompletionParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionItem_documentation(
      const LSPAny &any
    ) const -> CompletionItem_documentation;
    auto completionItem_documentationToAny(
      const CompletionItem_documentation &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionItem_textEdit(
      const LSPAny &any
    ) const -> CompletionItem_textEdit;
    auto completionItem_textEditToAny(
      const CompletionItem_textEdit &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionItem(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionItem>;
    auto completionItemToAny(
      const CompletionItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionList_itemDefaults_editRange_1(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionList_itemDefaults_editRange_1>;
    auto completionList_itemDefaults_editRange_1ToAny(
      const CompletionList_itemDefaults_editRange_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionList_itemDefaults_editRange(
      const LSPAny &any
    ) const -> CompletionList_itemDefaults_editRange;
    auto completionList_itemDefaults_editRangeToAny(
      const CompletionList_itemDefaults_editRange &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionList_itemDefaults(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionList_itemDefaults>;
    auto completionList_itemDefaultsToAny(
      const CompletionList_itemDefaults &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionList(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionList>;
    auto completionListToAny(
      const CompletionList &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionRegistrationOptions>;
    auto completionRegistrationOptionsToAny(
      const CompletionRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToHoverParams(
      const LSPAny &any
    ) const -> std::unique_ptr<HoverParams>;
    auto hoverParamsToAny(
      const HoverParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToHover_contents(
      const LSPAny &any
    ) const -> Hover_contents;
    auto hover_contentsToAny(
      const Hover_contents &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToHover(
      const LSPAny &any
    ) const -> std::unique_ptr<Hover>;
    auto hoverToAny(
      const Hover &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToHoverRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<HoverRegistrationOptions>;
    auto hoverRegistrationOptionsToAny(
      const HoverRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelpParams(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelpParams>;
    auto signatureHelpParamsToAny(
      const SignatureHelpParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelp(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelp>;
    auto signatureHelpToAny(
      const SignatureHelp &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelpRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelpRegistrationOptions>;
    auto signatureHelpRegistrationOptionsToAny(
      const SignatureHelpRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDefinitionParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DefinitionParams>;
    auto definitionParamsToAny(
      const DefinitionParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDefinitionRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DefinitionRegistrationOptions>;
    auto definitionRegistrationOptionsToAny(
      const DefinitionRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToReferenceParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ReferenceParams>;
    auto referenceParamsToAny(
      const ReferenceParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToReferenceRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<ReferenceRegistrationOptions>;
    auto referenceRegistrationOptionsToAny(
      const ReferenceRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentHighlightParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentHighlightParams>;
    auto documentHighlightParamsToAny(
      const DocumentHighlightParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentHighlight(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentHighlight>;
    auto documentHighlightToAny(
      const DocumentHighlight &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentHighlightRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentHighlightRegistrationOptions>;
    auto documentHighlightRegistrationOptionsToAny(
      const DocumentHighlightRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentSymbolParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentSymbolParams>;
    auto documentSymbolParamsToAny(
      const DocumentSymbolParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSymbolInformation(
      const LSPAny &any
    ) const -> std::unique_ptr<SymbolInformation>;
    auto symbolInformationToAny(
      const SymbolInformation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentSymbol(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentSymbol>;
    auto documentSymbolToAny(
      const DocumentSymbol &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentSymbolRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentSymbolRegistrationOptions>;
    auto documentSymbolRegistrationOptionsToAny(
      const DocumentSymbolRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionParams(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionParams>;
    auto codeActionParamsToAny(
      const CodeActionParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCommand(
      const LSPAny &any
    ) const -> std::unique_ptr<Command>;
    auto commandToAny(
      const Command &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeAction_disabled(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeAction_disabled>;
    auto codeAction_disabledToAny(
      const CodeAction_disabled &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeAction(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeAction>;
    auto codeActionToAny(
      const CodeAction &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionRegistrationOptions>;
    auto codeActionRegistrationOptionsToAny(
      const CodeActionRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbolParams(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbolParams>;
    auto workspaceSymbolParamsToAny(
      const WorkspaceSymbolParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbol_location_1(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbol_location_1>;
    auto workspaceSymbol_location_1ToAny(
      const WorkspaceSymbol_location_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbol_location(
      const LSPAny &any
    ) const -> WorkspaceSymbol_location;
    auto workspaceSymbol_locationToAny(
      const WorkspaceSymbol_location &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbol(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbol>;
    auto workspaceSymbolToAny(
      const WorkspaceSymbol &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbolRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbolRegistrationOptions>;
    auto workspaceSymbolRegistrationOptionsToAny(
      const WorkspaceSymbolRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeLensParams(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeLensParams>;
    auto codeLensParamsToAny(
      const CodeLensParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeLens(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeLens>;
    auto codeLensToAny(
      const CodeLens &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeLensRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeLensRegistrationOptions>;
    auto codeLensRegistrationOptionsToAny(
      const CodeLensRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentLinkParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentLinkParams>;
    auto documentLinkParamsToAny(
      const DocumentLinkParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentLink(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentLink>;
    auto documentLinkToAny(
      const DocumentLink &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentLinkRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentLinkRegistrationOptions>;
    auto documentLinkRegistrationOptionsToAny(
      const DocumentLinkRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentFormattingParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentFormattingParams>;
    auto documentFormattingParamsToAny(
      const DocumentFormattingParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentFormattingRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentFormattingRegistrationOptions>;
    auto documentFormattingRegistrationOptionsToAny(
      const DocumentFormattingRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentRangeFormattingParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentRangeFormattingParams>;
    auto documentRangeFormattingParamsToAny(
      const DocumentRangeFormattingParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentRangeFormattingRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentRangeFormattingRegistrationOptions>;
    auto documentRangeFormattingRegistrationOptionsToAny(
      const DocumentRangeFormattingRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentRangesFormattingParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentRangesFormattingParams>;
    auto documentRangesFormattingParamsToAny(
      const DocumentRangesFormattingParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentOnTypeFormattingParams(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentOnTypeFormattingParams>;
    auto documentOnTypeFormattingParamsToAny(
      const DocumentOnTypeFormattingParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentOnTypeFormattingRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentOnTypeFormattingRegistrationOptions>;
    auto documentOnTypeFormattingRegistrationOptionsToAny(
      const DocumentOnTypeFormattingRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRenameParams(
      const LSPAny &any
    ) const -> std::unique_ptr<RenameParams>;
    auto renameParamsToAny(
      const RenameParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRenameRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<RenameRegistrationOptions>;
    auto renameRegistrationOptionsToAny(
      const RenameRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPrepareRenameParams(
      const LSPAny &any
    ) const -> std::unique_ptr<PrepareRenameParams>;
    auto prepareRenameParamsToAny(
      const PrepareRenameParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToExecuteCommandParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ExecuteCommandParams>;
    auto executeCommandParamsToAny(
      const ExecuteCommandParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToExecuteCommandRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<ExecuteCommandRegistrationOptions>;
    auto executeCommandRegistrationOptionsToAny(
      const ExecuteCommandRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToApplyWorkspaceEditParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ApplyWorkspaceEditParams>;
    auto applyWorkspaceEditParamsToAny(
      const ApplyWorkspaceEditParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToApplyWorkspaceEditResult(
      const LSPAny &any
    ) const -> std::unique_ptr<ApplyWorkspaceEditResult>;
    auto applyWorkspaceEditResultToAny(
      const ApplyWorkspaceEditResult &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkDoneProgressBegin(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkDoneProgressBegin>;
    auto workDoneProgressBeginToAny(
      const WorkDoneProgressBegin &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkDoneProgressReport(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkDoneProgressReport>;
    auto workDoneProgressReportToAny(
      const WorkDoneProgressReport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkDoneProgressEnd(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkDoneProgressEnd>;
    auto workDoneProgressEndToAny(
      const WorkDoneProgressEnd &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSetTraceParams(
      const LSPAny &any
    ) const -> std::unique_ptr<SetTraceParams>;
    auto setTraceParamsToAny(
      const SetTraceParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLogTraceParams(
      const LSPAny &any
    ) const -> std::unique_ptr<LogTraceParams>;
    auto logTraceParamsToAny(
      const LogTraceParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCancelParams_id(
      const LSPAny &any
    ) const -> CancelParams_id;
    auto cancelParams_idToAny(
      const CancelParams_id &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCancelParams(
      const LSPAny &any
    ) const -> std::unique_ptr<CancelParams>;
    auto cancelParamsToAny(
      const CancelParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToProgressParams(
      const LSPAny &any
    ) const -> std::unique_ptr<ProgressParams>;
    auto progressParamsToAny(
      const ProgressParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentPositionParams(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentPositionParams>;
    auto textDocumentPositionParamsToAny(
      const TextDocumentPositionParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkDoneProgressParams(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkDoneProgressParams>;
    auto workDoneProgressParamsToAny(
      const WorkDoneProgressParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPartialResultParams(
      const LSPAny &any
    ) const -> std::unique_ptr<PartialResultParams>;
    auto partialResultParamsToAny(
      const PartialResultParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLocationLink(
      const LSPAny &any
    ) const -> std::unique_ptr<LocationLink>;
    auto locationLinkToAny(
      const LocationLink &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRange(
      const LSPAny &any
    ) const -> std::unique_ptr<Range>;
    auto rangeToAny(
      const Range &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToImplementationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<ImplementationOptions>;
    auto implementationOptionsToAny(
      const ImplementationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToStaticRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<StaticRegistrationOptions>;
    auto staticRegistrationOptionsToAny(
      const StaticRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeDefinitionOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeDefinitionOptions>;
    auto typeDefinitionOptionsToAny(
      const TypeDefinitionOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceFoldersChangeEvent(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceFoldersChangeEvent>;
    auto workspaceFoldersChangeEventToAny(
      const WorkspaceFoldersChangeEvent &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToConfigurationItem(
      const LSPAny &any
    ) const -> std::unique_ptr<ConfigurationItem>;
    auto configurationItemToAny(
      const ConfigurationItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentIdentifier>;
    auto textDocumentIdentifierToAny(
      const TextDocumentIdentifier &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToColor(
      const LSPAny &any
    ) const -> std::unique_ptr<Color>;
    auto colorToAny(
      const Color &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentColorOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentColorOptions>;
    auto documentColorOptionsToAny(
      const DocumentColorOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRangeOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeOptions>;
    auto foldingRangeOptionsToAny(
      const FoldingRangeOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeclarationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DeclarationOptions>;
    auto declarationOptionsToAny(
      const DeclarationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPosition(
      const LSPAny &any
    ) const -> std::unique_ptr<Position>;
    auto positionToAny(
      const Position &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSelectionRangeOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<SelectionRangeOptions>;
    auto selectionRangeOptionsToAny(
      const SelectionRangeOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyOptions>;
    auto callHierarchyOptionsToAny(
      const CallHierarchyOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensOptions_range_1(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensOptions_range_1>;
    auto semanticTokensOptions_range_1ToAny(
      const SemanticTokensOptions_range_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensOptions_range(
      const LSPAny &any
    ) const -> SemanticTokensOptions_range;
    auto semanticTokensOptions_rangeToAny(
      const SemanticTokensOptions_range &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensOptions_full_1(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensOptions_full_1>;
    auto semanticTokensOptions_full_1ToAny(
      const SemanticTokensOptions_full_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensOptions_full(
      const LSPAny &any
    ) const -> SemanticTokensOptions_full;
    auto semanticTokensOptions_fullToAny(
      const SemanticTokensOptions_full &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensOptions>;
    auto semanticTokensOptionsToAny(
      const SemanticTokensOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensEdit(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensEdit>;
    auto semanticTokensEditToAny(
      const SemanticTokensEdit &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLinkedEditingRangeOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<LinkedEditingRangeOptions>;
    auto linkedEditingRangeOptionsToAny(
      const LinkedEditingRangeOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileCreate(
      const LSPAny &any
    ) const -> std::unique_ptr<FileCreate>;
    auto fileCreateToAny(
      const FileCreate &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentEdit_edits(
      const LSPAny &any
    ) const -> TextDocumentEdit_edits;
    auto textDocumentEdit_editsToAny(
      const TextDocumentEdit_edits &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentEdit(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentEdit>;
    auto textDocumentEditToAny(
      const TextDocumentEdit &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCreateFile(
      const LSPAny &any
    ) const -> std::unique_ptr<CreateFile>;
    auto createFileToAny(
      const CreateFile &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRenameFile(
      const LSPAny &any
    ) const -> std::unique_ptr<RenameFile>;
    auto renameFileToAny(
      const RenameFile &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeleteFile(
      const LSPAny &any
    ) const -> std::unique_ptr<DeleteFile>;
    auto deleteFileToAny(
      const DeleteFile &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToChangeAnnotation(
      const LSPAny &any
    ) const -> std::unique_ptr<ChangeAnnotation>;
    auto changeAnnotationToAny(
      const ChangeAnnotation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileOperationFilter(
      const LSPAny &any
    ) const -> std::unique_ptr<FileOperationFilter>;
    auto fileOperationFilterToAny(
      const FileOperationFilter &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileRename(
      const LSPAny &any
    ) const -> std::unique_ptr<FileRename>;
    auto fileRenameToAny(
      const FileRename &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileDelete(
      const LSPAny &any
    ) const -> std::unique_ptr<FileDelete>;
    auto fileDeleteToAny(
      const FileDelete &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMonikerOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<MonikerOptions>;
    auto monikerOptionsToAny(
      const MonikerOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeHierarchyOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchyOptions>;
    auto typeHierarchyOptionsToAny(
      const TypeHierarchyOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueContext(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueContext>;
    auto inlineValueContextToAny(
      const InlineValueContext &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueText(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueText>;
    auto inlineValueTextToAny(
      const InlineValueText &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueVariableLookup(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueVariableLookup>;
    auto inlineValueVariableLookupToAny(
      const InlineValueVariableLookup &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueEvaluatableExpression(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueEvaluatableExpression>;
    auto inlineValueEvaluatableExpressionToAny(
      const InlineValueEvaluatableExpression &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueOptions>;
    auto inlineValueOptionsToAny(
      const InlineValueOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintLabelPart_tooltip(
      const LSPAny &any
    ) const -> InlayHintLabelPart_tooltip;
    auto inlayHintLabelPart_tooltipToAny(
      const InlayHintLabelPart_tooltip &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintLabelPart(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintLabelPart>;
    auto inlayHintLabelPartToAny(
      const InlayHintLabelPart &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMarkupContent(
      const LSPAny &any
    ) const -> std::unique_ptr<MarkupContent>;
    auto markupContentToAny(
      const MarkupContent &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintOptions>;
    auto inlayHintOptionsToAny(
      const InlayHintOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRelatedFullDocumentDiagnosticReport_relatedDocuments(
      const LSPAny &any
    ) const -> RelatedFullDocumentDiagnosticReport_relatedDocuments;
    auto relatedFullDocumentDiagnosticReport_relatedDocumentsToAny(
      const RelatedFullDocumentDiagnosticReport_relatedDocuments &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRelatedFullDocumentDiagnosticReport(
      const LSPAny &any
    ) const -> std::unique_ptr<RelatedFullDocumentDiagnosticReport>;
    auto relatedFullDocumentDiagnosticReportToAny(
      const RelatedFullDocumentDiagnosticReport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRelatedUnchangedDocumentDiagnosticReport_relatedDocuments(
      const LSPAny &any
    ) const -> RelatedUnchangedDocumentDiagnosticReport_relatedDocuments;
    auto relatedUnchangedDocumentDiagnosticReport_relatedDocumentsToAny(
      const RelatedUnchangedDocumentDiagnosticReport_relatedDocuments &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRelatedUnchangedDocumentDiagnosticReport(
      const LSPAny &any
    ) const -> std::unique_ptr<RelatedUnchangedDocumentDiagnosticReport>;
    auto relatedUnchangedDocumentDiagnosticReportToAny(
      const RelatedUnchangedDocumentDiagnosticReport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFullDocumentDiagnosticReport(
      const LSPAny &any
    ) const -> std::unique_ptr<FullDocumentDiagnosticReport>;
    auto fullDocumentDiagnosticReportToAny(
      const FullDocumentDiagnosticReport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToUnchangedDocumentDiagnosticReport(
      const LSPAny &any
    ) const -> std::unique_ptr<UnchangedDocumentDiagnosticReport>;
    auto unchangedDocumentDiagnosticReportToAny(
      const UnchangedDocumentDiagnosticReport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnosticOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DiagnosticOptions>;
    auto diagnosticOptionsToAny(
      const DiagnosticOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPreviousResultId(
      const LSPAny &any
    ) const -> std::unique_ptr<PreviousResultId>;
    auto previousResultIdToAny(
      const PreviousResultId &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocument(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocument>;
    auto notebookDocumentToAny(
      const NotebookDocument &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentItem(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentItem>;
    auto textDocumentItemToAny(
      const TextDocumentItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToVersionedNotebookDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<VersionedNotebookDocumentIdentifier>;
    auto versionedNotebookDocumentIdentifierToAny(
      const VersionedNotebookDocumentIdentifier &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentChangeEvent_cells_textContent(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentChangeEvent_cells_textContent>;
    auto notebookDocumentChangeEvent_cells_textContentToAny(
      const NotebookDocumentChangeEvent_cells_textContent &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentChangeEvent_cells_structure(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentChangeEvent_cells_structure>;
    auto notebookDocumentChangeEvent_cells_structureToAny(
      const NotebookDocumentChangeEvent_cells_structure &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentChangeEvent_cells(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentChangeEvent_cells>;
    auto notebookDocumentChangeEvent_cellsToAny(
      const NotebookDocumentChangeEvent_cells &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentChangeEvent(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentChangeEvent>;
    auto notebookDocumentChangeEventToAny(
      const NotebookDocumentChangeEvent &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentIdentifier>;
    auto notebookDocumentIdentifierToAny(
      const NotebookDocumentIdentifier &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionContext(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineCompletionContext>;
    auto inlineCompletionContextToAny(
      const InlineCompletionContext &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToStringValue(
      const LSPAny &any
    ) const -> std::unique_ptr<StringValue>;
    auto stringValueToAny(
      const StringValue &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineCompletionOptions>;
    auto inlineCompletionOptionsToAny(
      const InlineCompletionOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRegistration(
      const LSPAny &any
    ) const -> std::unique_ptr<Registration>;
    auto registrationToAny(
      const Registration &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToUnregistration(
      const LSPAny &any
    ) const -> std::unique_ptr<Unregistration>;
    auto unregistrationToAny(
      const Unregistration &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyTo_InitializeParams_processId(
      const LSPAny &any
    ) const -> _InitializeParams_processId;
    auto _InitializeParams_processIdToAny(
      const _InitializeParams_processId &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyTo_InitializeParams_clientInfo(
      const LSPAny &any
    ) const -> std::unique_ptr<_InitializeParams_clientInfo>;
    auto _InitializeParams_clientInfoToAny(
      const _InitializeParams_clientInfo &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyTo_InitializeParams_rootPath(
      const LSPAny &any
    ) const -> _InitializeParams_rootPath;
    auto _InitializeParams_rootPathToAny(
      const _InitializeParams_rootPath &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyTo_InitializeParams_rootUri(
      const LSPAny &any
    ) const -> _InitializeParams_rootUri;
    auto _InitializeParams_rootUriToAny(
      const _InitializeParams_rootUri &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyTo_InitializeParams(
      const LSPAny &any
    ) const -> std::unique_ptr<_InitializeParams>;
    auto _InitializeParamsToAny(
      const _InitializeParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceFoldersInitializeParams_workspaceFolders(
      const LSPAny &any
    ) const -> WorkspaceFoldersInitializeParams_workspaceFolders;
    auto workspaceFoldersInitializeParams_workspaceFoldersToAny(
      const WorkspaceFoldersInitializeParams_workspaceFolders &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceFoldersInitializeParams(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceFoldersInitializeParams>;
    auto workspaceFoldersInitializeParamsToAny(
      const WorkspaceFoldersInitializeParams &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_textDocumentSync(
      const LSPAny &any
    ) const -> ServerCapabilities_textDocumentSync;
    auto serverCapabilities_textDocumentSyncToAny(
      const ServerCapabilities_textDocumentSync &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_notebookDocumentSync(
      const LSPAny &any
    ) const -> ServerCapabilities_notebookDocumentSync;
    auto serverCapabilities_notebookDocumentSyncToAny(
      const ServerCapabilities_notebookDocumentSync &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_hoverProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_hoverProvider;
    auto serverCapabilities_hoverProviderToAny(
      const ServerCapabilities_hoverProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_declarationProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_declarationProvider;
    auto serverCapabilities_declarationProviderToAny(
      const ServerCapabilities_declarationProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_definitionProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_definitionProvider;
    auto serverCapabilities_definitionProviderToAny(
      const ServerCapabilities_definitionProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_typeDefinitionProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_typeDefinitionProvider;
    auto serverCapabilities_typeDefinitionProviderToAny(
      const ServerCapabilities_typeDefinitionProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_implementationProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_implementationProvider;
    auto serverCapabilities_implementationProviderToAny(
      const ServerCapabilities_implementationProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_referencesProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_referencesProvider;
    auto serverCapabilities_referencesProviderToAny(
      const ServerCapabilities_referencesProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_documentHighlightProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_documentHighlightProvider;
    auto serverCapabilities_documentHighlightProviderToAny(
      const ServerCapabilities_documentHighlightProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_documentSymbolProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_documentSymbolProvider;
    auto serverCapabilities_documentSymbolProviderToAny(
      const ServerCapabilities_documentSymbolProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_codeActionProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_codeActionProvider;
    auto serverCapabilities_codeActionProviderToAny(
      const ServerCapabilities_codeActionProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_colorProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_colorProvider;
    auto serverCapabilities_colorProviderToAny(
      const ServerCapabilities_colorProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_workspaceSymbolProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_workspaceSymbolProvider;
    auto serverCapabilities_workspaceSymbolProviderToAny(
      const ServerCapabilities_workspaceSymbolProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_documentFormattingProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_documentFormattingProvider;
    auto serverCapabilities_documentFormattingProviderToAny(
      const ServerCapabilities_documentFormattingProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_documentRangeFormattingProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_documentRangeFormattingProvider;
    auto serverCapabilities_documentRangeFormattingProviderToAny(
      const ServerCapabilities_documentRangeFormattingProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_renameProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_renameProvider;
    auto serverCapabilities_renameProviderToAny(
      const ServerCapabilities_renameProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_foldingRangeProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_foldingRangeProvider;
    auto serverCapabilities_foldingRangeProviderToAny(
      const ServerCapabilities_foldingRangeProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_selectionRangeProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_selectionRangeProvider;
    auto serverCapabilities_selectionRangeProviderToAny(
      const ServerCapabilities_selectionRangeProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_callHierarchyProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_callHierarchyProvider;
    auto serverCapabilities_callHierarchyProviderToAny(
      const ServerCapabilities_callHierarchyProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_linkedEditingRangeProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_linkedEditingRangeProvider;
    auto serverCapabilities_linkedEditingRangeProviderToAny(
      const ServerCapabilities_linkedEditingRangeProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_semanticTokensProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_semanticTokensProvider;
    auto serverCapabilities_semanticTokensProviderToAny(
      const ServerCapabilities_semanticTokensProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_monikerProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_monikerProvider;
    auto serverCapabilities_monikerProviderToAny(
      const ServerCapabilities_monikerProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_typeHierarchyProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_typeHierarchyProvider;
    auto serverCapabilities_typeHierarchyProviderToAny(
      const ServerCapabilities_typeHierarchyProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_inlineValueProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_inlineValueProvider;
    auto serverCapabilities_inlineValueProviderToAny(
      const ServerCapabilities_inlineValueProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_inlayHintProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_inlayHintProvider;
    auto serverCapabilities_inlayHintProviderToAny(
      const ServerCapabilities_inlayHintProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_diagnosticProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_diagnosticProvider;
    auto serverCapabilities_diagnosticProviderToAny(
      const ServerCapabilities_diagnosticProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_inlineCompletionProvider(
      const LSPAny &any
    ) const -> ServerCapabilities_inlineCompletionProvider;
    auto serverCapabilities_inlineCompletionProviderToAny(
      const ServerCapabilities_inlineCompletionProvider &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities_workspace(
      const LSPAny &any
    ) const -> std::unique_ptr<ServerCapabilities_workspace>;
    auto serverCapabilities_workspaceToAny(
      const ServerCapabilities_workspace &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToServerCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ServerCapabilities>;
    auto serverCapabilitiesToAny(
      const ServerCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToVersionedTextDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<VersionedTextDocumentIdentifier>;
    auto versionedTextDocumentIdentifierToAny(
      const VersionedTextDocumentIdentifier &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSaveOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<SaveOptions>;
    auto saveOptionsToAny(
      const SaveOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileEvent(
      const LSPAny &any
    ) const -> std::unique_ptr<FileEvent>;
    auto fileEventToAny(
      const FileEvent &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileSystemWatcher(
      const LSPAny &any
    ) const -> std::unique_ptr<FileSystemWatcher>;
    auto fileSystemWatcherToAny(
      const FileSystemWatcher &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnostic_code(
      const LSPAny &any
    ) const -> Diagnostic_code;
    auto diagnostic_codeToAny(
      const Diagnostic_code &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnostic(
      const LSPAny &any
    ) const -> std::unique_ptr<Diagnostic>;
    auto diagnosticToAny(
      const Diagnostic &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionContext(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionContext>;
    auto completionContextToAny(
      const CompletionContext &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionItemLabelDetails(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionItemLabelDetails>;
    auto completionItemLabelDetailsToAny(
      const CompletionItemLabelDetails &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInsertReplaceEdit(
      const LSPAny &any
    ) const -> std::unique_ptr<InsertReplaceEdit>;
    auto insertReplaceEditToAny(
      const InsertReplaceEdit &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionOptions_completionItem(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionOptions_completionItem>;
    auto completionOptions_completionItemToAny(
      const CompletionOptions_completionItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionOptions>;
    auto completionOptionsToAny(
      const CompletionOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToHoverOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<HoverOptions>;
    auto hoverOptionsToAny(
      const HoverOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelpContext(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelpContext>;
    auto signatureHelpContextToAny(
      const SignatureHelpContext &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureInformation_documentation(
      const LSPAny &any
    ) const -> SignatureInformation_documentation;
    auto signatureInformation_documentationToAny(
      const SignatureInformation_documentation &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureInformation(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureInformation>;
    auto signatureInformationToAny(
      const SignatureInformation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelpOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelpOptions>;
    auto signatureHelpOptionsToAny(
      const SignatureHelpOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDefinitionOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DefinitionOptions>;
    auto definitionOptionsToAny(
      const DefinitionOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToReferenceContext(
      const LSPAny &any
    ) const -> std::unique_ptr<ReferenceContext>;
    auto referenceContextToAny(
      const ReferenceContext &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToReferenceOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<ReferenceOptions>;
    auto referenceOptionsToAny(
      const ReferenceOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentHighlightOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentHighlightOptions>;
    auto documentHighlightOptionsToAny(
      const DocumentHighlightOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToBaseSymbolInformation(
      const LSPAny &any
    ) const -> std::unique_ptr<BaseSymbolInformation>;
    auto baseSymbolInformationToAny(
      const BaseSymbolInformation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentSymbolOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentSymbolOptions>;
    auto documentSymbolOptionsToAny(
      const DocumentSymbolOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionContext(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionContext>;
    auto codeActionContextToAny(
      const CodeActionContext &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionOptions>;
    auto codeActionOptionsToAny(
      const CodeActionOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbolOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbolOptions>;
    auto workspaceSymbolOptionsToAny(
      const WorkspaceSymbolOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeLensOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeLensOptions>;
    auto codeLensOptionsToAny(
      const CodeLensOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentLinkOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentLinkOptions>;
    auto documentLinkOptionsToAny(
      const DocumentLinkOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFormattingOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<FormattingOptions>;
    auto formattingOptionsToAny(
      const FormattingOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentFormattingOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentFormattingOptions>;
    auto documentFormattingOptionsToAny(
      const DocumentFormattingOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentRangeFormattingOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentRangeFormattingOptions>;
    auto documentRangeFormattingOptionsToAny(
      const DocumentRangeFormattingOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentOnTypeFormattingOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentOnTypeFormattingOptions>;
    auto documentOnTypeFormattingOptionsToAny(
      const DocumentOnTypeFormattingOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRenameOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<RenameOptions>;
    auto renameOptionsToAny(
      const RenameOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToExecuteCommandOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<ExecuteCommandOptions>;
    auto executeCommandOptionsToAny(
      const ExecuteCommandOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensLegend(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensLegend>;
    auto semanticTokensLegendToAny(
      const SemanticTokensLegend &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToOptionalVersionedTextDocumentIdentifier_version(
      const LSPAny &any
    ) const -> OptionalVersionedTextDocumentIdentifier_version;
    auto optionalVersionedTextDocumentIdentifier_versionToAny(
      const OptionalVersionedTextDocumentIdentifier_version &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToOptionalVersionedTextDocumentIdentifier(
      const LSPAny &any
    ) const -> std::unique_ptr<OptionalVersionedTextDocumentIdentifier>;
    auto optionalVersionedTextDocumentIdentifierToAny(
      const OptionalVersionedTextDocumentIdentifier &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToAnnotatedTextEdit(
      const LSPAny &any
    ) const -> std::unique_ptr<AnnotatedTextEdit>;
    auto annotatedTextEditToAny(
      const AnnotatedTextEdit &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToResourceOperation(
      const LSPAny &any
    ) const -> std::unique_ptr<ResourceOperation>;
    auto resourceOperationToAny(
      const ResourceOperation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCreateFileOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<CreateFileOptions>;
    auto createFileOptionsToAny(
      const CreateFileOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRenameFileOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<RenameFileOptions>;
    auto renameFileOptionsToAny(
      const RenameFileOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeleteFileOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<DeleteFileOptions>;
    auto deleteFileOptionsToAny(
      const DeleteFileOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileOperationPattern(
      const LSPAny &any
    ) const -> std::unique_ptr<FileOperationPattern>;
    auto fileOperationPatternToAny(
      const FileOperationPattern &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceFullDocumentDiagnosticReport_version(
      const LSPAny &any
    ) const -> WorkspaceFullDocumentDiagnosticReport_version;
    auto workspaceFullDocumentDiagnosticReport_versionToAny(
      const WorkspaceFullDocumentDiagnosticReport_version &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceFullDocumentDiagnosticReport(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceFullDocumentDiagnosticReport>;
    auto workspaceFullDocumentDiagnosticReportToAny(
      const WorkspaceFullDocumentDiagnosticReport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceUnchangedDocumentDiagnosticReport_version(
      const LSPAny &any
    ) const -> WorkspaceUnchangedDocumentDiagnosticReport_version;
    auto workspaceUnchangedDocumentDiagnosticReport_versionToAny(
      const WorkspaceUnchangedDocumentDiagnosticReport_version &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceUnchangedDocumentDiagnosticReport(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceUnchangedDocumentDiagnosticReport>;
    auto workspaceUnchangedDocumentDiagnosticReportToAny(
      const WorkspaceUnchangedDocumentDiagnosticReport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookCell(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookCell>;
    auto notebookCellToAny(
      const NotebookCell &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookCellArrayChange(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookCellArrayChange>;
    auto notebookCellArrayChangeToAny(
      const NotebookCellArrayChange &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSelectedCompletionInfo(
      const LSPAny &any
    ) const -> std::unique_ptr<SelectedCompletionInfo>;
    auto selectedCompletionInfoToAny(
      const SelectedCompletionInfo &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ClientCapabilities>;
    auto clientCapabilitiesToAny(
      const ClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentSyncOptions_save(
      const LSPAny &any
    ) const -> TextDocumentSyncOptions_save;
    auto textDocumentSyncOptions_saveToAny(
      const TextDocumentSyncOptions_save &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentSyncOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentSyncOptions>;
    auto textDocumentSyncOptionsToAny(
      const TextDocumentSyncOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncOptions_notebookSelector_1_cells(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentSyncOptions_notebookSelector_1_cells>;
    auto notebookDocumentSyncOptions_notebookSelector_1_cellsToAny(
      const NotebookDocumentSyncOptions_notebookSelector_1_cells &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncOptions_notebookSelector_1_notebook(
      const LSPAny &any
    ) const -> NotebookDocumentSyncOptions_notebookSelector_1_notebook;
    auto notebookDocumentSyncOptions_notebookSelector_1_notebookToAny(
      const NotebookDocumentSyncOptions_notebookSelector_1_notebook &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncOptions_notebookSelector_1(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentSyncOptions_notebookSelector_1>;
    auto notebookDocumentSyncOptions_notebookSelector_1ToAny(
      const NotebookDocumentSyncOptions_notebookSelector_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncOptions_notebookSelector_0_cells(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentSyncOptions_notebookSelector_0_cells>;
    auto notebookDocumentSyncOptions_notebookSelector_0_cellsToAny(
      const NotebookDocumentSyncOptions_notebookSelector_0_cells &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncOptions_notebookSelector_0_notebook(
      const LSPAny &any
    ) const -> NotebookDocumentSyncOptions_notebookSelector_0_notebook;
    auto notebookDocumentSyncOptions_notebookSelector_0_notebookToAny(
      const NotebookDocumentSyncOptions_notebookSelector_0_notebook &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncOptions_notebookSelector_0(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentSyncOptions_notebookSelector_0>;
    auto notebookDocumentSyncOptions_notebookSelector_0ToAny(
      const NotebookDocumentSyncOptions_notebookSelector_0 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncOptions_notebookSelector(
      const LSPAny &any
    ) const -> NotebookDocumentSyncOptions_notebookSelector;
    auto notebookDocumentSyncOptions_notebookSelectorToAny(
      const NotebookDocumentSyncOptions_notebookSelector &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentSyncOptions>;
    auto notebookDocumentSyncOptionsToAny(
      const NotebookDocumentSyncOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncRegistrationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentSyncRegistrationOptions>;
    auto notebookDocumentSyncRegistrationOptionsToAny(
      const NotebookDocumentSyncRegistrationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceFoldersServerCapabilities_changeNotifications(
      const LSPAny &any
    ) const -> WorkspaceFoldersServerCapabilities_changeNotifications;
    auto workspaceFoldersServerCapabilities_changeNotificationsToAny(
      const WorkspaceFoldersServerCapabilities_changeNotifications &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceFoldersServerCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceFoldersServerCapabilities>;
    auto workspaceFoldersServerCapabilitiesToAny(
      const WorkspaceFoldersServerCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileOperationOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<FileOperationOptions>;
    auto fileOperationOptionsToAny(
      const FileOperationOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeDescription(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeDescription>;
    auto codeDescriptionToAny(
      const CodeDescription &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnosticRelatedInformation(
      const LSPAny &any
    ) const -> std::unique_ptr<DiagnosticRelatedInformation>;
    auto diagnosticRelatedInformationToAny(
      const DiagnosticRelatedInformation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToParameterInformation_label(
      const LSPAny &any
    ) const -> ParameterInformation_label;
    auto parameterInformation_labelToAny(
      const ParameterInformation_label &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToParameterInformation_documentation(
      const LSPAny &any
    ) const -> ParameterInformation_documentation;
    auto parameterInformation_documentationToAny(
      const ParameterInformation_documentation &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToParameterInformation(
      const LSPAny &any
    ) const -> std::unique_ptr<ParameterInformation>;
    auto parameterInformationToAny(
      const ParameterInformation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookCellTextDocumentFilter_notebook(
      const LSPAny &any
    ) const -> NotebookCellTextDocumentFilter_notebook;
    auto notebookCellTextDocumentFilter_notebookToAny(
      const NotebookCellTextDocumentFilter_notebook &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookCellTextDocumentFilter(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookCellTextDocumentFilter>;
    auto notebookCellTextDocumentFilterToAny(
      const NotebookCellTextDocumentFilter &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileOperationPatternOptions(
      const LSPAny &any
    ) const -> std::unique_ptr<FileOperationPatternOptions>;
    auto fileOperationPatternOptionsToAny(
      const FileOperationPatternOptions &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToExecutionSummary(
      const LSPAny &any
    ) const -> std::unique_ptr<ExecutionSummary>;
    auto executionSummaryToAny(
      const ExecutionSummary &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceClientCapabilities>;
    auto workspaceClientCapabilitiesToAny(
      const WorkspaceClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentClientCapabilities>;
    auto textDocumentClientCapabilitiesToAny(
      const TextDocumentClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentClientCapabilities>;
    auto notebookDocumentClientCapabilitiesToAny(
      const NotebookDocumentClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWindowClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WindowClientCapabilities>;
    auto windowClientCapabilitiesToAny(
      const WindowClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToGeneralClientCapabilities_staleRequestSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<GeneralClientCapabilities_staleRequestSupport>;
    auto generalClientCapabilities_staleRequestSupportToAny(
      const GeneralClientCapabilities_staleRequestSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToGeneralClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<GeneralClientCapabilities>;
    auto generalClientCapabilitiesToAny(
      const GeneralClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRelativePattern_baseUri(
      const LSPAny &any
    ) const -> RelativePattern_baseUri;
    auto relativePattern_baseUriToAny(
      const RelativePattern_baseUri &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRelativePattern(
      const LSPAny &any
    ) const -> std::unique_ptr<RelativePattern>;
    auto relativePatternToAny(
      const RelativePattern &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceEditClientCapabilities_changeAnnotationSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceEditClientCapabilities_changeAnnotationSupport>;
    auto workspaceEditClientCapabilities_changeAnnotationSupportToAny(
      const WorkspaceEditClientCapabilities_changeAnnotationSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceEditClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceEditClientCapabilities>;
    auto workspaceEditClientCapabilitiesToAny(
      const WorkspaceEditClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeConfigurationClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeConfigurationClientCapabilities>;
    auto didChangeConfigurationClientCapabilitiesToAny(
      const DidChangeConfigurationClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDidChangeWatchedFilesClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DidChangeWatchedFilesClientCapabilities>;
    auto didChangeWatchedFilesClientCapabilitiesToAny(
      const DidChangeWatchedFilesClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbolClientCapabilities_symbolKind(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbolClientCapabilities_symbolKind>;
    auto workspaceSymbolClientCapabilities_symbolKindToAny(
      const WorkspaceSymbolClientCapabilities_symbolKind &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbolClientCapabilities_tagSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbolClientCapabilities_tagSupport>;
    auto workspaceSymbolClientCapabilities_tagSupportToAny(
      const WorkspaceSymbolClientCapabilities_tagSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbolClientCapabilities_resolveSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbolClientCapabilities_resolveSupport>;
    auto workspaceSymbolClientCapabilities_resolveSupportToAny(
      const WorkspaceSymbolClientCapabilities_resolveSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceSymbolClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<WorkspaceSymbolClientCapabilities>;
    auto workspaceSymbolClientCapabilitiesToAny(
      const WorkspaceSymbolClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToExecuteCommandClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ExecuteCommandClientCapabilities>;
    auto executeCommandClientCapabilitiesToAny(
      const ExecuteCommandClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensWorkspaceClientCapabilities>;
    auto semanticTokensWorkspaceClientCapabilitiesToAny(
      const SemanticTokensWorkspaceClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeLensWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeLensWorkspaceClientCapabilities>;
    auto codeLensWorkspaceClientCapabilitiesToAny(
      const CodeLensWorkspaceClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFileOperationClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<FileOperationClientCapabilities>;
    auto fileOperationClientCapabilitiesToAny(
      const FileOperationClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueWorkspaceClientCapabilities>;
    auto inlineValueWorkspaceClientCapabilitiesToAny(
      const InlineValueWorkspaceClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintWorkspaceClientCapabilities>;
    auto inlayHintWorkspaceClientCapabilitiesToAny(
      const InlayHintWorkspaceClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnosticWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DiagnosticWorkspaceClientCapabilities>;
    auto diagnosticWorkspaceClientCapabilitiesToAny(
      const DiagnosticWorkspaceClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRangeWorkspaceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeWorkspaceClientCapabilities>;
    auto foldingRangeWorkspaceClientCapabilitiesToAny(
      const FoldingRangeWorkspaceClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentSyncClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentSyncClientCapabilities>;
    auto textDocumentSyncClientCapabilitiesToAny(
      const TextDocumentSyncClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionClientCapabilities_completionItem_insertTextModeSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionClientCapabilities_completionItem_insertTextModeSupport>;
    auto completionClientCapabilities_completionItem_insertTextModeSupportToAny(
      const CompletionClientCapabilities_completionItem_insertTextModeSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionClientCapabilities_completionItem_resolveSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionClientCapabilities_completionItem_resolveSupport>;
    auto completionClientCapabilities_completionItem_resolveSupportToAny(
      const CompletionClientCapabilities_completionItem_resolveSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionClientCapabilities_completionItem_tagSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionClientCapabilities_completionItem_tagSupport>;
    auto completionClientCapabilities_completionItem_tagSupportToAny(
      const CompletionClientCapabilities_completionItem_tagSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionClientCapabilities_completionItem(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionClientCapabilities_completionItem>;
    auto completionClientCapabilities_completionItemToAny(
      const CompletionClientCapabilities_completionItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionClientCapabilities_completionItemKind(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionClientCapabilities_completionItemKind>;
    auto completionClientCapabilities_completionItemKindToAny(
      const CompletionClientCapabilities_completionItemKind &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionClientCapabilities_completionList(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionClientCapabilities_completionList>;
    auto completionClientCapabilities_completionListToAny(
      const CompletionClientCapabilities_completionList &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCompletionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CompletionClientCapabilities>;
    auto completionClientCapabilitiesToAny(
      const CompletionClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToHoverClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<HoverClientCapabilities>;
    auto hoverClientCapabilitiesToAny(
      const HoverClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelpClientCapabilities_signatureInformation_parameterInformation(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelpClientCapabilities_signatureInformation_parameterInformation>;
    auto signatureHelpClientCapabilities_signatureInformation_parameterInformationToAny(
      const SignatureHelpClientCapabilities_signatureInformation_parameterInformation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelpClientCapabilities_signatureInformation(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelpClientCapabilities_signatureInformation>;
    auto signatureHelpClientCapabilities_signatureInformationToAny(
      const SignatureHelpClientCapabilities_signatureInformation &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSignatureHelpClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SignatureHelpClientCapabilities>;
    auto signatureHelpClientCapabilitiesToAny(
      const SignatureHelpClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeclarationClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DeclarationClientCapabilities>;
    auto declarationClientCapabilitiesToAny(
      const DeclarationClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDefinitionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DefinitionClientCapabilities>;
    auto definitionClientCapabilitiesToAny(
      const DefinitionClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeDefinitionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeDefinitionClientCapabilities>;
    auto typeDefinitionClientCapabilitiesToAny(
      const TypeDefinitionClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToImplementationClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ImplementationClientCapabilities>;
    auto implementationClientCapabilitiesToAny(
      const ImplementationClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToReferenceClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ReferenceClientCapabilities>;
    auto referenceClientCapabilitiesToAny(
      const ReferenceClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentHighlightClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentHighlightClientCapabilities>;
    auto documentHighlightClientCapabilitiesToAny(
      const DocumentHighlightClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentSymbolClientCapabilities_symbolKind(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentSymbolClientCapabilities_symbolKind>;
    auto documentSymbolClientCapabilities_symbolKindToAny(
      const DocumentSymbolClientCapabilities_symbolKind &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentSymbolClientCapabilities_tagSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentSymbolClientCapabilities_tagSupport>;
    auto documentSymbolClientCapabilities_tagSupportToAny(
      const DocumentSymbolClientCapabilities_tagSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentSymbolClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentSymbolClientCapabilities>;
    auto documentSymbolClientCapabilitiesToAny(
      const DocumentSymbolClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionClientCapabilities_codeActionLiteralSupport_codeActionKind(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionClientCapabilities_codeActionLiteralSupport_codeActionKind>;
    auto codeActionClientCapabilities_codeActionLiteralSupport_codeActionKindToAny(
      const CodeActionClientCapabilities_codeActionLiteralSupport_codeActionKind &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionClientCapabilities_codeActionLiteralSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionClientCapabilities_codeActionLiteralSupport>;
    auto codeActionClientCapabilities_codeActionLiteralSupportToAny(
      const CodeActionClientCapabilities_codeActionLiteralSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionClientCapabilities_resolveSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionClientCapabilities_resolveSupport>;
    auto codeActionClientCapabilities_resolveSupportToAny(
      const CodeActionClientCapabilities_resolveSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeActionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeActionClientCapabilities>;
    auto codeActionClientCapabilitiesToAny(
      const CodeActionClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCodeLensClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CodeLensClientCapabilities>;
    auto codeLensClientCapabilitiesToAny(
      const CodeLensClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentLinkClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentLinkClientCapabilities>;
    auto documentLinkClientCapabilitiesToAny(
      const DocumentLinkClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentColorClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentColorClientCapabilities>;
    auto documentColorClientCapabilitiesToAny(
      const DocumentColorClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentFormattingClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentFormattingClientCapabilities>;
    auto documentFormattingClientCapabilitiesToAny(
      const DocumentFormattingClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentRangeFormattingClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentRangeFormattingClientCapabilities>;
    auto documentRangeFormattingClientCapabilitiesToAny(
      const DocumentRangeFormattingClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentOnTypeFormattingClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DocumentOnTypeFormattingClientCapabilities>;
    auto documentOnTypeFormattingClientCapabilitiesToAny(
      const DocumentOnTypeFormattingClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRenameClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<RenameClientCapabilities>;
    auto renameClientCapabilitiesToAny(
      const RenameClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRangeClientCapabilities_foldingRangeKind(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeClientCapabilities_foldingRangeKind>;
    auto foldingRangeClientCapabilities_foldingRangeKindToAny(
      const FoldingRangeClientCapabilities_foldingRangeKind &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRangeClientCapabilities_foldingRange(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeClientCapabilities_foldingRange>;
    auto foldingRangeClientCapabilities_foldingRangeToAny(
      const FoldingRangeClientCapabilities_foldingRange &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToFoldingRangeClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<FoldingRangeClientCapabilities>;
    auto foldingRangeClientCapabilitiesToAny(
      const FoldingRangeClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSelectionRangeClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SelectionRangeClientCapabilities>;
    auto selectionRangeClientCapabilitiesToAny(
      const SelectionRangeClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPublishDiagnosticsClientCapabilities_tagSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<PublishDiagnosticsClientCapabilities_tagSupport>;
    auto publishDiagnosticsClientCapabilities_tagSupportToAny(
      const PublishDiagnosticsClientCapabilities_tagSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPublishDiagnosticsClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<PublishDiagnosticsClientCapabilities>;
    auto publishDiagnosticsClientCapabilitiesToAny(
      const PublishDiagnosticsClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToCallHierarchyClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<CallHierarchyClientCapabilities>;
    auto callHierarchyClientCapabilitiesToAny(
      const CallHierarchyClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensClientCapabilities_requests_full_1(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensClientCapabilities_requests_full_1>;
    auto semanticTokensClientCapabilities_requests_full_1ToAny(
      const SemanticTokensClientCapabilities_requests_full_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensClientCapabilities_requests_full(
      const LSPAny &any
    ) const -> SemanticTokensClientCapabilities_requests_full;
    auto semanticTokensClientCapabilities_requests_fullToAny(
      const SemanticTokensClientCapabilities_requests_full &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensClientCapabilities_requests_range_1(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensClientCapabilities_requests_range_1>;
    auto semanticTokensClientCapabilities_requests_range_1ToAny(
      const SemanticTokensClientCapabilities_requests_range_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensClientCapabilities_requests_range(
      const LSPAny &any
    ) const -> SemanticTokensClientCapabilities_requests_range;
    auto semanticTokensClientCapabilities_requests_rangeToAny(
      const SemanticTokensClientCapabilities_requests_range &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensClientCapabilities_requests(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensClientCapabilities_requests>;
    auto semanticTokensClientCapabilities_requestsToAny(
      const SemanticTokensClientCapabilities_requests &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToSemanticTokensClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<SemanticTokensClientCapabilities>;
    auto semanticTokensClientCapabilitiesToAny(
      const SemanticTokensClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToLinkedEditingRangeClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<LinkedEditingRangeClientCapabilities>;
    auto linkedEditingRangeClientCapabilitiesToAny(
      const LinkedEditingRangeClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMonikerClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<MonikerClientCapabilities>;
    auto monikerClientCapabilitiesToAny(
      const MonikerClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTypeHierarchyClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<TypeHierarchyClientCapabilities>;
    auto typeHierarchyClientCapabilitiesToAny(
      const TypeHierarchyClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValueClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineValueClientCapabilities>;
    auto inlineValueClientCapabilitiesToAny(
      const InlineValueClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintClientCapabilities_resolveSupport(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintClientCapabilities_resolveSupport>;
    auto inlayHintClientCapabilities_resolveSupportToAny(
      const InlayHintClientCapabilities_resolveSupport &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlayHintClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlayHintClientCapabilities>;
    auto inlayHintClientCapabilitiesToAny(
      const InlayHintClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDiagnosticClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<DiagnosticClientCapabilities>;
    auto diagnosticClientCapabilitiesToAny(
      const DiagnosticClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineCompletionClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<InlineCompletionClientCapabilities>;
    auto inlineCompletionClientCapabilitiesToAny(
      const InlineCompletionClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentSyncClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentSyncClientCapabilities>;
    auto notebookDocumentSyncClientCapabilitiesToAny(
      const NotebookDocumentSyncClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToShowMessageRequestClientCapabilities_messageActionItem(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowMessageRequestClientCapabilities_messageActionItem>;
    auto showMessageRequestClientCapabilities_messageActionItemToAny(
      const ShowMessageRequestClientCapabilities_messageActionItem &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToShowMessageRequestClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowMessageRequestClientCapabilities>;
    auto showMessageRequestClientCapabilitiesToAny(
      const ShowMessageRequestClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToShowDocumentClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<ShowDocumentClientCapabilities>;
    auto showDocumentClientCapabilitiesToAny(
      const ShowDocumentClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRegularExpressionsClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<RegularExpressionsClientCapabilities>;
    auto regularExpressionsClientCapabilitiesToAny(
      const RegularExpressionsClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMarkdownClientCapabilities(
      const LSPAny &any
    ) const -> std::unique_ptr<MarkdownClientCapabilities>;
    auto markdownClientCapabilitiesToAny(
      const MarkdownClientCapabilities &structure
    ) const -> std::unique_ptr<LSPAny>;

    // ==================================== //
    // LSPAny <-> LSP Type Alias Transforms //
    // ==================================== //

    auto anyToInteger(
      const LSPAny &any
    ) const -> int;
    auto integerToAny(
      const integer_t &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToUInteger(
      const LSPAny &any
    ) const -> unsigned int;
    auto uintegerToAny(
      const uinteger_t &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDecimal(
      const LSPAny &any
    ) const -> double;
    auto decimalToAny(
      const decimal_t &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToBoolean(
      const LSPAny &any
    ) const -> bool;
    auto booleanToAny(
      const boolean_t &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNull(
      const LSPAny &any
    ) const -> std::nullptr_t;
    auto nullToAny(
      const null_t &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToString(
      const LSPAny &any
    ) const -> std::string;
    auto stringToAny(
      const string_t &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToURI(
      const LSPAny &any
    ) const -> string_t;
    auto uriToAny(
      const URI &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentUri(
      const LSPAny &any
    ) const -> string_t;
    auto documentUriToAny(
      const DocumentUri &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRegExp(
      const LSPAny &any
    ) const -> string_t;
    auto regExpToAny(
      const RegExp &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToRequestId(
      const LSPAny &any
    ) const -> RequestId;
    auto requestIdToAny(
      const RequestId &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMessageParams(
      const LSPAny &any
    ) const -> MessageParams;
    auto messageParamsToAny(
      const MessageParams &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToResponseId(
      const LSPAny &any
    ) const -> ResponseId;
    auto responseIdToAny(
      const ResponseId &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDefinition(
      const LSPAny &any
    ) const -> Definition;
    auto definitionToAny(
      const Definition &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDefinitionLink(
      const LSPAny &any
    ) const -> std::unique_ptr<LocationLink>;
    auto definitionLinkToAny(
      const DefinitionLink &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeclaration(
      const LSPAny &any
    ) const -> Declaration;
    auto declarationToAny(
      const Declaration &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDeclarationLink(
      const LSPAny &any
    ) const -> std::unique_ptr<LocationLink>;
    auto declarationLinkToAny(
      const DeclarationLink &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToInlineValue(
      const LSPAny &any
    ) const -> InlineValue;
    auto inlineValueToAny(
      const InlineValue &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentDiagnosticReport(
      const LSPAny &any
    ) const -> DocumentDiagnosticReport;
    auto documentDiagnosticReportToAny(
      const DocumentDiagnosticReport &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPrepareRenameResult_2(
      const LSPAny &any
    ) const -> std::unique_ptr<PrepareRenameResult_2>;
    auto prepareRenameResult_2ToAny(
      const PrepareRenameResult_2 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPrepareRenameResult_1(
      const LSPAny &any
    ) const -> std::unique_ptr<PrepareRenameResult_1>;
    auto prepareRenameResult_1ToAny(
      const PrepareRenameResult_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPrepareRenameResult(
      const LSPAny &any
    ) const -> PrepareRenameResult;
    auto prepareRenameResultToAny(
      const PrepareRenameResult &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentSelector(
      const LSPAny &any
    ) const -> std::vector<DocumentFilter>;
    auto documentSelectorToAny(
      const DocumentSelector &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToProgressToken(
      const LSPAny &any
    ) const -> ProgressToken;
    auto progressTokenToAny(
      const ProgressToken &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToChangeAnnotationIdentifier(
      const LSPAny &any
    ) const -> string_t;
    auto changeAnnotationIdentifierToAny(
      const ChangeAnnotationIdentifier &alias
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToWorkspaceDocumentDiagnosticReport(
      const LSPAny &any
    ) const -> WorkspaceDocumentDiagnosticReport;
    auto workspaceDocumentDiagnosticReportToAny(
      const WorkspaceDocumentDiagnosticReport &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentContentChangeEvent_1(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentContentChangeEvent_1>;
    auto textDocumentContentChangeEvent_1ToAny(
      const TextDocumentContentChangeEvent_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentContentChangeEvent_0(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentContentChangeEvent_0>;
    auto textDocumentContentChangeEvent_0ToAny(
      const TextDocumentContentChangeEvent_0 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentContentChangeEvent(
      const LSPAny &any
    ) const -> TextDocumentContentChangeEvent;
    auto textDocumentContentChangeEventToAny(
      const TextDocumentContentChangeEvent &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMarkedString_1(
      const LSPAny &any
    ) const -> std::unique_ptr<MarkedString_1>;
    auto markedString_1ToAny(
      const MarkedString_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToMarkedString(
      const LSPAny &any
    ) const -> MarkedString;
    auto markedStringToAny(
      const MarkedString &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToDocumentFilter(
      const LSPAny &any
    ) const -> DocumentFilter;
    auto documentFilterToAny(
      const DocumentFilter &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToGlobPattern(
      const LSPAny &any
    ) const -> GlobPattern;
    auto globPatternToAny(
      const GlobPattern &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentFilter_2(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentFilter_2>;
    auto textDocumentFilter_2ToAny(
      const TextDocumentFilter_2 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentFilter_1(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentFilter_1>;
    auto textDocumentFilter_1ToAny(
      const TextDocumentFilter_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentFilter_0(
      const LSPAny &any
    ) const -> std::unique_ptr<TextDocumentFilter_0>;
    auto textDocumentFilter_0ToAny(
      const TextDocumentFilter_0 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToTextDocumentFilter(
      const LSPAny &any
    ) const -> TextDocumentFilter;
    auto textDocumentFilterToAny(
      const TextDocumentFilter &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentFilter_2(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentFilter_2>;
    auto notebookDocumentFilter_2ToAny(
      const NotebookDocumentFilter_2 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentFilter_1(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentFilter_1>;
    auto notebookDocumentFilter_1ToAny(
      const NotebookDocumentFilter_1 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentFilter_0(
      const LSPAny &any
    ) const -> std::unique_ptr<NotebookDocumentFilter_0>;
    auto notebookDocumentFilter_0ToAny(
      const NotebookDocumentFilter_0 &structure
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToNotebookDocumentFilter(
      const LSPAny &any
    ) const -> NotebookDocumentFilter;
    auto notebookDocumentFilterToAny(
      const NotebookDocumentFilter &variant
    ) const -> std::unique_ptr<LSPAny>;
    auto anyToPattern(
      const LSPAny &any
    ) const -> string_t;
    auto patternToAny(
      const Pattern &alias
    ) const -> std::unique_ptr<LSPAny>;

    // ================= //
    // Incoming Requests //
    // ================= //

    auto asTextDocumentImplementationParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<ImplementationParams>;
    auto textDocumentImplementationResultToAny(
      const TextDocumentImplementationResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentTypeDefinitionParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<TypeDefinitionParams>;
    auto textDocumentTypeDefinitionResultToAny(
      const TextDocumentTypeDefinitionResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentDocumentColorParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentColorParams>;
    auto textDocumentDocumentColorResultToAny(
      const TextDocumentDocumentColorResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentColorPresentationParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<ColorPresentationParams>;
    auto textDocumentColorPresentationResultToAny(
      const TextDocumentColorPresentationResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentFoldingRangeParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<FoldingRangeParams>;
    auto textDocumentFoldingRangeResultToAny(
      const TextDocumentFoldingRangeResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentDeclarationParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DeclarationParams>;
    auto textDocumentDeclarationResultToAny(
      const TextDocumentDeclarationResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentSelectionRangeParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<SelectionRangeParams>;
    auto textDocumentSelectionRangeResultToAny(
      const TextDocumentSelectionRangeResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentPrepareCallHierarchyParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CallHierarchyPrepareParams>;
    auto textDocumentPrepareCallHierarchyResultToAny(
      const TextDocumentPrepareCallHierarchyResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asCallHierarchyIncomingCallsParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CallHierarchyIncomingCallsParams>;
    auto callHierarchyIncomingCallsResultToAny(
      const CallHierarchyIncomingCallsResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asCallHierarchyOutgoingCallsParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CallHierarchyOutgoingCallsParams>;
    auto callHierarchyOutgoingCallsResultToAny(
      const CallHierarchyOutgoingCallsResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentSemanticTokensFullParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<SemanticTokensParams>;
    auto textDocumentSemanticTokensFullResultToAny(
      const TextDocumentSemanticTokensFullResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentSemanticTokensFullDeltaParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<SemanticTokensDeltaParams>;
    auto textDocumentSemanticTokensFullDeltaResultToAny(
      const TextDocumentSemanticTokensFullDeltaResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentSemanticTokensRangeParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<SemanticTokensRangeParams>;
    auto textDocumentSemanticTokensRangeResultToAny(
      const TextDocumentSemanticTokensRangeResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentLinkedEditingRangeParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<LinkedEditingRangeParams>;
    auto textDocumentLinkedEditingRangeResultToAny(
      const TextDocumentLinkedEditingRangeResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asWorkspaceWillCreateFilesParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CreateFilesParams>;
    auto workspaceWillCreateFilesResultToAny(
      const WorkspaceWillCreateFilesResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asWorkspaceWillRenameFilesParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<RenameFilesParams>;
    auto workspaceWillRenameFilesResultToAny(
      const WorkspaceWillRenameFilesResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asWorkspaceWillDeleteFilesParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DeleteFilesParams>;
    auto workspaceWillDeleteFilesResultToAny(
      const WorkspaceWillDeleteFilesResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentMonikerParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<MonikerParams>;
    auto textDocumentMonikerResultToAny(
      const TextDocumentMonikerResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentPrepareTypeHierarchyParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<TypeHierarchyPrepareParams>;
    auto textDocumentPrepareTypeHierarchyResultToAny(
      const TextDocumentPrepareTypeHierarchyResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTypeHierarchySupertypesParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<TypeHierarchySupertypesParams>;
    auto typeHierarchySupertypesResultToAny(
      const TypeHierarchySupertypesResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTypeHierarchySubtypesParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<TypeHierarchySubtypesParams>;
    auto typeHierarchySubtypesResultToAny(
      const TypeHierarchySubtypesResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentInlineValueParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<InlineValueParams>;
    auto textDocumentInlineValueResultToAny(
      const TextDocumentInlineValueResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentInlayHintParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<InlayHintParams>;
    auto textDocumentInlayHintResultToAny(
      const TextDocumentInlayHintResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asInlayHintResolveParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<InlayHint>;
    auto inlayHintResolveResultToAny(
      const InlayHintResolveResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentDiagnosticParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentDiagnosticParams>;
    auto textDocumentDiagnosticResultToAny(
      const TextDocumentDiagnosticResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asWorkspaceDiagnosticParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<WorkspaceDiagnosticParams>;
    auto workspaceDiagnosticResultToAny(
      const WorkspaceDiagnosticResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentInlineCompletionParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<InlineCompletionParams>;
    auto textDocumentInlineCompletionResultToAny(
      const TextDocumentInlineCompletionResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asInitializeParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<InitializeParams>;
    auto shutdownResultToAny(
      const ShutdownResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentWillSaveWaitUntilParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<WillSaveTextDocumentParams>;
    auto textDocumentWillSaveWaitUntilResultToAny(
      const TextDocumentWillSaveWaitUntilResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentCompletionParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CompletionParams>;
    auto textDocumentCompletionResultToAny(
      const TextDocumentCompletionResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asCompletionItemResolveParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CompletionItem>;
    auto completionItemResolveResultToAny(
      const CompletionItemResolveResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentHoverParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<HoverParams>;
    auto textDocumentHoverResultToAny(
      const TextDocumentHoverResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentSignatureHelpParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<SignatureHelpParams>;
    auto textDocumentSignatureHelpResultToAny(
      const TextDocumentSignatureHelpResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentDefinitionParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DefinitionParams>;
    auto textDocumentDefinitionResultToAny(
      const TextDocumentDefinitionResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentReferencesParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<ReferenceParams>;
    auto textDocumentReferencesResultToAny(
      const TextDocumentReferencesResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentDocumentHighlightParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentHighlightParams>;
    auto textDocumentDocumentHighlightResultToAny(
      const TextDocumentDocumentHighlightResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentDocumentSymbolParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentSymbolParams>;
    auto textDocumentDocumentSymbolResultToAny(
      const TextDocumentDocumentSymbolResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentCodeActionParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CodeActionParams>;
    auto textDocumentCodeActionResultToAny(
      const TextDocumentCodeActionResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asCodeActionResolveParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CodeAction>;
    auto codeActionResolveResultToAny(
      const CodeActionResolveResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asWorkspaceSymbolParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<WorkspaceSymbolParams>;
    auto workspaceSymbolResultToAny(
      const WorkspaceSymbolResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asWorkspaceSymbolResolveParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<WorkspaceSymbol>;
    auto workspaceSymbolResolveResultToAny(
      const WorkspaceSymbolResolveResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentCodeLensParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CodeLensParams>;
    auto textDocumentCodeLensResultToAny(
      const TextDocumentCodeLensResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asCodeLensResolveParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<CodeLens>;
    auto codeLensResolveResultToAny(
      const CodeLensResolveResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentDocumentLinkParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentLinkParams>;
    auto textDocumentDocumentLinkResultToAny(
      const TextDocumentDocumentLinkResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asDocumentLinkResolveParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentLink>;
    auto documentLinkResolveResultToAny(
      const DocumentLinkResolveResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentFormattingParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentFormattingParams>;
    auto textDocumentFormattingResultToAny(
      const TextDocumentFormattingResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentRangeFormattingParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentRangeFormattingParams>;
    auto textDocumentRangeFormattingResultToAny(
      const TextDocumentRangeFormattingResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentRangesFormattingParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentRangesFormattingParams>;
    auto textDocumentRangesFormattingResultToAny(
      const TextDocumentRangesFormattingResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentOnTypeFormattingParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<DocumentOnTypeFormattingParams>;
    auto textDocumentOnTypeFormattingResultToAny(
      const TextDocumentOnTypeFormattingResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentRenameParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<RenameParams>;
    auto textDocumentRenameResultToAny(
      const TextDocumentRenameResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asTextDocumentPrepareRenameParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<PrepareRenameParams>;
    auto textDocumentPrepareRenameResultToAny(
      const TextDocumentPrepareRenameResult &result
    ) -> std::unique_ptr<LSPAny>;
    auto asWorkspaceExecuteCommandParams(
      const MessageParams &requestParams
    ) const -> std::unique_ptr<ExecuteCommandParams>;
    auto workspaceExecuteCommandResultToAny(
      const WorkspaceExecuteCommandResult &result
    ) -> std::unique_ptr<LSPAny>;

    // ====================== //
    // Incoming Notifications //
    // ====================== //

    auto asWorkspaceDidChangeWorkspaceFoldersParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidChangeWorkspaceFoldersParams>;
    auto asWindowWorkDoneProgressCancelParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<WorkDoneProgressCancelParams>;
    auto asWorkspaceDidCreateFilesParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<CreateFilesParams>;
    auto asWorkspaceDidRenameFilesParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<RenameFilesParams>;
    auto asWorkspaceDidDeleteFilesParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DeleteFilesParams>;
    auto asNotebookDocumentDidOpenParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidOpenNotebookDocumentParams>;
    auto asNotebookDocumentDidChangeParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidChangeNotebookDocumentParams>;
    auto asNotebookDocumentDidSaveParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidSaveNotebookDocumentParams>;
    auto asNotebookDocumentDidCloseParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidCloseNotebookDocumentParams>;
    auto asInitializedParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<InitializedParams>;
    auto asWorkspaceDidChangeConfigurationParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidChangeConfigurationParams>;
    auto asTextDocumentDidOpenParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidOpenTextDocumentParams>;
    auto asTextDocumentDidChangeParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidChangeTextDocumentParams>;
    auto asTextDocumentDidCloseParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidCloseTextDocumentParams>;
    auto asTextDocumentDidSaveParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidSaveTextDocumentParams>;
    auto asTextDocumentWillSaveParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<WillSaveTextDocumentParams>;
    auto asWorkspaceDidChangeWatchedFilesParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<DidChangeWatchedFilesParams>;
    auto asSetTraceParams(
      const MessageParams &notificationParams
    ) const -> std::unique_ptr<SetTraceParams>;

    // ================= //
    // Outgoing Requests //
    // ================= //

    auto asMessageParams(
      const ConfigurationParams &requestParams
    ) const -> MessageParams;
    auto asMessageParams(
      const WorkDoneProgressCreateParams &requestParams
    ) const -> MessageParams;
    auto asMessageParams(
      const ShowDocumentParams &requestParams
    ) const -> MessageParams;
    auto asMessageParams(
      const RegistrationParams &requestParams
    ) const -> MessageParams;
    auto asMessageParams(
      const UnregistrationParams &requestParams
    ) const -> MessageParams;
    auto asMessageParams(
      const ShowMessageRequestParams &requestParams
    ) const -> MessageParams;
    auto asMessageParams(
      const ApplyWorkspaceEditParams &requestParams
    ) const -> MessageParams;

    // ====================== //
    // Outgoing Notifications //
    // ====================== //

    auto asMessageParams(
      const ShowMessageParams &notificationParams
    ) const -> MessageParams;
    auto asMessageParams(
      const LogMessageParams &notificationParams
    ) const -> MessageParams;
    auto asMessageParams(
      const LSPAny &notificationParams
    ) const -> MessageParams;
    auto asMessageParams(
      const PublishDiagnosticsParams &notificationParams
    ) const -> MessageParams;
    auto asMessageParams(
      const LogTraceParams &notificationParams
    ) const -> MessageParams;

  }; // class LspTransformer

} // namespace LCompilers::LanguageServerProtocol
