#include <cmath>
#include <format>

#include <lsp/lsp_exception.h>
#include <lsp/lsp_transformer.h>

namespace LCompilers::LanguageServerProtocol {

  auto LspTransformer::asInitializeParams(
    const RequestParams &requestParams
  ) const -> InitializeParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::WILL_SAVE_WAIT_UNTIL
      ),
      requestParams,
      RequestParamsType::LSP_OBJECT
    );

    InitializeParams initializeParams;

    LSPObject &lspObject =
      *std::get<std::unique_ptr<LSPObject>>(requestParams.value);

    auto iter = lspObject.find("capabilities");
    if (iter != lspObject.end()) {
      initializeParams.capabilities = anyToClientCapabilities(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required attribute: capabilities"
      );
    }

    return initializeParams;
  }

  auto LspTransformer::asWillSaveTextDocumentParams(
    const RequestParams &requestParams
  ) const -> WillSaveTextDocumentParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::WILL_SAVE_WAIT_UNTIL
      ),
      requestParams,
      RequestParamsType::LSP_OBJECT
    );

    WillSaveTextDocumentParams willSaveTextDocumentParams;

    LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(requestParams.value);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      willSaveTextDocumentParams.textDocument =
        anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required attribute: textDocument"
      );
    }

    iter = object.find("reason");
    if (iter != object.end()) {
      willSaveTextDocumentParams.reason =
        anyToTextDocumentSaveReason(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required attribute: reason"
      );
    }

    return willSaveTextDocumentParams;
  }

  auto LspTransformer::asDeclarationParams(
    const RequestParams &requestParams
  ) const -> DeclarationParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::GOTO_DECLARATION
      ),
      requestParams,
      RequestParamsType::LSP_OBJECT
    );

    DeclarationParams params;

    LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(requestParams.value);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DeclarationParams attribute: textDocument"
      );
    }

    iter = object.find("position");
    if (iter != object.end()) {
      params.position = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DeclarationParams attribute: position"
      );
    }

    iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("partialResultToken");
    if (iter != object.end()) {
      params.partialResultToken = anyToProgressToken(*iter->second);
    }

    return params;
  }

  auto LspTransformer::asDefinitionParams(
    const RequestParams &requestParams
  ) const -> DefinitionParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::GOTO_DEFINITION
      ),
      requestParams,
      RequestParamsType::LSP_OBJECT
    );

    DefinitionParams params;

    LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(requestParams.value);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DefinitionParams attribute: textDocument"
      );
    }

    iter = object.find("position");
    if (iter != object.end()) {
      params.position = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DefinitionParams attribute: position"
      );
    }

    iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("partialResultToken");
    if (iter != object.end()) {
      params.partialResultToken = anyToProgressToken(*iter->second);
    }

    return params;
  }

  auto LspTransformer::anyToVersionedNotebookDocumentIdentifier(
    const LSPAny &any
  ) const -> std::unique_ptr<VersionedNotebookDocumentIdentifier> {
    assertAnyType("VersionedNotebookDocumentIdentifier", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<VersionedNotebookDocumentIdentifier> documentId =
      std::make_unique<VersionedNotebookDocumentIdentifier>();

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("version");
    if (iter != object.end()) {
      documentId->version = anyToInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required VersionedNotebookDocumentIdentifier attribute: version"
      );
    }

    iter = object.find("uri");
    if (iter != object.end()) {
      documentId->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required VersionedNotebookDocumentIdentifier attribute: uri"
      );
    }

    return documentId;
  }

  auto LspTransformer::anyToNotebookDocumentChangeEventCellsStructure(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookDocumentChangeEventCellsStructure> {
    assertAnyType("NotebookDocumentChangeEventCellsStructure", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<NotebookDocumentChangeEventCellsStructure> structure =
      std::make_unique<NotebookDocumentChangeEventCellsStructure>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("array");
    if (iter != object.end()) {
      structure->array = anyToNotebookCellArrayChange(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookDocumentChangeEventCellsStructure attribute: array"
      );
    }

    iter = object.find("didOpen");
    if (iter != object.end()) {
      const LSPAny &anyDidOpen = *iter->second;
      assertAnyType(
        "NotebookDocumentChangeEventCellsStructure",
        anyDidOpen, LSPAnyType::LSP_ARRAY
      );
      ptr_vector<TextDocumentItem> didOpen;
      for (const std::unique_ptr<LSPAny> &item
             : *std::get<std::unique_ptr<LSPArray>>(anyDidOpen.value)) {
        didOpen.push_back(anyToTextDocumentItem(*item));
      }
      structure->didOpen = std::move(didOpen);
    }

    iter = object.find("didClose");
    if (iter != object.end()) {
      const LSPAny &anyDidClose = *iter->second;
      assertAnyType(
        "NotebookDocumentChangeEventCellsStructure",
        anyDidClose, LSPAnyType::LSP_ARRAY
      );
      ptr_vector<TextDocumentIdentifier> didClose;
      for (const std::unique_ptr<LSPAny> &item
             : *std::get<std::unique_ptr<LSPArray>>(anyDidClose.value)) {
        didClose.push_back(anyToTextDocumentIdentifier(*item));
      }
      structure->didClose = std::move(didClose);
    }

    return structure;
  }

  auto LspTransformer::anyToProgressToken(
    const LSPAny &any
  ) const -> std::unique_ptr<ProgressToken> {
    std::unique_ptr<ProgressToken> token =
      std::make_unique<ProgressToken>();

    switch (any.type) {
    case LSPAnyType::LSP_INTEGER: {
      token->type = ProgressTokenType::LSP_INTEGER;
      token->value = std::get<int>(any.value);
      break;
    }
    case LSPAnyType::LSP_STRING: {
      token->type = ProgressTokenType::LSP_STRING;
      token->value = std::get<std::string>(any.value);
      break;
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAnyType for a(n) {} must be of type {} or {} but received type {}",
          "ProgressToken",
          static_cast<int>(LSPAnyType::LSP_INTEGER),
          static_cast<int>(LSPAnyType::LSP_STRING),
          static_cast<int>(any.type)
        )
      );
    }
    }

    return token;
  }

  auto LspTransformer::anyToNotebookCellArrayChange(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookCellArrayChange> {
    assertAnyType("NotebookCellArrayChange", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<NotebookCellArrayChange> change =
      std::make_unique<NotebookCellArrayChange>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("start");
    if (iter != object.end()) {
      change->start = anyToUnsignedInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookCellArrayChange attribute: start"
      );
    }

    iter = object.find("deleteCount");
    if (iter != object.end()) {
      change->deleteCount = anyToUnsignedInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookCellArrayChange attribute: deleteCount"
      );
    }

    iter = object.find("cells");
    if (iter != object.end()) {
      const LSPAny &anyCells = *iter->second;
      assertAnyType("std::vector", anyCells, LSPAnyType::LSP_ARRAY);
      ptr_vector<NotebookCell> cells;
      for (const std::unique_ptr<LSPAny> &cell
             : *std::get<std::unique_ptr<LSPArray>>(anyCells.value)) {
        cells.push_back(anyToNotebookCell(*cell));
      }
      change->cells = std::move(cells);
    }

    return change;
  }

  auto LspTransformer::anyToNotebookDocumentIdentifier(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookDocumentIdentifier> {
    assertAnyType("NotebookDocumentIdentifier", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<NotebookDocumentIdentifier> notebookId =
      std::make_unique<NotebookDocumentIdentifier>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("uri");
    if (iter != object.end()) {
      notebookId->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookDocumentIdentifier attribute: uri"
      );
    }

    return notebookId;
  }

  auto LspTransformer::anyToNotebookDocumentChangeEventCellsTextContent(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookDocumentChangeEventCellsTextContent> {
    assertAnyType("NotebookDocumentChangeEventCellsTextContent", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<NotebookDocumentChangeEventCellsTextContent> content =
      std::make_unique<NotebookDocumentChangeEventCellsTextContent>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("document");
    if (iter != object.end()) {
      content->document = anyToVersionedTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookDocumentChangeEventCellsTextContent attribute: document"
      );
    }

    iter = object.find("changes");
    if (iter != object.end()) {
      LSPAny &anyData = *iter->second;
      assertAnyType("std::vector", any, LSPAnyType::LSP_ARRAY);
      ptr_vector<TextDocumentContentChangeEvent> changes;
      for (const std::unique_ptr<LSPAny> &change
             : *std::get<std::unique_ptr<LSPArray>>(anyData.value)) {
        changes.push_back(anyToTextDocumentContentChangeEvent(*change));
      }
      content->changes = std::move(changes);
    }

    return content;
  }

  auto LspTransformer::anyToNotebookDocumentChangeEventCells(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookDocumentChangeEventCells> {
    assertAnyType("NotebookDocumentChangeEventCells", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<NotebookDocumentChangeEventCells> cells =
      std::make_unique<NotebookDocumentChangeEventCells>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("structure");
    if (iter != object.end()) {
      cells->structure =
        anyToNotebookDocumentChangeEventCellsStructure(*iter->second);
    }

    iter = object.find("data");
    if (iter != object.end()) {
      LSPAny &anyData = *iter->second;
      assertAnyType("std::vector", any, LSPAnyType::LSP_ARRAY);
      ptr_vector<NotebookCell> data;
      for (const std::unique_ptr<LSPAny> &cell
             : *std::get<std::unique_ptr<LSPArray>>(anyData.value)) {
        data.push_back(anyToNotebookCell(*cell));
      }
      cells->data = std::move(data);
    }

    iter = object.find("textContent");
    if (iter != object.end()) {
      LSPAny &anyData = *iter->second;
      assertAnyType("std::vector", any, LSPAnyType::LSP_ARRAY);
      ptr_vector<NotebookDocumentChangeEventCellsTextContent> textContent;
      for (const std::unique_ptr<LSPAny> &content
             : *std::get<std::unique_ptr<LSPArray>>(anyData.value)) {
        textContent.push_back(
          anyToNotebookDocumentChangeEventCellsTextContent(*content)
        );
      }
      cells->textContent = std::move(textContent);
    }

    return cells;
  }

  auto LspTransformer::anyToNotebookDocumentChangeEvent(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookDocumentChangeEvent> {
    assertAnyType("NotebookDocumentChangeEvent", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<NotebookDocumentChangeEvent> event =
      std::make_unique<NotebookDocumentChangeEvent>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("metadata");
    if (iter != object.end()) {
      const LSPAny &any = *iter->second;
      assertAnyType("NotebookDocumentChangeEvent.metadata", any, LSPAnyType::LSP_OBJECT);
      const std::unique_ptr<LSPObject> &object =
        std::get<std::unique_ptr<LSPObject>>(any.value);
      event->metadata = copy(object);
    }

    iter = object.find("cells");
    if (iter != object.end()) {
      event->cells = anyToNotebookDocumentChangeEventCells(*iter->second);
    }

    return event;
  }

  auto LspTransformer::anyToExecutionSummary(
    const LSPAny &any
  ) const -> std::unique_ptr<ExecutionSummary> {
    assertAnyType("ExecutionSummary", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<ExecutionSummary> executionSummary =
      std::make_unique<ExecutionSummary>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("executionOrder");
    if (iter != object.end()) {
      executionSummary->executionOrder = anyToUnsignedInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ExecutionSummary attribute: executionOrder"
      );
    }

    iter = object.find("success");
    if (iter != object.end()) {
      executionSummary->success = anyToBool(*iter->second);
    }

    return executionSummary;
  }

  auto LspTransformer::anyToNotebookCell(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookCell> {
    assertAnyType("NotebookCell", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<NotebookCell> notebookCell =
      std::make_unique<NotebookCell>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("kind");
    if (iter != object.end()) {
      int value = anyToInt(*iter->second);
      try {
        notebookCell->kind = notebookCellKindByValue(value);
      } catch (std::invalid_argument &e) {
        throw LspException(
          ErrorCodes::InvalidParams,
          e.what()
        );
      }
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookCell attribute: kind"
      );
    }

    iter = object.find("document");
    if (iter != object.end()) {
      notebookCell->document = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookCell attribute: document"
      );
    }

    iter = object.find("metadata");
    if (iter != object.end()) {
      const LSPAny &any = *iter->second;
      assertAnyType("NotebookCell.metadata", any, LSPAnyType::LSP_OBJECT);
      const std::unique_ptr<LSPObject> &object =
        std::get<std::unique_ptr<LSPObject>>(any.value);
      notebookCell->metadata = copy(object);
    }

    iter = object.find("executionSummary");
    if (iter != object.end()) {
      notebookCell->executionSummary = anyToExecutionSummary(*iter->second);
    }

    return notebookCell;
  }

  auto LspTransformer::anyToNotebookDocument(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookDocument> {
    assertAnyType("NotebookDocument", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<NotebookDocument> notebookDocument =
      std::make_unique<NotebookDocument>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("uri");
    if (iter != object.end()) {
      notebookDocument->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookDocument attribute: uri"
      );
    }

    iter = object.find("notebookType");
    if (iter != object.end()) {
      notebookDocument->notebookType = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookDocument attribute: notebookType"
      );
    }

    iter = object.find("version");
    if (iter != object.end()) {
      notebookDocument->version = anyToInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookDocument attribute: version"
      );
    }

    iter = object.find("metadata");
    if (iter != object.end()) {
      const LSPAny &any = *iter->second;
      assertAnyType("NotebookDocument.metadata", any, LSPAnyType::LSP_OBJECT);
      const std::unique_ptr<LSPObject> &object =
        std::get<std::unique_ptr<LSPObject>>(any.value);
      notebookDocument->metadata = copy(object);
    }

    iter = object.find("cells");
    if (iter != object.end()) {
      const LSPAny &any = *iter->second;
      assertAnyType("NotebookDocument.cells", any, LSPAnyType::LSP_ARRAY);
      const std::unique_ptr<LSPArray> &array =
        std::get<std::unique_ptr<LSPArray>>(any.value);
      for (const std::unique_ptr<LSPAny> &elem : *array) {
        notebookDocument->cells.push_back(anyToNotebookCell(*elem));
      }
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookDocument attribute: cells"
      );
    }

    return notebookDocument;
  }

  auto LspTransformer::anyToTraceValue(
    const LSPAny &any
  ) const -> TraceValue {
    assertAnyType("TraceValue", any, LSPAnyType::LSP_STRING);
    const std::string &value = std::get<std::string>(any.value);
    try {
      return traceValueByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(
        ErrorCodes::InvalidParams,
        e.what()
      );
    }
  }

  auto LspTransformer::anyToTextDocumentSaveReason(
    const LSPAny &any
  ) const -> TextDocumentSaveReason {
    assertAnyType("TextDocumentSaveReason", any, LSPAnyType::LSP_INTEGER);
    int value = std::get<int>(any.value);
    try {
      return textDocumentSaveReasonByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(
        ErrorCodes::InvalidParams,
        e.what()
      );
    }
  }

  auto LspTransformer::anyToRequestId(
    const LSPAny &any
  ) const -> std::unique_ptr<RequestId> {
    std::unique_ptr<RequestId> requestId = std::make_unique<RequestId>();
    switch (any.type) {
    case LSPAnyType::LSP_INTEGER: {
      requestId->type = RequestIdType::LSP_INTEGER;
      requestId->value = std::get<int>(any.value);
      break;
    }
    case LSPAnyType::LSP_STRING: {
      requestId->type = RequestIdType::LSP_STRING;
      requestId->value = std::get<std::string>(any.value);
      break;
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAnyType for a(n) {} must be of type {} or {} but received type {}",
          "RequestId",
          static_cast<int>(LSPAnyType::LSP_INTEGER),
          static_cast<int>(LSPAnyType::LSP_STRING),
          static_cast<int>(any.type)
        )
      );
    }
    }
    return requestId;
  }

  auto LspTransformer::anyToClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ClientCapabilities> {
    std::unique_ptr<ClientCapabilities> capabilities =
      std::make_unique<ClientCapabilities>();
    // FIXME: Finish filling this in ...
    return capabilities;
  }

  auto LspTransformer::anyToTextDocumentItem(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentItem> {
    assertAnyType("TextDocumentItem", any, LSPAnyType::LSP_OBJECT);
    LSPObject &value = *std::get<std::unique_ptr<LSPObject>>(any.value);
    std::unique_ptr<TextDocumentItem> textDocumentItem =
      std::make_unique<TextDocumentItem>();

    auto iter = value.find("uri");
    if (iter != value.end()) {
      textDocumentItem->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TextDocumentItem attribute: uri"
      );
    }

    iter = value.find("languageId");
    if (iter != value.end()) {
      textDocumentItem->languageId = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TextDocumentItem attribute: languageId"
      );
    }

    iter = value.find("version");
    if (iter != value.end()) {
      textDocumentItem->version = anyToInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TextDocumentItem attribute: version"
      );
    }

    iter = value.find("text");
    if (iter != value.end()) {
      textDocumentItem->text = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TextDocumentItem attribute: text"
      );
    }

    return textDocumentItem;
  }

  auto LspTransformer::anyToTextDocumentIdentifier(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentIdentifier> {
    assertAnyType(
      "TextDocumentIdentifier",
      any, LSPAnyType::LSP_OBJECT
    );
    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);
    std::unique_ptr<TextDocumentIdentifier> textDocumentId =
      std::make_unique<TextDocumentIdentifier>();

    auto iter = object.find("uri");
    if (iter != object.end()) {
      textDocumentId->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TextDocumentIdentifier attribute: uri"
      );
    }

    return textDocumentId;
  }

  auto LspTransformer::anyToVersionedTextDocumentIdentifier(
    const LSPAny &any
  ) const -> std::unique_ptr<VersionedTextDocumentIdentifier> {
    assertAnyType(
      "VersionedTextDocumentIdentifier",
      any, LSPAnyType::LSP_OBJECT
    );

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);
    std::unique_ptr<VersionedTextDocumentIdentifier> textDocumentId =
      std::make_unique<VersionedTextDocumentIdentifier>();

    auto iter = object.find("uri");
    if (iter != object.end()) {
      textDocumentId->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required VersionedTextDocumentIdentifier attribute: uri"
      );
    }

    iter = object.find("version");
    if (iter != object.end()) {
      textDocumentId->version = anyToInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required VersionedTextDocumentIdentifier attribute: version"
      );
    }

    return textDocumentId;
  }

  auto LspTransformer::anyToTextDocumentContentChangeEvent(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentContentChangeEvent> {
    assertAnyType("TextDocumentContentChangeEvent", any, LSPAnyType::LSP_OBJECT);
    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    std::unique_ptr<TextDocumentContentChangeEvent> change =
      std::make_unique<TextDocumentContentChangeEvent>();

    auto iter = object.find("range");
    if (iter != object.end()) {
      change->type = TextDocumentContentChangeEventType::PARTIAL_TEXT_DOCUMENT;

      std::unique_ptr<PartialTextDocumentContentChangeEvent> value =
        std::make_unique<PartialTextDocumentContentChangeEvent>();
      value->range = anyToRange(*iter->second);

      iter = object.find("text");
      if (iter != object.end()) {
        value->text = anyToString(*iter->second);
      } else {
        throw LspException(
          ErrorCodes::InvalidParams,
          "Missing required TextDocumentContentChangeEvent attribute: text"
        );
      }

      iter = object.find("rangeLength");
      if (iter != object.end()) {
        value->rangeLength = anyToInt(*iter->second);
      }

      change->value = std::move(value);
    } else {
      change->type = TextDocumentContentChangeEventType::WHOLE_TEXT_DOCUMENT;

      std::unique_ptr<WholeTextDocumentContentChangeEvent> value =
        std::make_unique<WholeTextDocumentContentChangeEvent>();
      iter = object.find("text");
      if (iter != object.end()) {
        value->text = anyToString(*iter->second);
      } else {
        throw LspException(
          ErrorCodes::InvalidParams,
          "Missing required TextDocumentContentChangeEvent attribute: text"
        );
      }

      change->value = std::move(value);
    }

    return change;
  }

  auto LspTransformer::anyToRange(
    const LSPAny &any
  ) const -> std::unique_ptr<Range> {
    assertAnyType("Range", any, LSPAnyType::LSP_OBJECT);
    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    std::unique_ptr<Range> range = std::make_unique<Range>();

    auto iter = object.find("start");
    if (iter != object.end()) {
      range->start = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required Range attribute: start"
      );
    }

    iter = object.find("end");
    if (iter != object.end()) {
      range->end = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required Range attribute: end"
      );
    }

    return range;
  }

  auto LspTransformer::anyToPosition(
    const LSPAny &any
  ) const -> std::unique_ptr<Position> {
    assertAnyType("Position", any, LSPAnyType::LSP_OBJECT);
    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    std::unique_ptr<Position> position = std::make_unique<Position>();

    auto iter = object.find("line");
    if (iter != object.end()) {
      position->line = anyToUnsignedInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required Position attribute: line"
      );
    }

    iter = object.find("character");
    if (iter != object.end()) {
      position->character = anyToUnsignedInt(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required Position attribute: character"
      );
    }

    return position;
  }

  auto LspTransformer::anyToString(
    const LSPAny &any
  ) const -> const std::string & {
    assertAnyType("string", any, LSPAnyType::LSP_STRING);
    return std::get<std::string>(any.value);
  }

  auto LspTransformer::anyToInt(
    const LSPAny &any
  ) const -> int {
    switch (any.type) {
    case LSPAnyType::LSP_INTEGER: {
      return std::get<int>(any.value);
    }
    case LSPAnyType::LSP_UINTEGER: {
      unsigned int value = std::get<unsigned int>(any.value);
      return static_cast<int>(value);
    }
    case LSPAnyType::LSP_DECIMAL: {
      double value = std::get<double>(any.value);
      if (std::floor(value) != value) {
        throw LspException(
          ErrorCodes::InvalidParams,
          std::format(
            "loss of precision converting decimal to integer: {}",
            value
          )
        );
      }
      return static_cast<int>(value);
    }
    default: {
      // This will throw an exception
      assertAnyType("int", any, LSPAnyType::LSP_INTEGER);
    }
    }
    throw std::runtime_error(
      "LspTransformer::anyToInt: This should not be reached."
    );
  }

  auto LspTransformer::anyToUnsignedInt(
    const LSPAny &any
  ) const -> unsigned int {
    switch (any.type) {
    case LSPAnyType::LSP_UINTEGER: {
      return std::get<unsigned int>(any.value);
    }
    case LSPAnyType::LSP_INTEGER: {
      int value = std::get<int>(any.value);
      if (value < 0) {
        throw LspException(
          ErrorCodes::InvalidParams,
          std::format(
            "unsigned int must be greater than 0, but received: {}",
            value
          )
        );
      }
      return static_cast<unsigned int>(value);
    }
    case LSPAnyType::LSP_DECIMAL: {
      double value = std::get<double>(any.value);
      if ((std::floor(value) != value) || (value < 0.0)) {
        throw LspException(
          ErrorCodes::InvalidParams,
          std::format(
            "unsigned int must be a non-negative integer, but received: {}",
            value
          )
        );
      }
      return static_cast<unsigned int>(value);
    }
    default: {
      // This will throw an exception
      assertAnyType("unsigned int", any, LSPAnyType::LSP_UINTEGER);
    }
    }
    throw std::runtime_error(
      "LspTransformer::anyToUnsignedInt: This should not be reached."
    );
  }

  auto LspTransformer::anyToBool(
    const LSPAny &any
  ) const -> bool {
    assertAnyType("bool", any, LSPAnyType::LSP_BOOLEAN);
    return std::get<bool>(any.value);
  }

  auto LspTransformer::assertAnyType(
    const std::string &name,
    const LSPAny &any,
    LSPAnyType type
  ) const -> void {
    if (any.type != type) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAnyType for a(n) {} must be of type {} but received type {}",
          name,
          static_cast<int>(type),
          static_cast<int>(any.type)
        )
      );
    }
  }

  auto LspTransformer::assertRequestType(
    const std::string &method,
    const RequestParams &params,
    RequestParamsType type
  ) const -> void {
    if (params.type != type) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "RequestParamsType must be {} for method=\"{}\" but received type {}",
          static_cast<int>(type),
          method,
          static_cast<int>(params.type)
        )
      );
    }
  }

  auto LspTransformer::assertNotificationType(
    const std::string &method,
    const NotificationParams &params,
    NotificationParamsType type
  ) const -> void {
    if (params.type != type) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "NotificationParamsType must be {} for method=\"{}\" but received type {}",
          static_cast<int>(type),
          method,
          static_cast<int>(params.type)
        )
      );
    }
  }

  auto LspTransformer::asCancelParams(
    const NotificationParams &notificationParams
  ) const -> CancelParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::CANCEL_REQUEST
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    CancelParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    auto iter = object.find("id");
    if (iter != object.end()) {
      params.id = anyToRequestId(*iter->second);
    }

    return params;
  }

  auto LspTransformer::asSetTraceParams(
    const NotificationParams &notificationParams
  ) const -> SetTraceParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::SET_TRACE
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    SetTraceParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    auto iter = object.find("value");
    if (iter != object.end()) {
      params.value = anyToTraceValue(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required SetTraceParams attribute: value"
      );
    }

    return params;
  }

  auto LspTransformer::asInitializedParams(
    const optional_ptr<NotificationParams> &notificationParams
  ) const -> InitializedParams {
    InitializedParams params;
    // Nothing to initialize at the moment ...
    return params;
  }

  auto LspTransformer::asDidOpenNotebookDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidOpenNotebookDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_OPEN_NOTEBOOK_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    DidOpenNotebookDocumentParams params;

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    auto iter = value.find("notebookDocument");
    if (iter != value.end()) {
      params.notebookDocument = anyToNotebookDocument(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidOpenNotebookDocumentParams attribute: notebookDocument"
      );
    }

    iter = value.find("cellTextDocuments");
    if (iter != value.end()) {
      const LSPAny &anyDocs = *iter->second;
      assertAnyType("std::vector", anyDocs, LSPAnyType::LSP_ARRAY);
      for (const std::unique_ptr<LSPAny> &anyDoc
             : *std::get<std::unique_ptr<LSPArray>>(anyDocs.value)) {
        params.cellTextDocuments.push_back(anyToTextDocumentItem(*anyDoc));
      }
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidOpenNotebookDocumentParams attribute: cellTextDocuments"
      );
    }

    return params;
  }

  auto LspTransformer::asDidChangeNotebookDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidChangeNotebookDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_CHANGE_NOTEBOOK_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    DidChangeNotebookDocumentParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    auto iter = object.find("notebookDocument");
    if (iter != object.end()) {
      params.notebookDocument =
        anyToVersionedNotebookDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidChangeNotebookDocumentParams attribute: notebookDocument"
      );
    }

    iter = object.find("change");
    if (iter != object.end()) {
      params.change = anyToNotebookDocumentChangeEvent(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidChangeNotebookDocumentParams attribute: change"
      );
    }

    return params;
  }

  auto LspTransformer::asDidSaveNotebookDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidSaveNotebookDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_SAVE_NOTEBOOK_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    DidSaveNotebookDocumentParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    auto iter = object.find("notebookDocument");
    if (iter != object.end()) {
      params.notebookDocument = anyToNotebookDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidSaveNotebookDocumentParams attribute: notebookDocument"
      );
    }

    return params;
  }

  auto LspTransformer::asDidCloseNotebookDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidCloseNotebookDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_CLOSE_NOTEBOOK_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    DidCloseNotebookDocumentParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    auto iter = object.find("notebookDocument");
    if (iter != object.end()) {
      params.notebookDocument = anyToNotebookDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidCloseNotebookDocumentParams attribute: notebookDocument"
      );
    }

    iter = object.find("cellTextDocuments");
    if (iter != object.end()) {
      const LSPAny &anyCellTextDocuments = *iter->second;
      assertAnyType("std::vector", anyCellTextDocuments, LSPAnyType::LSP_ARRAY);
      ptr_vector<TextDocumentIdentifier> cellTextDocuments;
      for (const std::unique_ptr<LSPAny> &cellTextDocument
             : *std::get<std::unique_ptr<LSPArray>>(anyCellTextDocuments.value)) {
        cellTextDocuments.push_back(
          anyToTextDocumentIdentifier(*cellTextDocument)
        );
      }
      params.cellTextDocuments = std::move(cellTextDocuments);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidCloseNotebookDocumentParams attribute: cellTextDocuments"
      );
    }

    return params;
  }

  auto LspTransformer::asDidOpenTextDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidOpenTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_OPEN_TEXT_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    DidOpenTextDocumentParams params;

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    auto iter = value.find("textDocumentItem");
    if (iter != value.end()) {
      const LSPAny &textDocumentItem = *iter->second;
      params.textDocument = anyToTextDocumentItem(textDocumentItem);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidOpenTextDocumentParams attribute: textDocumentItem"
      );
    }

    return params;
  }

  auto LspTransformer::asDidChangeTextDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidChangeTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_CHANGE_TEXT_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    DidChangeTextDocumentParams params;

    auto iter = value.find("textDocument");
    if (iter != value.end()) {
      const LSPAny &textDocument = *iter->second;
      params.textDocument = anyToVersionedTextDocumentIdentifier(textDocument);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidChangeTextDocumentParams attribute: textDocument"
      );
    }

    iter = value.find("contentChanges");
    if (iter != value.end()) {
      const LSPAny &anyContentChanges = *iter->second;
      assertAnyType("std::vector", anyContentChanges, LSPAnyType::LSP_ARRAY);
      LSPArray &anyChanges =
        *std::get<std::unique_ptr<LSPArray>>(anyContentChanges.value);
      for (const auto &anyChange : anyChanges) {
        std::unique_ptr<TextDocumentContentChangeEvent> change =
          anyToTextDocumentContentChangeEvent(*anyChange);
        params.contentChanges.push_back(std::move(change));
      }
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidChangeTextDocumentParams attribute: contentChanges"
      );
    }

    return params;
  }

  auto LspTransformer::asDidSaveTextDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidSaveTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_SAVE_TEXT_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    DidSaveTextDocumentParams params;

    auto iter = value.find("textDocument");
    if (iter != value.end()) {
      const LSPAny &textDocument = *iter->second;
      params.textDocument = anyToTextDocumentIdentifier(textDocument);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidSaveTextDocumentParams attribute: textDocument"
      );
    }

    iter = value.find("text");
    if (iter != value.end()) {
      const LSPAny &text = *iter->second;
      params.text = anyToString(text);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidSaveTextDocumentParams attribute: text"
      );
    }

    return params;
  }

  auto LspTransformer::asDidCloseTextDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidCloseTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_CLOSE_TEXT_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);

    DidCloseTextDocumentParams params;

    auto iter = value.find("textDocument");
    if (iter != value.end()) {
      const LSPAny &textDocument = *iter->second;
      params.textDocument = anyToTextDocumentIdentifier(textDocument);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DidCloseTextDocumentParams attribute: textDocument"
      );
    }

    return params;
  }

  auto LspTransformer::lspToAny(
    const InitializeResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    std::unique_ptr<LSPObject> value = std::make_unique<LSPObject>();
    value->insert({"capabilities", lspToAny(*result.capabilities)});
    any->value = std::move(value);
    return any;
  }

  auto LspTransformer::lspToAny(
    const WillSaveWaitUntilResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    switch (result.type) {
    case WillSaveWaitUntilResultType::TEXT_EDITS: {
      any->type = LSPAnyType::LSP_ARRAY;
      std::unique_ptr<LSPArray> edits;
      for (const std::unique_ptr<TextEdit> &edit
             : std::get<ptr_vector<TextEdit>>(result.value)) {
        edits->push_back(lspToAny(*edit));
      }
      any->value = std::move(edits);
      break;
    }
    case WillSaveWaitUntilResultType::LSP_NULL: {
      any->type = LSPAnyType::LSP_NULL;
      any->value = nullptr;
      break;
    }
    }
    return any;
  }

  auto LspTransformer::lspToAny(
    const GotoDeclarationResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();

    switch (result.type) {
    case GotoDeclarationResultType::LOCATION: {
      any->type = LSPAnyType::LSP_OBJECT;
      any->value = lspToObject(
        *std::get<std::unique_ptr<Location>>(result.value)
      );
      break;
    }
    case GotoDeclarationResultType::LOCATION_ARRAY: {
      any->type = LSPAnyType::LSP_ARRAY;
      std::unique_ptr<LSPArray> locations = std::make_unique<LSPArray>();
      for (const std::unique_ptr<Location> &location
             : std::get<ptr_vector<Location>>(result.value)) {
        locations->push_back(lspToAny(*location));
      }
      any->value = std::move(locations);
      break;
    }
    case GotoDeclarationResultType::LOCATION_LINK_ARRAY: {
      any->type = LSPAnyType::LSP_ARRAY;
      std::unique_ptr<LSPArray> links = std::make_unique<LSPArray>();
      for (const std::unique_ptr<LocationLink> &link
             : std::get<ptr_vector<LocationLink>>(result.value)) {
        links->push_back(lspToAny(*link));
      }
      any->value = std::move(links);
      break;
    }
    case GotoDeclarationResultType::LSP_NULL: {
      any->type = LSPAnyType::LSP_NULL;
      any->value = nullptr;
      break;
    }
    }

    return any;
  }

  auto LspTransformer::lspToAny(
    const GotoDefinitionResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();

    switch (result.type) {
    case GotoDefinitionResultType::LOCATION: {
      any->type = LSPAnyType::LSP_OBJECT;
      any->value = lspToObject(
        *std::get<std::unique_ptr<Location>>(result.value)
      );
      break;
    }
    case GotoDefinitionResultType::LOCATION_ARRAY: {
      any->type = LSPAnyType::LSP_ARRAY;
      std::unique_ptr<LSPArray> locations = std::make_unique<LSPArray>();
      for (const std::unique_ptr<Location> &location
             : std::get<ptr_vector<Location>>(result.value)) {
        locations->push_back(lspToAny(*location));
      }
      any->value = std::move(locations);
      break;
    }
    case GotoDefinitionResultType::LOCATION_LINK_ARRAY: {
      any->type = LSPAnyType::LSP_ARRAY;
      std::unique_ptr<LSPArray> links = std::make_unique<LSPArray>();
      for (const std::unique_ptr<LocationLink> &link
             : std::get<ptr_vector<LocationLink>>(result.value)) {
        links->push_back(lspToAny(*link));
      }
      any->value = std::move(links);
      break;
    }
    case GotoDefinitionResultType::LSP_NULL: {
      any->type = LSPAnyType::LSP_NULL;
      any->value = nullptr;
      break;
    }
    }

    return any;
  }

  auto LspTransformer::lspToAny(
    const ServerCapabilities &capabilities
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    std::unique_ptr<LSPObject> value = std::make_unique<LSPObject>();
    if (capabilities.textDocumentSync.has_value()) {
      (*value)["textDocumentSync"] =
        lspToAny(*capabilities.textDocumentSync.value());
    }
    any->value = std::move(value);
    return any;
  }

  auto LspTransformer::lspToAny(
    const TextDocumentSync &textDocumentSync
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    switch (textDocumentSync.type) {
    case TextDocumentSyncType::TEXT_DOCUMENT_SYNC_KIND: {
      any->type = LSPAnyType::LSP_UINTEGER;
      any->value = static_cast<uinteger>(
        std::get<TextDocumentSyncKind>(textDocumentSync.value)
      );
      break;
    }
    case TextDocumentSyncType::TEXT_DOCUMENT_SYNC_OPTIONS: {
      any->type = LSPAnyType::LSP_OBJECT;
      any->value = lspToObject(
        *std::get<std::unique_ptr<TextDocumentSyncOptions>>(
          textDocumentSync.value
        )
      );
      break;
    }
    }
    return any;
  }

  auto LspTransformer::lspToAny(
    const TextEdit &edit
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    any->value = lspToObject(edit);
    return any;
  }

  auto LspTransformer::lspToAny(
    const LocationLink &link
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    any->value = lspToObject(link);
    return any;
  }

  auto LspTransformer::lspToAny(
    const Location &location
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    any->value = lspToObject(location);
    return any;
  }

  auto LspTransformer::lspToAny(
    const Range &range
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    any->value = lspToObject(range);
    return any;
  }

  auto LspTransformer::lspToAny(
    const Position &position
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    any->value = lspToObject(position);
    return any;
  }

  auto LspTransformer::uintegerToAny(
    uinteger value
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_UINTEGER;
    any->value = value;
    return any;
  }

  auto LspTransformer::stringToAny(
    const std::string &value
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_STRING;
    any->value = value;
    return any;
  }

  auto LspTransformer::lspToObject(
    const TextEdit &edit
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("range", lspToAny(*edit.range));
    object->emplace("newText", stringToAny(edit.newText));
    return object;
  }

  auto LspTransformer::lspToObject(
    const LocationLink &link
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    if (link.originSelectionRange.has_value()) {
      object->emplace(
        "originSelectionRange",
        lspToAny(*link.originSelectionRange.value())
      );
    }
    object->emplace("targetUri", stringToAny(link.targetUri));
    object->emplace("targetRange", lspToAny(*link.targetRange));
    object->emplace(
      "targetSelectionRange",
      lspToAny(*link.targetSelectionRange)
    );
    return object;
  }

  auto LspTransformer::lspToObject(
    const Location &location
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("uri", stringToAny(location.uri));
    object->emplace("range", lspToAny(location.range));
    return object;
  }

  auto LspTransformer::lspToObject(
    const Range &range
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("start", lspToAny(*range.start));
    object->emplace("end", lspToAny(*range.end));
    return object;
  }

  auto LspTransformer::lspToObject(
    const Position &position
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("line", uintegerToAny(position.line));
    object->emplace("character", uintegerToAny(position.character));
    return object;
  }

  auto LspTransformer::lspToObject(
    const TextDocumentSyncOptions &options
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    if (options.openClose.has_value()) {
      std::unique_ptr<LSPAny> openClose = std::make_unique<LSPAny>();
      openClose->type = LSPAnyType::LSP_BOOLEAN;
      openClose->value = options.openClose.value();
      object->insert({"openClose", std::move(openClose)});
    }
    if (options.change.has_value()) {
      std::unique_ptr<LSPAny> change = std::make_unique<LSPAny>();
      change->type = LSPAnyType::LSP_UINTEGER;
      change->value = static_cast<uinteger>(options.change.value());
      object->insert({"change", std::move(change)});
    }
    if (options.willSave.has_value()) {
      std::unique_ptr<LSPAny> willSave = std::make_unique<LSPAny>();
      willSave->type = LSPAnyType::LSP_BOOLEAN;
      willSave->value = options.willSave.value();
      object->insert({"willSave", std::move(willSave)});
    }
    if (options.willSaveWaitUntil.has_value()) {
      std::unique_ptr<LSPAny> willSaveWaitUntil = std::make_unique<LSPAny>();
      willSaveWaitUntil->type = LSPAnyType::LSP_BOOLEAN;
      willSaveWaitUntil->value = options.willSaveWaitUntil.value();
      object->insert({"willSaveWaitUntil", std::move(willSaveWaitUntil)});
    }
    if (options.save.has_value()) {
      std::unique_ptr<LSPAny> save = std::make_unique<LSPAny>();
      SaveOrOptions &saveOrOptions = *options.save.value();
      switch (saveOrOptions.type) {
      case SaveOrOptionsType::BOOLEAN: {
        save->type = LSPAnyType::LSP_BOOLEAN;
        save->value = std::get<boolean>(saveOrOptions.value);
        break;
      }
      case SaveOrOptionsType::SAVE_OPTIONS: {
        save->type = LSPAnyType::LSP_OBJECT;
        save->value = lspToObject(
          *std::get<std::unique_ptr<SaveOptions>>(saveOrOptions.value)
        );
        break;
      }
      }
      object->insert({"save", std::move(save)});
    }
    return object;
  }

  auto LspTransformer::lspToObject(
    const SaveOptions &options
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    if (options.includeText.has_value()) {
      std::unique_ptr<LSPAny> includeText = std::make_unique<LSPAny>();
      includeText->type = LSPAnyType::LSP_BOOLEAN;
      includeText->value = options.includeText.value();
      object->insert({"includeText", std::move(includeText)});
    }
    return object;
  }

  auto LspTransformer::requireRequestParams(
    const RequestMessage &request
  ) const -> const RequestParams & {
    if (request.params.has_value()) {
      return *request.params.value();
    }
    throw LspException(
      ErrorCodes::InvalidParams,
      std::format(
        "RequestMessage.params must be defined for method=\"{}\"",
        request.method
      )
    );
  }

  auto LspTransformer::requireNotificationParams(
    const NotificationMessage &notification
  ) const -> const NotificationParams & {
    if (notification.params.has_value()) {
      return *notification.params.value();
    }
    throw LspException(
      ErrorCodes::InvalidParams,
      std::format(
        "NotificationMessage.params must be defined for method=\"{}\"",
        notification.method
      )
    );
  }

  auto LspTransformer::copy(
    const std::unique_ptr<LSPAny> &any
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> clone = std::make_unique<LSPAny>();
    clone->type = any->type;
    switch (any->type) {
    case LSPAnyType::LSP_OBJECT: {
      clone->value = copy(std::get<std::unique_ptr<LSPObject>>(any->value));
      break;
    }
    case LSPAnyType::LSP_ARRAY: {
      clone->value = copy(std::get<std::unique_ptr<LSPArray>>(any->value));
      break;
    }
    case LSPAnyType::LSP_STRING: {
      clone->value = std::get<std::string>(any->value);
      break;
    }
    case LSPAnyType::LSP_INTEGER: {
      clone->value = std::get<int>(any->value);
      break;
    }
    case LSPAnyType::LSP_UINTEGER: {
      clone->value = std::get<unsigned int>(any->value);
      break;
    }
    case LSPAnyType::LSP_DECIMAL: {
      clone->value = std::get<double>(any->value);
      break;
    }
    case LSPAnyType::LSP_BOOLEAN: {
      clone->value = std::get<bool>(any->value);
      break;
    }
    case LSPAnyType::LSP_NULL: {
      clone->value = nullptr;
      break;
    }
    }
    return clone;
  }

  auto LspTransformer::copy(
    const std::unique_ptr<LSPObject> &object
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> clone = std::make_unique<LSPObject>();
    for (const auto &[key, value] : *object) {
      clone->emplace(key, copy(value));
    }
    return clone;
  }

  auto LspTransformer::copy(
    const std::unique_ptr<LSPArray> &array
  ) const -> std::unique_ptr<LSPArray> {
    std::unique_ptr<LSPArray> clone = std::make_unique<LSPArray>();
    for (const std::unique_ptr<LSPAny> &elem : *array) {
      clone->push_back(copy(elem));
    }
    return clone;
  }

} // namespace LCompilers::LanguageServerProtocol
