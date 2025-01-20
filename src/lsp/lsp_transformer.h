#ifndef LCOMPILERS_LSP_TRANSFORMER_H
#define LCOMPILERS_LSP_TRANSFORMER_H

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

    auto uintegerToAny(
      uinteger value
    ) const -> std::unique_ptr<LSPAny>;
    auto stringToAny(
      const string &value
    ) const -> std::unique_ptr<LSPAny>;

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
