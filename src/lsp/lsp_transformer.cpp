#include <cmath>
#include <format>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/lsp_transformer.h>
#include <stdexcept>

namespace LCompilers::LanguageServerProtocol {

  auto LspTransformer::asInitializeParams(
    const MessageParams &requestParams
  ) const -> InitializeParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::WILL_SAVE_WAIT_UNTIL
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    InitializeParams initializeParams;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("workDoneToken");
    if (iter != object.end()) {
      initializeParams.workDoneToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("processId");
    if (iter != object.end()) {
      const LSPAny &processId = *iter->second;
      switch (static_cast<LSPAnyType>(processId.value.index())) {
      case LSPAnyType::LSP_INTEGER: {
        initializeParams.processId = anyToInt(processId);
        break;
      }
      case LSPAnyType::LSP_NULL: {
        // do nothing
        break;
      }
      default: {
        // Raise an exception
        assertAnyType(
          "InitializeParams.processId",
          processId, LSPAnyType::LSP_INTEGER
        );
      }
      }
    }

    iter = object.find("clientInfo");
    if (iter != object.end()) {
      initializeParams.clientInfo = anyToClientInfo(*iter->second);
    }

    iter = object.find("locale");
    if (iter != object.end()) {
      initializeParams.locale = anyToString(*iter->second);
    }

    iter = object.find("rootPath");
    if (iter != object.end()) {
      const LSPAny &rootPath = *iter->second;
      switch (static_cast<LSPAnyType>(rootPath.value.index())) {
      case LSPAnyType::LSP_STRING: {
        initializeParams.rootPath = anyToString(*iter->second);
        break;
      }
      case LSPAnyType::LSP_NULL: {
        // do nothing
        break;
      }
      default: {
        // Raise an exception
        assertAnyType(
          "InitializeParams.rootPath",
          rootPath, LSPAnyType::LSP_STRING
        );
      }
      }
    }

    iter = object.find("rootUri");
    if (iter != object.end()) {
      const LSPAny &rootUri = *iter->second;
      switch (static_cast<LSPAnyType>(rootUri.value.index())) {
      case LSPAnyType::LSP_STRING: {
        initializeParams.rootUri = anyToString(rootUri);
        break;
      }
      case LSPAnyType::LSP_NULL: {
        // do nothing
        break;
      }
      default: {
        // Raise an exception
        assertAnyType(
          "InitializeParams.rootUri",
          rootUri, LSPAnyType::LSP_STRING
        );
      }
      }
    }

    iter = object.find("initializationOptions");
    if (iter != object.end()) {
      initializeParams.initializationOptions = copy(iter->second);
    }

    iter = object.find("capabilities");
    if (iter != object.end()) {
      initializeParams.capabilities = anyToClientCapabilities(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required attribute: capabilities"
      );
    }

    iter = object.find("trace");
    if (iter != object.end()) {
      initializeParams.trace = anyToTraceValue(*iter->second);
    }

    iter = object.find("workspaceFolders");
    if (iter != object.end()) {
      const LSPAny &workspaceFolders = *iter->second;
      switch (static_cast<LSPAnyType>(workspaceFolders.value.index())) {
      case LSPAnyType::LSP_ARRAY: {
        ptr_vector<WorkspaceFolder> folders;
        for (const std::unique_ptr<LSPAny> &folder
               : *std::get<std::unique_ptr<LSPArray>>(workspaceFolders.value)) {
          folders.push_back(anyToWorkspaceFolder(*folder));
        }
        initializeParams.workspaceFolders = std::move(folders);
        break;
      }
      case LSPAnyType::LSP_NULL: {
        // do nothing
        break;
      }
      default: {
        // Raise an exception
        assertAnyType(
          "InitializeParams.workspaceFolders",
          workspaceFolders, LSPAnyType::LSP_ARRAY
        );
      }
      }
    }

    return initializeParams;
  }

  auto LspTransformer::asWillSaveTextDocumentParams(
    const MessageParams &requestParams
  ) const -> WillSaveTextDocumentParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::WILL_SAVE_WAIT_UNTIL
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    WillSaveTextDocumentParams willSaveTextDocumentParams;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

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
    const MessageParams &requestParams
  ) const -> DeclarationParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::GOTO_DECLARATION
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    DeclarationParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

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
    const MessageParams &requestParams
  ) const -> DefinitionParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::GOTO_DEFINITION
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    DefinitionParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

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

  auto LspTransformer::asTypeDefinitionParams(
    const MessageParams &requestParams
  ) const -> TypeDefinitionParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::GOTO_TYPE_DEFINITION
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    TypeDefinitionParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeDefinitionParams attribute: textDocument"
      );
    }

    iter = object.find("position");
    if (iter != object.end()) {
      params.position = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeDefinitionParams attribute: position"
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

  auto LspTransformer::asImplementationParams(
    const MessageParams &requestParams
  ) const -> ImplementationParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::GOTO_IMPLEMENTATION
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    ImplementationParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ImplementationParams attribute: textDocument"
      );
    }

    iter = object.find("position");
    if (iter != object.end()) {
      params.position = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ImplementationParams attribute: position"
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

  auto LspTransformer::asReferenceParams(
    const MessageParams &requestParams
  ) const -> ReferenceParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::FIND_REFERENCES
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    ReferenceParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ReferenceParams attribute: textDocument"
      );
    }

    iter = object.find("position");
    if (iter != object.end()) {
      params.position = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ReferenceParams attribute: position"
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

    iter = object.find("context");
    if (iter != object.end()) {
      params.context = anyToReferenceContext(*iter->second);
    }

    return params;
  }

  auto LspTransformer::asCallHierarchyPrepareParams(
    const MessageParams &requestParams
  ) const -> CallHierarchyPrepareParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::PREPARE_CALL_HIERARCHY
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    CallHierarchyPrepareParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyPrepareParams attribute: textDocument"
      );
    }

    iter = object.find("position");
    if (iter != object.end()) {
      params.position = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyPrepareParams attribute: position"
      );
    }

    iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    return params;
  }

  auto LspTransformer::asCallHierarchyIncomingCallsParams(
    const MessageParams &requestParams
  ) const -> CallHierarchyIncomingCallsParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::CALL_HIERARCHY_INCOMING_CALLS
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    CallHierarchyIncomingCallsParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("partialResultToken");
    if (iter != object.end()) {
      params.partialResultToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("item");
    if (iter != object.end()) {
      params.item = anyToCallHierarchyItem(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyPrepareParams attribute: item"
      );
    }

    return params;
  }

  auto LspTransformer::anyToCallHierarchyItem(
    const LSPAny &any
  ) const -> std::unique_ptr<CallHierarchyItem> {
    assertAnyType("CallHierarchyItem", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<CallHierarchyItem> item =
      std::make_unique<CallHierarchyItem>();

    const LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("name");
    if (iter != object.end()) {
      item->name = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyItem attribute: name"
      );
    }

    iter = object.find("kind");
    if (iter != object.end()) {
      item->kind = anyToSymbolKind(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyItem attribute: kind"
      );
    }

    iter = object.find("tags");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("CallHierarchyItem::tags", array, LSPAnyType::LSP_ARRAY);
      std::vector<SymbolTag> tags;
      for (const std::unique_ptr<LSPAny> &tag
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        tags.push_back(anyToSymbolTag(*tag));
      }
      item->tags = std::move(tags);
    }

    iter = object.find("detail");
    if (iter != object.end()) {
      item->detail = anyToString(*iter->second);
    }

    iter = object.find("uri");
    if (iter != object.end()) {
      item->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyItem attribute: uri"
      );
    }

    iter = object.find("range");
    if (iter != object.end()) {
      item->range = anyToRange(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyItem attribute: range"
      );
    }

    iter = object.find("selectionRange");
    if (iter != object.end()) {
      item->selectionRange = anyToRange(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyItem attribute: selectionRange"
      );
    }

    iter = object.find("data");
    if (iter != object.end()) {
      item->data = copy(iter->second);
    }

    return item;
  }

  auto LspTransformer::lspToAny(
    const CallHierarchyIncomingCallsResult &result
  ) const -> std::unique_ptr<LSPAny> {
    switch (static_cast<CallHierarchyIncomingCallsResultType>(result.index())) {
    case CallHierarchyIncomingCallsResultType::CALL_HIERARCHY_INCOMING_CALL_ARRAY: {
      std::unique_ptr<LSPAny> array = std::make_unique<LSPAny>();
      std::unique_ptr<LSPArray> value = std::make_unique<LSPArray>();
      for (const std::unique_ptr<CallHierarchyIncomingCall> &call
             : std::get<ptr_vector<CallHierarchyIncomingCall>>(result)) {
        value->push_back(lspToAny(*call));
      }
      array->value = std::move(value);
      return array;
    }
    case CallHierarchyIncomingCallsResultType::LSP_NULL: {
      return nullToAny(std::get<std::nullptr_t>(result));
    }
    }
  }

  auto LspTransformer::lspToAny(
    const CallHierarchyIncomingCall &call
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(call);
    return any;
  }

  auto LspTransformer::lspToObject(
    const CallHierarchyIncomingCall &call
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("from", lspToAny(*call.from));
    std::unique_ptr<LSPArray> fromRangesValue = std::make_unique<LSPArray>();
    for (const std::unique_ptr<Range> &range : call.fromRanges) {
      fromRangesValue->push_back(lspToAny(*range));
    }
    std::unique_ptr<LSPAny> fromRanges = std::make_unique<LSPAny>();
    fromRanges->value = std::move(fromRangesValue);
    object->emplace("fromRanges", std::move(fromRanges));
    return object;
  }

  auto LspTransformer::asCallHierarchyOutgoingCallsParams(
    const MessageParams &requestParams
  ) const -> CallHierarchyOutgoingCallsParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::CALL_HIERARCHY_OUTGOING_CALLS
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    CallHierarchyOutgoingCallsParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("partialResultToken");
    if (iter != object.end()) {
      params.partialResultToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("item");
    if (iter != object.end()) {
      params.item = anyToCallHierarchyItem(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required CallHierarchyPrepareParams attribute: item"
      );
    }

    return params;
  }

  auto LspTransformer::lspToAny(
    const CallHierarchyOutgoingCallsResult &result
  ) const -> std::unique_ptr<LSPAny> {
    switch (static_cast<CallHierarchyOutgoingCallsResultType>(result.index())) {
    case CallHierarchyOutgoingCallsResultType::CALL_HIERARCHY_OUTGOING_CALL_ARRAY: {
      std::unique_ptr<LSPAny> array = std::make_unique<LSPAny>();
      std::unique_ptr<LSPArray> value = std::make_unique<LSPArray>();
      for (const std::unique_ptr<CallHierarchyOutgoingCall> &call
             : std::get<ptr_vector<CallHierarchyOutgoingCall>>(result)) {
        value->push_back(lspToAny(*call));
      }
      array->value = std::move(value);
      return array;
    }
    case CallHierarchyOutgoingCallsResultType::LSP_NULL: {
      return nullToAny(std::get<std::nullptr_t>(result));
    }
    }
  }

  auto LspTransformer::lspToAny(
    const CallHierarchyOutgoingCall &call
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(call);
    return any;
  }

  auto LspTransformer::lspToObject(
    const CallHierarchyOutgoingCall &call
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("to", lspToAny(*call.to));
    std::unique_ptr<LSPArray> fromRangesValue = std::make_unique<LSPArray>();
    for (const std::unique_ptr<Range> &range : call.fromRanges) {
      fromRangesValue->push_back(lspToAny(*range));
    }
    std::unique_ptr<LSPAny> fromRanges = std::make_unique<LSPAny>();
    fromRanges->value = std::move(fromRangesValue);
    object->emplace("fromRanges", std::move(fromRanges));
    return object;
  }

  auto LspTransformer::asTypeHierarchyPrepareParams(
    const MessageParams &requestParams
  ) const -> TypeHierarchyPrepareParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::PREPARE_TYPE_HIERARCHY
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    TypeHierarchyPrepareParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchyPrepareParams attribute: textDocument"
      );
    }

    iter = object.find("position");
    if (iter != object.end()) {
      params.position = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchyPrepareParams attribute: position"
      );
    }

    iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    return params;
  }

  auto LspTransformer::lspToAny(
    const TypeHierarchyResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();

    switch (static_cast<TypeHierarchyResultType>(result.index())) {
    case TypeHierarchyResultType::TYPE_HIERARCHY_ITEM_ARRAY: {
      std::unique_ptr<LSPArray> items = std::make_unique<LSPArray>();
      for (const std::unique_ptr<TypeHierarchyItem> &item
             : std::get<ptr_vector<TypeHierarchyItem>>(result)) {
        items->push_back(lspToAny(*item));
      }
      any->value = std::move(items);
      break;
    }
    case TypeHierarchyResultType::LSP_NULL: {
      any->value = nullptr;
      break;
    }
    }

    return any;
  }

  auto LspTransformer::asTypeHierarchySupertypesParams(
    const MessageParams &requestParams
  ) const -> TypeHierarchySupertypesParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::TYPE_HIERARCHY_SUPERTYPES
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    TypeHierarchySupertypesParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("partialResultToken");
    if (iter != object.end()) {
      params.partialResultToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("item");
    if (iter != object.end()) {
      params.item = anyToTypeHierarchyItem(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchySupertypeParams attribute: item"
      );
    }

    return params;
  }

  auto LspTransformer::anyToTypeHierarchyItem(
    const LSPAny &any
  ) const -> std::unique_ptr<TypeHierarchyItem> {
    assertAnyType("TypeHierarchyItem", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<TypeHierarchyItem> item =
      std::unique_ptr<TypeHierarchyItem>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("name");
    if (iter != object.end()) {
      item->name = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchyItem attribute: name"
      );
    }

    iter = object.find("kind");
    if (iter != object.end()) {
      item->kind = anyToSymbolKind(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchyItem attribute: kind"
      );
    }

    iter = object.find("tags");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("TypeHierarchyItem::tags", array, LSPAnyType::LSP_ARRAY);
      std::vector<SymbolTag> tags;
      for (const std::unique_ptr<LSPAny> &tag
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        tags.push_back(anyToSymbolTag(*tag));
      }
      item->tags = std::move(tags);
    }

    iter = object.find("detail");
    if (iter != object.end()) {
      item->detail = anyToString(*iter->second);
    }

    iter = object.find("uri");
    if (iter != object.end()) {
      item->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchyItem attribute: uri"
      );
    }

    iter = object.find("range");
    if (iter != object.end()) {
      item->range = anyToRange(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchyItem attribute: range"
      );
    }

    iter = object.find("selectionRange");
    if (iter != object.end()) {
      item->selectionRange = anyToRange(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchyItem attribute: selectionRange"
      );
    }

    iter = object.find("data");
    if (iter != object.end()) {
      item->data = copy(iter->second);
    }

    return item;
  }

  auto LspTransformer::asTypeHierarchySubtypesParams(
    const MessageParams &requestParams
  ) const -> TypeHierarchySubtypesParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::TYPE_HIERARCHY_SUBTYPES
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    TypeHierarchySubtypesParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("partialResultToken");
    if (iter != object.end()) {
      params.partialResultToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("item");
    if (iter != object.end()) {
      params.item = anyToTypeHierarchyItem(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required TypeHierarchySubtypeParams attribute: item"
      );
    }

    return params;
  }

  auto LspTransformer::asDocumentHighlightParams(
    const MessageParams &requestParams
  ) const -> DocumentHighlightParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::HIGHLIGHT_DOCUMENT
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    DocumentHighlightParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DocumentHighlightParams attribute: textDocument"
      );
    }

    iter = object.find("position");
    if (iter != object.end()) {
      params.position = anyToPosition(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DocumentHighlightParams attribute: position"
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

  auto LspTransformer::lspToAny(
    const DocumentHighlightResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();

    switch (static_cast<DocumentHighlightResultType>(result.index())) {
    case DocumentHighlightResultType::DOCUMENT_HIGHLIGHT_ARRAY: {
      std::unique_ptr<LSPArray> items = std::make_unique<LSPArray>();
      for (const std::unique_ptr<DocumentHighlight> &highlight
             : std::get<ptr_vector<DocumentHighlight>>(result)) {
        items->push_back(lspToAny(*highlight));
      }
      any->value = std::move(items);
      break;
    }
    case DocumentHighlightResultType::LSP_NULL: {
      any->value = nullptr;
      break;
    }
    }

    return any;
  }

  auto LspTransformer::lspToAny(
    const DocumentHighlight &highlight
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(highlight);
    return any;
  }

  auto LspTransformer::lspToObject(
    const DocumentHighlight &highlight
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("range", lspToAny(*highlight.range));
    if (highlight.kind.has_value()) {
      object->emplace("kind", lspToAny(highlight.kind.value()));
    }
    return object;
  }

  auto LspTransformer::lspToAny(
    DocumentHighlightKind kind
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = static_cast<int>(kind);
    return any;
  }

  auto LspTransformer::asDocumentLinkParams(
    const MessageParams &requestParams
  ) const -> DocumentLinkParams {
    assertRequestType(
      RequestMethodValues.at(
        RequestMethod::EXTRACT_DOCUMENT_LINKS
      ),
      requestParams,
      MessageParamsType::LSP_OBJECT
    );

    DocumentLinkParams params;

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(requestParams);

    auto iter = object.find("workDoneToken");
    if (iter != object.end()) {
      params.workDoneToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("partialResultToken");
    if (iter != object.end()) {
      params.partialResultToken = anyToProgressToken(*iter->second);
    }

    iter = object.find("textDocument");
    if (iter != object.end()) {
      params.textDocument = anyToTextDocumentIdentifier(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required DocumentLinkParams attribute: textDocument"
      );
    }

    return params;
  }

  auto LspTransformer::lspToAny(
    const DocumentLinkResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();

    switch (static_cast<DocumentLinkResultType>(result.index())) {
    case DocumentLinkResultType::DOCUMENT_LINK_ARRAY: {
      std::unique_ptr<LSPArray> items = std::make_unique<LSPArray>();
      for (const std::unique_ptr<DocumentLink> &link
             : std::get<ptr_vector<DocumentLink>>(result)) {
        items->push_back(lspToAny(*link));
      }
      any->value = std::move(items);
      break;
    }
    case DocumentLinkResultType::LSP_NULL: {
      any->value = nullptr;
      break;
    }
    }

    return any;
  }

  auto LspTransformer::lspToAny(
    const DocumentLink &link
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(link);
    return any;
  }

  auto LspTransformer::lspToObject(
    const DocumentLink &link
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("range", lspToAny(*link.range));
    if (link.target.has_value()) {
      object->emplace("target", stringToAny(link.target.value()));
    }
    if (link.tooltip.has_value()) {
      object->emplace("tooltip", stringToAny(link.tooltip.value()));
    }
    if (link.data.has_value()) {
      object->emplace("data", copy(link.data.value()));
    }
    return object;
  }

  auto LspTransformer::lspToAny(
    const TypeHierarchyItem &item
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(item);
    return any;
  }

  auto LspTransformer::lspToObject(
    const TypeHierarchyItem &item
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("name", stringToAny(item.name));
    object->emplace("kind", lspToAny(item.kind));
    if (item.tags.has_value()) {
      std::unique_ptr<LSPArray> array = std::make_unique<LSPArray>();
      for (SymbolTag tag : item.tags.value()) {
        array->push_back(lspToAny(tag));
      }
      std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
      any->value = std::move(array);
    }
    return object;
  }

  auto LspTransformer::lspToAny(
    SymbolTag tag
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = static_cast<int>(tag);
    return any;
  }

  auto LspTransformer::lspToAny(
    SymbolKind kind
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = static_cast<int>(kind);
    return any;
  }

  auto LspTransformer::nullToAny(
    std::nullptr_t null
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = null;
    return any;
  }

  auto LspTransformer::anyToReferenceContext(
    const LSPAny &any
  ) const -> std::unique_ptr<ReferenceContext> {
    assertAnyType("WorkspaceFolder", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<ReferenceContext> context =
      std::make_unique<ReferenceContext>();

    const LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("includeDeclaration");
    if (iter != object.end()) {
      context->includeDeclaration = anyToBool(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ReferenceContext attribute: includeDeclaration"
      );
    }

    return context;
  }

  auto LspTransformer::anyToWorkspaceFolder(
    const LSPAny &any
  ) const -> std::unique_ptr<WorkspaceFolder> {
    assertAnyType("WorkspaceFolder", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<WorkspaceFolder> workspaceFolder =
      std::make_unique<WorkspaceFolder>();

    const LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("uri");
    if (iter != object.end()) {
      workspaceFolder->uri = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required WorkspaceFolder attribute: uri"
      );
    }

    iter = object.find("name");
    if (iter != object.end()) {
      workspaceFolder->name = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required WorkspaceFolder attribute: name"
      );
    }

    return workspaceFolder;
  }

  auto LspTransformer::anyToClientInfo(
    const LSPAny &any
  ) const -> std::unique_ptr<ClientInfo> {
    assertAnyType("ClientInfo", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<ClientInfo> clientInfo = std::make_unique<ClientInfo>();

    const LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("name");
    if (iter != object.end()) {
      clientInfo->name = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ClientInfo attribute: name"
      );
    }

    iter = object.find("version");
    if (iter != object.end()) {
      clientInfo->version = anyToString(*iter->second);
    }

    return clientInfo;
  }

  auto LspTransformer::anyToVersionedNotebookDocumentIdentifier(
    const LSPAny &any
  ) const -> std::unique_ptr<VersionedNotebookDocumentIdentifier> {
    assertAnyType("VersionedNotebookDocumentIdentifier", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<VersionedNotebookDocumentIdentifier> documentId =
      std::make_unique<VersionedNotebookDocumentIdentifier>();

    const LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

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
  ) const -> ProgressToken {
    switch (static_cast<LSPAnyType>(any.value.index())) {
    case LSPAnyType::LSP_INTEGER: {
      return std::get<int>(any.value);
    }
    case LSPAnyType::LSP_STRING: {
      return std::get<std::string>(any.value);
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAnyType for a(n) {} must be of type {} or {} but received type {}",
          "ProgressToken",
          static_cast<int>(LSPAnyType::LSP_INTEGER),
          static_cast<int>(LSPAnyType::LSP_STRING),
          any.value.index()
        )
      );
    }
    }
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
      assertAnyType("NotebookCellArrayChange::cells", anyCells, LSPAnyType::LSP_ARRAY);
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
      assertAnyType("NotebookDocumentChangeEventCellsTextContent::changes", any, LSPAnyType::LSP_ARRAY);
      std::vector<TextDocumentContentChangeEvent> changes;
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
      assertAnyType("NotebookDocumentChangeEventCells::data", any, LSPAnyType::LSP_ARRAY);
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
      assertAnyType("NotebookDocumentChangeEventCells::textContent", any, LSPAnyType::LSP_ARRAY);
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
      assertAnyType("NotebookDocumentChangeEvent::metadata", any, LSPAnyType::LSP_OBJECT);
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
      assertAnyType("NotebookCell::metadata", any, LSPAnyType::LSP_OBJECT);
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
      assertAnyType("NotebookDocument::metadata", any, LSPAnyType::LSP_OBJECT);
      const std::unique_ptr<LSPObject> &object =
        std::get<std::unique_ptr<LSPObject>>(any.value);
      notebookDocument->metadata = copy(object);
    }

    iter = object.find("cells");
    if (iter != object.end()) {
      const LSPAny &any = *iter->second;
      assertAnyType("NotebookDocument::cells", any, LSPAnyType::LSP_ARRAY);
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
  ) const -> RequestId {
    switch (static_cast<LSPAnyType>(any.value.index())) {
    case LSPAnyType::LSP_INTEGER: {
      return std::get<int>(any.value);
    }
    case LSPAnyType::LSP_STRING: {
      return std::get<std::string>(any.value);
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAnyType for a(n) {} must be of type {} or {} but received type {}",
          "RequestId",
          static_cast<int>(LSPAnyType::LSP_INTEGER),
          static_cast<int>(LSPAnyType::LSP_STRING),
          any.value.index()
        )
      );
    }
    }
  }

  auto LspTransformer::anyToFailureHandlingKind(
    const LSPAny &any
  ) const -> FailureHandlingKind {
    const std::string &value = anyToString(any);
    try {
      return failureHandlingKindByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToResourceOperationKind(
    const LSPAny &any
  ) const -> ResourceOperationKind {
    const std::string &value = anyToString(any);
    try {
      return resourceOperationKindByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToChangeAnnotationSupport(
    const LSPAny &any
  ) const -> std::unique_ptr<ChangeAnnotationSupport> {
    assertAnyType("ChangeAnnotationSupport", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<ChangeAnnotationSupport> support =
      std::make_unique<ChangeAnnotationSupport>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("groupsOnLabel");
    if (iter != object.end()) {
      support->groupsOnLabel = anyToBool(*iter->second);
    }

    return support;
  }

  auto LspTransformer::anyToWorkspaceEditClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<WorkspaceEditClientCapabilities> {
    assertAnyType(
      "WorkspaceEditClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<WorkspaceEditClientCapabilities> capabilities =
      std::make_unique<WorkspaceEditClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("documentChanges");
    if (iter != object.end()) {
      capabilities->documentChanges = anyToBool(*iter->second);
    }

    iter = object.find("resourceOperations");
    if (iter != object.end()) {
      const LSPAny &any = *iter->second;
      assertAnyType(
        "WorkspaceEditClientCapabilities.resourceOperations",
        any, LSPAnyType::LSP_ARRAY
      );
      std::vector<ResourceOperationKind> resourceOperations;
      for (const std::unique_ptr<LSPAny> &operation
             : *std::get<std::unique_ptr<LSPArray>>(any.value)) {
        resourceOperations.push_back(anyToResourceOperationKind(*operation));
      }
      capabilities->resourceOperations = std::move(resourceOperations);
    }

    iter = object.find("failureHandling");
    if (iter != object.end()) {
      capabilities->failureHandling = anyToFailureHandlingKind(*iter->second);
    }

    iter = object.find("normalizesLineEndings");
    if (iter != object.end()) {
      capabilities->normalizesLineEndings = anyToBool(*iter->second);
    }

    iter = object.find("changeAnnotationSupport");
    if (iter != object.end()) {
      capabilities->changeAnnotationSupport =
        anyToChangeAnnotationSupport(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDidChangeConfigurationClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DidChangeConfigurationClientCapabilities> {
    assertAnyType(
      "DidChangeConfigurationClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DidChangeConfigurationClientCapabilities> capabilities =
      std::make_unique<DidChangeConfigurationClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDidChangeWatchedFilesClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DidChangeWatchedFilesClientCapabilities> {
    assertAnyType("WorkspaceCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<DidChangeWatchedFilesClientCapabilities> capabilities =
      std::make_unique<DidChangeWatchedFilesClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("relativePatternSupport");
    if (iter != object.end()) {
      capabilities->relativePatternSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToSymbolKind(
    const LSPAny &any
  ) const -> SymbolKind {
    int value = anyToInt(any);
    try {
      return symbolKindByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToOptionalSymbolKindValueSet(
    const LSPAny &any
  ) const -> std::unique_ptr<OptionalValueSet<SymbolKind>> {
    assertAnyType(
      "OptionalValueSet<SymbolKind>",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<OptionalValueSet<SymbolKind>> valueSet =
      std::make_unique<OptionalValueSet<SymbolKind>>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("valueSet");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType(
        "OptionalValueSet<SymbolKind>::valueSet",
        array, LSPAnyType::LSP_ARRAY
      );
      std::vector<SymbolKind> symbolKinds;
      for (const std::unique_ptr<LSPAny> &symbolKind
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        symbolKinds.push_back(anyToSymbolKind(*symbolKind));
      }
      valueSet->valueSet = std::move(symbolKinds);
    }

    return valueSet;
  }

  auto LspTransformer::anyToSymbolTag(
    const LSPAny &any
  ) const -> SymbolTag {
    int value = anyToInt(any);
    try {
      return symbolTagByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToSymbolTagValueSet(
    const LSPAny &any
  ) const -> std::unique_ptr<ValueSet<SymbolTag>> {
    assertAnyType(
      "ValueSet<SymbolTag>",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ValueSet<SymbolTag>> valueSet =
      std::make_unique<ValueSet<SymbolTag>>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("valueSet");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType(
        "ValueSet<SymbolTag>::valueSet",
        array, LSPAnyType::LSP_ARRAY
      );
      std::vector<SymbolTag> symbolKinds;
      for (const std::unique_ptr<LSPAny> &symbolKind
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        symbolKinds.push_back(anyToSymbolTag(*symbolKind));
      }
      valueSet->valueSet = std::move(symbolKinds);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ValueSet<SymbolTab> attribute: valueSet"
      );
    }

    return valueSet;
  }

  auto LspTransformer::anyToResolveSupportCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ResolveSupportCapabilities> {
    assertAnyType(
      "ResolveSupportCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ResolveSupportCapabilities> capabilities =
      std::make_unique<ResolveSupportCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("properties");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("ResolveSupportCapabilities::properties", array, LSPAnyType::LSP_ARRAY);
      std::vector<std::string> properties;
      for (const std::unique_ptr<LSPAny> &property
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        properties.push_back(anyToString(*property));
      }
      capabilities->properties = std::move(properties);
    }

    return capabilities;
  }

  auto LspTransformer::anyToWorkspaceSymbolClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<WorkspaceSymbolClientCapabilities> {
    assertAnyType("WorkspaceCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<WorkspaceSymbolClientCapabilities> capabilities =
      std::make_unique<WorkspaceSymbolClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("symbolKind");
    if (iter != object.end()) {
      capabilities->symbolKind = anyToOptionalSymbolKindValueSet(*iter->second);
    }

    iter = object.find("tagSupport");
    if (iter != object.end()) {
      capabilities->tagSupport = anyToSymbolTagValueSet(*iter->second);
    }

    iter = object.find("resolveSupport");
    if (iter != object.end()) {
      capabilities->resolveSupport =
        anyToResolveSupportCapabilities(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToExecuteCommandClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ExecuteCommandClientCapabilities> {
    assertAnyType(
      "ExecuteCommandClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ExecuteCommandClientCapabilities> capabilities =
      std::make_unique<ExecuteCommandClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToSemanticTokensWorkspaceClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<SemanticTokensWorkspaceClientCapabilities> {
    assertAnyType(
      "SemanticTokensWorkspaceClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<SemanticTokensWorkspaceClientCapabilities> capabilities =
      std::make_unique<SemanticTokensWorkspaceClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("refreshSupport");
    if (iter != object.end()) {
      capabilities->refreshSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToCodeLensWorkspaceClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<CodeLensWorkspaceClientCapabilities> {
    assertAnyType(
      "CodeLensWorkspaceClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<CodeLensWorkspaceClientCapabilities> capabilities =
      std::make_unique<CodeLensWorkspaceClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("refreshSupport");
    if (iter != object.end()) {
      capabilities->refreshSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToFileCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<FileCapabilities> {
    assertAnyType("FileCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<FileCapabilities> capabilities =
      std::make_unique<FileCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("didCreate");
    if (iter != object.end()) {
      capabilities->didCreate = anyToBool(*iter->second);
    }

    iter = object.find("willCreate");
    if (iter != object.end()) {
      capabilities->willCreate = anyToBool(*iter->second);
    }

    iter = object.find("didRename");
    if (iter != object.end()) {
      capabilities->didRename = anyToBool(*iter->second);
    }

    iter = object.find("willRename");
    if (iter != object.end()) {
      capabilities->willRename = anyToBool(*iter->second);
    }

    iter = object.find("didDelete");
    if (iter != object.end()) {
      capabilities->didDelete = anyToBool(*iter->second);
    }

    iter = object.find("willDelete");
    if (iter != object.end()) {
      capabilities->willDelete = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToInlineValueWorkspaceClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<InlineValueWorkspaceClientCapabilities> {
    assertAnyType(
      "InlineValueWorkspaceClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<InlineValueWorkspaceClientCapabilities> capabilities =
      std::make_unique<InlineValueWorkspaceClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("refreshSupport");
    if (iter != object.end()) {
      capabilities->refreshSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToInlayHintWorkspaceClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<InlayHintWorkspaceClientCapabilities> {
    assertAnyType(
      "InlayHintWorkspaceClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<InlayHintWorkspaceClientCapabilities> capabilities =
      std::make_unique<InlayHintWorkspaceClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("refreshSupport");
    if (iter != object.end()) {
      capabilities->refreshSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDiagnosticWorkspaceClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DiagnosticWorkspaceClientCapabilities> {
    assertAnyType(
      "DiagnosticWorkspaceClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DiagnosticWorkspaceClientCapabilities> capabilities =
      std::make_unique<DiagnosticWorkspaceClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("refreshSupport");
    if (iter != object.end()) {
      capabilities->refreshSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToWorkspaceCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<WorkspaceCapabilities> {
    assertAnyType("WorkspaceCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<WorkspaceCapabilities> capabilities =
      std::make_unique<WorkspaceCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("applyEdit");
    if (iter != object.end()) {
      capabilities->applyEdit = anyToBool(*iter->second);
    }

    iter = object.find("workspaceEdit");
    if (iter != object.end()) {
      capabilities->workspaceEdit =
        anyToWorkspaceEditClientCapabilities(*iter->second);
    }

    iter = object.find("didChangeConfiguration");
    if (iter != object.end()) {
      capabilities->didChangeConfiguration =
        anyToDidChangeConfigurationClientCapabilities(*iter->second);
    }

    iter = object.find("didChangeWatchedFiles");
    if (iter != object.end()) {
      capabilities->didChangeWatchedFiles =
        anyToDidChangeWatchedFilesClientCapabilities(*iter->second);
    }

    iter = object.find("symbol");
    if (iter != object.end()) {
      capabilities->symbol =
        anyToWorkspaceSymbolClientCapabilities(*iter->second);
    }

    iter = object.find("executeCommand");
    if (iter != object.end()) {
      capabilities->executeCommand =
        anyToExecuteCommandClientCapabilities(*iter->second);
    }

    iter = object.find("workspaceFolders");
    if (iter != object.end()) {
      capabilities->workspaceFolders = anyToBool(*iter->second);
    }

    iter = object.find("configuration");
    if (iter != object.end()) {
      capabilities->configuration = anyToBool(*iter->second);
    }

    iter = object.find("semanticTokens");
    if (iter != object.end()) {
      capabilities->semanticTokens =
        anyToSemanticTokensWorkspaceClientCapabilities(*iter->second);
    }

    iter = object.find("codeLens");
    if (iter != object.end()) {
      capabilities->codeLens =
        anyToCodeLensWorkspaceClientCapabilities(*iter->second);
    }

    iter = object.find("fileOperations");
    if (iter != object.end()) {
      capabilities->fileOperations = anyToFileCapabilities(*iter->second);
    }

    iter = object.find("inlineValue");
    if (iter != object.end()) {
      capabilities->inlineValue =
        anyToInlineValueWorkspaceClientCapabilities(*iter->second);
    }

    iter = object.find("inlayHint");
    if (iter != object.end()) {
      capabilities->inlayHint =
        anyToInlayHintWorkspaceClientCapabilities(*iter->second);
    }

    iter = object.find("diagnostics");
    if (iter != object.end()) {
      capabilities->diagnostics =
        anyToDiagnosticWorkspaceClientCapabilities(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToTextDocumentSyncClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentSyncClientCapabilities> {
    assertAnyType(
      "TextDocumentSyncClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<TextDocumentSyncClientCapabilities> capabilities =
      std::make_unique<TextDocumentSyncClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("willSave");
    if (iter != object.end()) {
      capabilities->willSave = anyToBool(*iter->second);
    }

    iter = object.find("willSaveWaitUntil");
    if (iter != object.end()) {
      capabilities->willSaveWaitUntil = anyToBool(*iter->second);
    }

    iter = object.find("didSave");
    if (iter != object.end()) {
      capabilities->didSave = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToMarkupKind(
    const LSPAny &any
  ) const -> MarkupKind {
    const std::string &value = anyToString(any);
    try {
      return markupKindByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToCompletionItemTag(
    const LSPAny &any
  ) const -> CompletionItemTag {
    int value = anyToInt(any);
    try {
      return completionItemTagByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToCompletionItemTagValueSet(
    const LSPAny &any
  ) const -> std::unique_ptr<ValueSet<CompletionItemTag>> {
    assertAnyType(
      "ValueSet<CompletionItemTag>",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ValueSet<CompletionItemTag>> valueSet =
      std::make_unique<ValueSet<CompletionItemTag>>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("valueSet");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("ValueSet<CompletionItemTag>::valueSet", array, LSPAnyType::LSP_ARRAY);
      std::vector<CompletionItemTag> tags;
      for (const std::unique_ptr<LSPAny> &tag
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        tags.push_back(anyToCompletionItemTag(*tag));
      }
      valueSet->valueSet = std::move(tags);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ValueSet<CompletionItemTag> attribute: valueSet"
      );
    }

    return valueSet;
  }

  auto LspTransformer::anyToInsertTextModeValueSet(
    const LSPAny &any
  ) const -> std::unique_ptr<ValueSet<InsertTextMode>> {
    assertAnyType(
      "ValueSet<InsertTextMode>",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ValueSet<InsertTextMode>> modes =
      std::make_unique<ValueSet<InsertTextMode>>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("valueSet");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("ValueSet<InsertTextMode>::valueSet", array, LSPAnyType::LSP_ARRAY);
      std::vector<InsertTextMode> valueSet;
      for (const std::unique_ptr<LSPAny> &mode
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        valueSet.push_back(anyToInsertTextMode(*mode));
      }
      modes->valueSet = std::move(valueSet);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ValueSet<InsertTextMode> attribute: valueSet"
      );
    }

    return modes;
  }

  auto LspTransformer::anyToInsertTextMode(
    const LSPAny &any
  ) const -> InsertTextMode {
    int value = anyToInt(any);
    try {
      return insertTextModeByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToCompletionItemCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<CompletionItemCapabilities> {
    assertAnyType(
      "CompletionItemCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<CompletionItemCapabilities> capabilities =
      std::make_unique<CompletionItemCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("snippetSupport");
    if (iter != object.end()) {
      capabilities->snippetSupport = anyToBool(*iter->second);
    }

    iter = object.find("commitCharactersSupport");
    if (iter != object.end()) {
      capabilities->commitCharactersSupport = anyToBool(*iter->second);
    }

    iter = object.find("documentationFormat");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("CompletionItemCapabilities::documentationFormat", array, LSPAnyType::LSP_ARRAY);
      std::vector<MarkupKind> documentationFormat;
      for (const std::unique_ptr<LSPAny> &markupKind
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        documentationFormat.push_back(anyToMarkupKind(*markupKind));
      }
      capabilities->documentationFormat = std::move(documentationFormat);
    }

    iter = object.find("deprecatedSupport");
    if (iter != object.end()) {
      capabilities->deprecatedSupport = anyToBool(*iter->second);
    }

    iter = object.find("preselectSupport");
    if (iter != object.end()) {
      capabilities->preselectSupport = anyToBool(*iter->second);
    }

    iter = object.find("tagSupport");
    if (iter != object.end()) {
      capabilities->tagSupport = anyToCompletionItemTagValueSet(*iter->second);
    }

    iter = object.find("insertReplaceSupport");
    if (iter != object.end()) {
      capabilities->insertReplaceSupport = anyToBool(*iter->second);
    }

    iter = object.find("resolveSupport");
    if (iter != object.end()) {
      capabilities->resolveSupport =
        anyToResolveSupportCapabilities(*iter->second);
    }

    iter = object.find("insertTextModeSupport");
    if (iter != object.end()) {
      capabilities->insertTextModeSupport =
        anyToInsertTextModeValueSet(*iter->second);
    }

    iter = object.find("labelDetailsSupport");
    if (iter != object.end()) {
      capabilities->labelDetailsSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToOptionalCompletionItemKindValueSet(
    const LSPAny &any
  ) const -> std::unique_ptr<OptionalValueSet<CompletionItemKind>> {
    assertAnyType(
      "OptionalValueSet<CompletionItemKind>",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<OptionalValueSet<CompletionItemKind>> kinds =
      std::make_unique<OptionalValueSet<CompletionItemKind>>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("valueSet");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("OptionalValueSet<CompletionItemKind>::valueSet", array, LSPAnyType::LSP_ARRAY);
      std::vector<CompletionItemKind> valueSet;
      for (const std::unique_ptr<LSPAny> &kind
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        valueSet.push_back(anyToCompletionItemKind(*kind));
      }
      kinds->valueSet = std::move(valueSet);
    }

    return kinds;
  }

  auto LspTransformer::anyToCompletionItemKind(
    const LSPAny &any
  ) const -> CompletionItemKind {
    int value = anyToInt(any);
    try {
      return completionItemKindByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToCompletionListCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<CompletionListCapabilities> {
    assertAnyType(
      "CompletionListCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<CompletionListCapabilities> capabilities =
      std::make_unique<CompletionListCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("itemDefaults");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("CompletionListCapabilities::itemDefaults", array, LSPAnyType::LSP_ARRAY);
      std::vector<std::string> itemDefaults;
      for (const std::unique_ptr<LSPAny> &itemDefault
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        itemDefaults.push_back(anyToString(*itemDefault));
      }
      capabilities->itemDefaults = std::move(itemDefaults);
    }

    return capabilities;
  }

  auto LspTransformer::anyToCompletionClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<CompletionClientCapabilities> {
    assertAnyType(
      "CompletionClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<CompletionClientCapabilities> capabilities =
      std::make_unique<CompletionClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("completionItem");
    if (iter != object.end()) {
      capabilities->completionItem =
        anyToCompletionItemCapabilities(*iter->second);
    }

    iter = object.find("completionItemKind");
    if (iter != object.end()) {
      capabilities->completionItemKind =
        anyToOptionalCompletionItemKindValueSet(*iter->second);
    }

    iter = object.find("contextSupport");
    if (iter != object.end()) {
      capabilities->contextSupport = anyToBool(*iter->second);
    }

    iter = object.find("insertTextMode");
    if (iter != object.end()) {
      capabilities->insertTextMode = anyToInsertTextMode(*iter->second);
    }

    iter = object.find("completionList");
    if (iter != object.end()) {
      capabilities->completionList =
        anyToCompletionListCapabilities(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToHoverClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<HoverClientCapabilities> {
    assertAnyType(
      "HoverClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<HoverClientCapabilities> capabilities =
      std::make_unique<HoverClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("contentFormat");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("HoverClientCapabilities::contentFormat", array, LSPAnyType::LSP_ARRAY);
      std::vector<MarkupKind> contentFormat;
      for (const std::unique_ptr<LSPAny> &value
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        contentFormat.push_back(anyToMarkupKind(*value));
      }
      capabilities->contentFormat = std::move(contentFormat);
    }

    return capabilities;
  }

  auto LspTransformer::anyToParameterInformationCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ParameterInformationCapabilities> {
    assertAnyType(
      "ParameterInformationCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ParameterInformationCapabilities> capabilities =
      std::make_unique<ParameterInformationCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("labelOffsetSupport");
    if (iter != object.end()) {
      capabilities->labelOffsetSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToSignatureInformationCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<SignatureInformationCapabilities> {
    assertAnyType(
      "SignatureInformationCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<SignatureInformationCapabilities> capabilities =
      std::make_unique<SignatureInformationCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("documentationFormat");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("SignatureInformationCapabilities::documentationFormat", array, LSPAnyType::LSP_ARRAY);
      std::vector<MarkupKind> documentationFormat;
      for (const std::unique_ptr<LSPAny> &value
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        documentationFormat.push_back(anyToMarkupKind(*value));
      }
      capabilities->documentationFormat = std::move(documentationFormat);
    }

    iter = object.find("parameterInformation");
    if (iter != object.end()) {
      capabilities->parameterInformation =
        anyToParameterInformationCapabilities(*iter->second);
    }

    iter = object.find("activeParameterSupport");
    if (iter != object.end()) {
      capabilities->activeParameterSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToSignatureHelpClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<SignatureHelpClientCapabilities> {
    assertAnyType(
      "SignatureHelpClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<SignatureHelpClientCapabilities> capabilities =
      std::make_unique<SignatureHelpClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("signatureInformation");
    if (iter != object.end()) {
      capabilities->signatureInformation =
        anyToSignatureInformationCapabilities(*iter->second);
    }

    iter = object.find("contextSupport");
    if (iter != object.end()) {
      capabilities->contextSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDeclarationClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DeclarationClientCapabilities> {
    assertAnyType(
      "DeclarationClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DeclarationClientCapabilities> capabilities =
      std::make_unique<DeclarationClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("linkSupport");
    if (iter != object.end()) {
      capabilities->linkSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDefinitionClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DefinitionClientCapabilities> {
    assertAnyType(
      "DefinitionClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DefinitionClientCapabilities> capabilities =
      std::make_unique<DefinitionClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("linkSupport");
    if (iter != object.end()) {
      capabilities->linkSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToTypeDefinitionClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<TypeDefinitionClientCapabilities> {
    assertAnyType(
      "TypeDefinitionClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<TypeDefinitionClientCapabilities> capabilities =
      std::make_unique<TypeDefinitionClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("linkSupport");
    if (iter != object.end()) {
      capabilities->linkSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToImplementationClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ImplementationClientCapabilities> {
    assertAnyType(
      "ImplementationClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ImplementationClientCapabilities> capabilities =
      std::make_unique<ImplementationClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("linkSupport");
    if (iter != object.end()) {
      capabilities->linkSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToReferenceClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ReferenceClientCapabilities> {
    assertAnyType(
      "ReferenceClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ReferenceClientCapabilities> capabilities =
      std::make_unique<ReferenceClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDocumentHighlightClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DocumentHighlightClientCapabilities> {
    assertAnyType(
      "DocumentHighlightClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DocumentHighlightClientCapabilities> capabilities =
      std::make_unique<DocumentHighlightClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDocumentSymbolClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DocumentSymbolClientCapabilities> {
    assertAnyType(
      "DocumentSymbolClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DocumentSymbolClientCapabilities> capabilities =
      std::make_unique<DocumentSymbolClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("symbolKind");
    if (iter != object.end()) {
      capabilities->symbolKind = anyToOptionalSymbolKindValueSet(*iter->second);
    }

    iter = object.find("hierarchicalDocumentSymbolSupport");
    if (iter != object.end()) {
      capabilities->hierarchicalDocumentSymbolSupport =
        anyToBool(*iter->second);
    }

    iter = object.find("tagSupport");
    if (iter != object.end()) {
      capabilities->tagSupport = anyToSymbolTagValueSet(*iter->second);
    }

    iter = object.find("labelSupport");
    if (iter != object.end()) {
      capabilities->labelSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToCodeActionKind(
    const LSPAny &any
  ) const -> CodeActionKind {
    const std::string &value = anyToString(any);
    try {
      return codeActionKindByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToCodeActionKindValueSet(
    const LSPAny &any
  ) const -> std::unique_ptr<ValueSet<CodeActionKind>> {
    assertAnyType(
      "ValueSet<CodeActionKind>",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ValueSet<CodeActionKind>> codeActions =
      std::make_unique<ValueSet<CodeActionKind>>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("valueSet");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("ValueSet<CodeActionKind>::valueSet", array, LSPAnyType::LSP_ARRAY);
      std::vector<CodeActionKind> valueSet;
      for (const std::unique_ptr<LSPAny> &codeAction
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        valueSet.push_back(anyToCodeActionKind(*codeAction));
      }
      codeActions->valueSet = std::move(valueSet);
    }

    return codeActions;
  }

  auto LspTransformer::anyToCodeActionLiteralSupportCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<CodeActionLiteralSupportCapabilities> {
    assertAnyType(
      "CodeActionLiteralSupportCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<CodeActionLiteralSupportCapabilities> capabilities =
      std::make_unique<CodeActionLiteralSupportCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("codeActionKind");
    if (iter != object.end()) {
      capabilities->codeActionKind = anyToCodeActionKindValueSet(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToCodeActionClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<CodeActionClientCapabilities> {
    assertAnyType(
      "CodeActionClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<CodeActionClientCapabilities> capabilities =
      std::make_unique<CodeActionClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("codeActionLiteralSupport");
    if (iter != object.end()) {
      capabilities->codeActionLiteralSupport =
        anyToCodeActionLiteralSupportCapabilities(*iter->second);
    }

    iter = object.find("isPreferredSupport");
    if (iter != object.end()) {
      capabilities->isPreferredSupport = anyToBool(*iter->second);
    }

    iter = object.find("disabledSupport");
    if (iter != object.end()) {
      capabilities->disabledSupport = anyToBool(*iter->second);
    }

    iter = object.find("dataSupport");
    if (iter != object.end()) {
      capabilities->dataSupport = anyToBool(*iter->second);
    }

    iter = object.find("resolveSupport");
    if (iter != object.end()) {
      capabilities->resolveSupport =
        anyToResolveSupportCapabilities(*iter->second);
    }

    iter = object.find("honorsChangeAnnotations");
    if (iter != object.end()) {
      capabilities->honorsChangeAnnotations = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToCodeLensClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<CodeLensClientCapabilities> {
    assertAnyType(
      "CodeLensClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<CodeLensClientCapabilities> capabilities =
      std::make_unique<CodeLensClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDocumentLinkClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DocumentLinkClientCapabilities> {
    assertAnyType(
      "DocumentLinkClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DocumentLinkClientCapabilities> capabilities =
      std::make_unique<DocumentLinkClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("tooltipSupport");
    if (iter != object.end()) {
      capabilities->tooltipSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDocumentColorClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DocumentColorClientCapabilities> {
    assertAnyType(
      "DocumentColorClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DocumentColorClientCapabilities> capabilities =
      std::make_unique<DocumentColorClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDocumentFormattingClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DocumentFormattingClientCapabilities> {
    assertAnyType(
      "DocumentFormattingClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DocumentFormattingClientCapabilities> capabilities =
      std::make_unique<DocumentFormattingClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDocumentRangeFormattingClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DocumentRangeFormattingClientCapabilities> {
    assertAnyType(
      "DocumentRangeFormattingClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DocumentRangeFormattingClientCapabilities> capabilities =
      std::make_unique<DocumentRangeFormattingClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDocumentOnTypeFormattingClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DocumentOnTypeFormattingClientCapabilities> {
    assertAnyType(
      "DocumentOnTypeFormattingClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DocumentOnTypeFormattingClientCapabilities> capabilities =
      std::make_unique<DocumentOnTypeFormattingClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToRenameClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<RenameClientCapabilities> {
    assertAnyType(
      "RenameClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<RenameClientCapabilities> capabilities =
      std::make_unique<RenameClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("prepareSupport");
    if (iter != object.end()) {
      capabilities->prepareSupport = anyToBool(*iter->second);
    }

    iter = object.find("prepareSupportDefaultBehavior");
    if (iter != object.end()) {
      capabilities->prepareSupportDefaultBehavior =
        anyToPrepareSupportDefaultBehavior(*iter->second);
    }

    iter = object.find("honorsChangeAnnotations");
    if (iter != object.end()) {
      capabilities->honorsChangeAnnotations = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToPublishDiagnosticsClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<PublishDiagnosticsClientCapabilities> {
    assertAnyType(
      "PublishDiagnosticsClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<PublishDiagnosticsClientCapabilities> capabilities =
      std::make_unique<PublishDiagnosticsClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("relatedInformation");
    if (iter != object.end()) {
      capabilities->relatedInformation = anyToBool(*iter->second);
    }

    iter = object.find("tagSupport");
    if (iter != object.end()) {
      capabilities->tagSupport = anyToDiagnosticTagValueSet(*iter->second);
    }

    iter = object.find("versionSupport");
    if (iter != object.end()) {
      capabilities->versionSupport = anyToBool(*iter->second);
    }

    iter = object.find("codeDescriptionSupport");
    if (iter != object.end()) {
      capabilities->codeDescriptionSupport = anyToBool(*iter->second);
    }

    iter = object.find("dataSupport");
    if (iter != object.end()) {
      capabilities->dataSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDiagnosticTag(
    const LSPAny &any
  ) const -> DiagnosticTag {
    int value = anyToInt(any);
    try {
      return diagnosticTagByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToDiagnosticTagValueSet(
    const LSPAny &any
  ) const -> std::unique_ptr<ValueSet<DiagnosticTag>> {
    assertAnyType(
      "ValueSet<DiagnosticTag>",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ValueSet<DiagnosticTag>> tags =
      std::make_unique<ValueSet<DiagnosticTag>>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("valueSet");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("ValueSet<DiagnosticTag>::valueSet", array, LSPAnyType::LSP_ARRAY);
      std::vector<DiagnosticTag> valueSet;
      for (const std::unique_ptr<LSPAny> &tag
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        valueSet.push_back(anyToDiagnosticTag(*tag));
      }
      tags->valueSet = std::move(valueSet);
    }

    return tags;
  }

  auto LspTransformer::anyToOptionalFoldingRangeKindValueSet(
    const LSPAny &any
  ) const -> std::unique_ptr<OptionalValueSet<FoldingRangeKind>> {
    assertAnyType(
      "FoldingRangeClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<OptionalValueSet<FoldingRangeKind>> kinds =
      std::make_unique<OptionalValueSet<FoldingRangeKind>>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("valueSet");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("OptionalValueSet<FoldingRangeKind>::valueSet", array, LSPAnyType::LSP_ARRAY);
      std::vector<FoldingRangeKind> valueSet;
      for (const std::unique_ptr<LSPAny> &kind
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        valueSet.push_back(anyToFoldingRangeKind(*kind));
      }
      kinds->valueSet = std::move(valueSet);
    }

    return kinds;
  }

  auto LspTransformer::anyToFoldingRangeCollapsedText(
    const LSPAny &any
  ) const -> std::unique_ptr<FoldingRangeCollapsedText> {
    assertAnyType(
      "FoldingRangeCollapsedText",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<FoldingRangeCollapsedText> foldingRange =
      std::make_unique<FoldingRangeCollapsedText>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("collapsedText");
    if (iter != object.end()) {
      foldingRange->collapsedText = anyToBool(*iter->second);
    }

    return foldingRange;
  }

  auto LspTransformer::anyToFoldingRangeClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<FoldingRangeClientCapabilities> {
    assertAnyType(
      "FoldingRangeClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<FoldingRangeClientCapabilities> capabilities =
      std::make_unique<FoldingRangeClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("rangeLimit");
    if (iter != object.end()) {
      capabilities->rangeLimit = anyToUnsignedInt(*iter->second);
    }

    iter = object.find("lineFoldingOnly");
    if (iter != object.end()) {
      capabilities->lineFoldingOnly = anyToBool(*iter->second);
    }

    iter = object.find("foldingRangeKind");
    if (iter != object.end()) {
      capabilities->foldingRangeKind =
        anyToOptionalFoldingRangeKindValueSet(*iter->second);
    }

    iter = object.find("foldingRange");
    if (iter != object.end()) {
      capabilities->foldingRange =
        anyToFoldingRangeCollapsedText(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToSelectionRangeClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<SelectionRangeClientCapabilities> {
    assertAnyType(
      "SelectionRangeClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<SelectionRangeClientCapabilities> capabilities =
      std::make_unique<SelectionRangeClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToLinkedEditingRangeClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<LinkedEditingRangeClientCapabilities> {
    assertAnyType(
      "LinkedEditingRangeClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<LinkedEditingRangeClientCapabilities> capabilities =
      std::make_unique<LinkedEditingRangeClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToCallHierarchyClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<CallHierarchyClientCapabilities> {
    assertAnyType(
      "CallHierarchyClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<CallHierarchyClientCapabilities> capabilities =
      std::make_unique<CallHierarchyClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToRangeCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<RangeCapabilities> {
    assertAnyType("RangeCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<RangeCapabilities> capabilities =
      std::make_unique<RangeCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    if (object.size() > 0) {
      throw LspException(
        ErrorCodes::InvalidParams,
        "RangeCapabilities accepts no attributes (currently)."
      );
    }

    return capabilities;
  }

  auto LspTransformer::anyToOptionalRangeCapabilities(
    const LSPAny &any
  ) const -> OptionalRangeCapabilities {
    switch (static_cast<LSPAnyType>(any.value.index())) {
    case LSPAnyType::LSP_BOOLEAN: {
      return anyToBool(any);
    }
    case LSPAnyType::LSP_OBJECT: {
      return anyToRangeCapabilities(any);
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAnyType for a(n) {} must be of type {} or {} but received type {}",
          "OptionalRangeCapabilities",
          static_cast<int>(LSPAnyType::LSP_BOOLEAN),
          static_cast<int>(LSPAnyType::LSP_OBJECT),
          any.value.index()
        )
      );
    }
    }
  }

  auto LspTransformer::anyToFullCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<FullCapabilities> {
    assertAnyType("FullCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<FullCapabilities> capabilities =
      std::make_unique<FullCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("delta");
    if (iter != object.end()) {
      capabilities->delta = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToOptionalFullCapabilities(
    const LSPAny &any
  ) const -> OptionalFullCapabilities {
    switch (static_cast<LSPAnyType>(any.value.index())) {
    case LSPAnyType::LSP_BOOLEAN: {
      return anyToBool(any);
    }
    case LSPAnyType::LSP_OBJECT: {
      return anyToFullCapabilities(any);
    }
    default: {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAnyType for a(n) {} must be of type {} or {} but received type {}",
          "OptionalFullCapabilities",
          static_cast<int>(LSPAnyType::LSP_BOOLEAN),
          static_cast<int>(LSPAnyType::LSP_OBJECT),
          any.value.index()
        )
      );
    }
    }
  }

  auto LspTransformer::anyToSemanticTokensClientRequestCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<SemanticTokensClientRequestCapabilities> {
    assertAnyType(
      "SemanticTokensClientRequestCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<SemanticTokensClientRequestCapabilities> capabilities =
      std::make_unique<SemanticTokensClientRequestCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("range");
    if (iter != object.end()) {
      capabilities->range = anyToOptionalRangeCapabilities(*iter->second);
    }

    iter = object.find("full");
    if (iter != object.end()) {
      capabilities->full = anyToOptionalFullCapabilities(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToTokenFormat(
    const LSPAny &any
  ) const -> TokenFormat {
    const std::string &value = anyToString(any);
    try {
      return tokenFormatByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToSemanticTokensClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<SemanticTokensClientCapabilities> {
    assertAnyType(
      "SemanticTokensClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<SemanticTokensClientCapabilities> capabilities =
      std::make_unique<SemanticTokensClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("requests");
    if (iter != object.end()) {
      capabilities->requests =
        anyToSemanticTokensClientRequestCapabilities(*iter->second);
    }

    iter = object.find("tokenTypes");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("SemanticTokensClientCapabilities::tokenTypes", array, LSPAnyType::LSP_ARRAY);
      std::vector<std::string> tokenTypes;
      for (const std::unique_ptr<LSPAny> &type
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        tokenTypes.push_back(anyToString(*type));
      }
      capabilities->tokenTypes = std::move(tokenTypes);
    }

    iter = object.find("tokenModifiers");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("SemanticTokensClientCapabilities::tokenModifiers", array, LSPAnyType::LSP_ARRAY);
      std::vector<std::string> tokenModifiers;
      for (const std::unique_ptr<LSPAny> &modifier
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        tokenModifiers.push_back(anyToString(*modifier));
      }
      capabilities->tokenModifiers = std::move(tokenModifiers);
    }

    iter = object.find("formats");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("SemanticTokensClientCapabilities::formats", array, LSPAnyType::LSP_ARRAY);
      std::vector<TokenFormat> formats;
      for (const std::unique_ptr<LSPAny> &format
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        formats.push_back(anyToTokenFormat(*format));
      }
      capabilities->formats = std::move(formats);
    }

    iter = object.find("overlappingTokenSupport");
    if (iter != object.end()) {
      capabilities->overlappingTokenSupport = anyToBool(*iter->second);
    }

    iter = object.find("multilineTokenSupport");
    if (iter != object.end()) {
      capabilities->multilineTokenSupport = anyToBool(*iter->second);
    }

    iter = object.find("serverCancelSupport");
    if (iter != object.end()) {
      capabilities->serverCancelSupport = anyToBool(*iter->second);
    }

    iter = object.find("augmentsSyntaxTokens");
    if (iter != object.end()) {
      capabilities->augmentsSyntaxTokens = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToMonikerClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<MonikerClientCapabilities> {
    assertAnyType(
      "MonikerClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<MonikerClientCapabilities> capabilities =
      std::make_unique<MonikerClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToTypeHierarchyClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<TypeHierarchyClientCapabilities> {
    assertAnyType(
      "TypeHierarchyClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<TypeHierarchyClientCapabilities> capabilities =
      std::make_unique<TypeHierarchyClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToInlineValueClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<InlineValueClientCapabilities> {
    assertAnyType(
      "InlineValueClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<InlineValueClientCapabilities> capabilities =
      std::make_unique<InlineValueClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToInlayHintClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<InlayHintClientCapabilities> {
    assertAnyType(
      "InlayHintClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<InlayHintClientCapabilities> capabilities =
      std::make_unique<InlayHintClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("resolveSupport");
    if (iter != object.end()) {
      capabilities->resolveSupport =
        anyToResolveSupportCapabilities(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToDiagnosticClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<DiagnosticClientCapabilities> {
    assertAnyType(
      "DiagnosticClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<DiagnosticClientCapabilities> capabilities =
      std::make_unique<DiagnosticClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("relatedDocumentSupport");
    if (iter != object.end()) {
      capabilities->relatedDocumentSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToTextDocumentClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentClientCapabilities> {
    assertAnyType(
      "TextDocumentClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<TextDocumentClientCapabilities> capabilities =
      std::make_unique<TextDocumentClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("synchronization");
    if (iter != object.end()) {
      capabilities->synchronization =
        anyToTextDocumentSyncClientCapabilities(*iter->second);
    }

    iter = object.find("completion");
    if (iter != object.end()) {
      capabilities->completion =
        anyToCompletionClientCapabilities(*iter->second);
    }

    iter = object.find("hover");
    if (iter != object.end()) {
      capabilities->hover = anyToHoverClientCapabilities(*iter->second);
    }

    iter = object.find("signatureHelp");
    if (iter != object.end()) {
      capabilities->signatureHelp =
        anyToSignatureHelpClientCapabilities(*iter->second);
    }

    iter = object.find("declaration");
    if (iter != object.end()) {
      capabilities->declaration =
        anyToDeclarationClientCapabilities(*iter->second);
    }

    iter = object.find("definition");
    if (iter != object.end()) {
      capabilities->definition =
        anyToDefinitionClientCapabilities(*iter->second);
    }

    iter = object.find("typeDefinition");
    if (iter != object.end()) {
      capabilities->typeDefinition =
        anyToTypeDefinitionClientCapabilities(*iter->second);
    }

    iter = object.find("implementation");
    if (iter != object.end()) {
      capabilities->implementation =
        anyToImplementationClientCapabilities(*iter->second);
    }

    iter = object.find("references");
    if (iter != object.end()) {
      capabilities->references =
        anyToReferenceClientCapabilities(*iter->second);
    }

    iter = object.find("documentHighlight");
    if (iter != object.end()) {
      capabilities->documentHighlight =
        anyToDocumentHighlightClientCapabilities(*iter->second);
    }

    iter = object.find("documentSymbol");
    if (iter != object.end()) {
      capabilities->documentSymbol =
        anyToDocumentSymbolClientCapabilities(*iter->second);
    }

    iter = object.find("codeAction");
    if (iter != object.end()) {
      capabilities->codeAction =
        anyToCodeActionClientCapabilities(*iter->second);
    }

    iter = object.find("codeLens");
    if (iter != object.end()) {
      capabilities->codeLens = anyToCodeLensClientCapabilities(*iter->second);
    }

    iter = object.find("documentLink");
    if (iter != object.end()) {
      capabilities->documentLink =
        anyToDocumentLinkClientCapabilities(*iter->second);
    }

    iter = object.find("colorProvider");
    if (iter != object.end()) {
      capabilities->colorProvider =
        anyToDocumentColorClientCapabilities(*iter->second);
    }

    iter = object.find("formatting");
    if (iter != object.end()) {
      capabilities->formatting =
        anyToDocumentFormattingClientCapabilities(*iter->second);
    }

    iter = object.find("rangeFormatting");
    if (iter != object.end()) {
      capabilities->rangeFormatting =
        anyToDocumentRangeFormattingClientCapabilities(*iter->second);
    }

    iter = object.find("onTypeFormatting");
    if (iter != object.end()) {
      capabilities->onTypeFormatting =
        anyToDocumentOnTypeFormattingClientCapabilities(*iter->second);
    }

    iter = object.find("rename");
    if (iter != object.end()) {
      capabilities->rename = anyToRenameClientCapabilities(*iter->second);
    }

    iter = object.find("publishDiagnostics");
    if (iter != object.end()) {
      capabilities->publishDiagnostics =
        anyToPublishDiagnosticsClientCapabilities(*iter->second);
    }

    iter = object.find("foldingRange");
    if (iter != object.end()) {
      capabilities->foldingRange =
        anyToFoldingRangeClientCapabilities(*iter->second);
    }

    iter = object.find("selectionRange");
    if (iter != object.end()) {
      capabilities->selectionRange =
        anyToSelectionRangeClientCapabilities(*iter->second);
    }

    iter = object.find("linkedEditingRange");
    if (iter != object.end()) {
      capabilities->linkedEditingRange =
        anyToLinkedEditingRangeClientCapabilities(*iter->second);
    }

    iter = object.find("callHierarchy");
    if (iter != object.end()) {
      capabilities->callHierarchy =
        anyToCallHierarchyClientCapabilities(*iter->second);
    }

    iter = object.find("semanticTokens");
    if (iter != object.end()) {
      capabilities->semanticTokens =
        anyToSemanticTokensClientCapabilities(*iter->second);
    }

    iter = object.find("moniker");
    if (iter != object.end()) {
      capabilities->moniker =
        anyToMonikerClientCapabilities(*iter->second);
    }

    iter = object.find("typeHierarchy");
    if (iter != object.end()) {
      capabilities->typeHierarchy =
        anyToTypeHierarchyClientCapabilities(*iter->second);
    }

    iter = object.find("inlineValue");
    if (iter != object.end()) {
      capabilities->inlineValue =
        anyToInlineValueClientCapabilities(*iter->second);
    }

    iter = object.find("inlayHint");
    if (iter != object.end()) {
      capabilities->inlayHint =
        anyToInlayHintClientCapabilities(*iter->second);
    }

    iter = object.find("diagnostic");
    if (iter != object.end()) {
      capabilities->diagnostic =
        anyToDiagnosticClientCapabilities(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToPrepareSupportDefaultBehavior(
    const LSPAny &any
  ) const -> PrepareSupportDefaultBehavior {
    int value = anyToInt(any);
    try {
      return prepareSupportDefaultBehaviorByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToFoldingRangeKind(
    const LSPAny &any
  ) const -> FoldingRangeKind {
    const std::string &value = anyToString(any);
    try {
      return foldingRangeKindByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToNotebookDocumentSyncClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookDocumentSyncClientCapabilities> {
    assertAnyType(
      "NotebookDocumentSyncClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<NotebookDocumentSyncClientCapabilities> capabilities =
      std::make_unique<NotebookDocumentSyncClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("dynamicRegistration");
    if (iter != object.end()) {
      capabilities->dynamicRegistration = anyToBool(*iter->second);
    }

    iter = object.find("executionSummarySupport");
    if (iter != object.end()) {
      capabilities->executionSummarySupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToNotebookDocumentClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<NotebookDocumentClientCapabilities> {
    assertAnyType(
      "NotebookDocumentClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<NotebookDocumentClientCapabilities> capabilities =
      std::make_unique<NotebookDocumentClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("synchronization");
    if (iter != object.end()) {
      capabilities->synchronization =
        anyToNotebookDocumentSyncClientCapabilities(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required NotebookDocumentClientCapabilities attribute: synchronization"
      );
    }

    return capabilities;
  }

  auto LspTransformer::anyToMessageActionItemCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<MessageActionItemCapabilities> {
    assertAnyType(
      "MessageActionItemCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<MessageActionItemCapabilities> capabilities =
      std::make_unique<MessageActionItemCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("additionalPropertiesSupport");
    if (iter != object.end()) {
      capabilities->additionalPropertiesSupport = anyToBool(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToShowMessageRequestClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ShowMessageRequestClientCapabilities> {
    assertAnyType(
      "ShowMessageRequestClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ShowMessageRequestClientCapabilities> capabilities =
      std::make_unique<ShowMessageRequestClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("messageActionItem");
    if (iter != object.end()) {
      capabilities->messageActionItem =
        anyToMessageActionItemCapabilities(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToShowDocumentClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ShowDocumentClientCapabilities> {
    assertAnyType(
      "ShowDocumentClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<ShowDocumentClientCapabilities> capabilities =
      std::make_unique<ShowDocumentClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("support");
    if (iter != object.end()) {
      capabilities->support = anyToBool(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required ShowDocumentClientCapabilities attribute: support"
      );
    }

    return capabilities;
  }

  auto LspTransformer::anyToWindowCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<WindowCapabilities> {
    assertAnyType("WindowCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<WindowCapabilities> capabilities =
      std::make_unique<WindowCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("workDoneProgress");
    if (iter != object.end()) {
      capabilities->workDoneProgress = anyToBool(*iter->second);
    }

    iter = object.find("showMessage");
    if (iter != object.end()) {
      capabilities->showMessage =
        anyToShowMessageRequestClientCapabilities(*iter->second);
    }

    iter = object.find("showDocument");
    if (iter != object.end()) {
      capabilities->showDocument =
        anyToShowDocumentClientCapabilities(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToStaleRequestSupport(
    const LSPAny &any
  ) const -> std::unique_ptr<StaleRequestSupport> {
    assertAnyType("StaleRequestSupport", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<StaleRequestSupport> capabilities =
      std::make_unique<StaleRequestSupport>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("cancel");
    if (iter != object.end()) {
      capabilities->cancel = anyToBool(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required StaleRequestSupport attribute: cancel"
      );
    }

    iter = object.find("retryOnContentModified");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("StaleRequestSupport::retryOnContentModified", array, LSPAnyType::LSP_ARRAY);
      std::vector<std::string> retryOnContentModified;
      for (const std::unique_ptr<LSPAny> &value
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        retryOnContentModified.push_back(anyToString(*value));
      }
      capabilities->retryOnContentModified = std::move(retryOnContentModified);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required StaleRequestSupport attribute: retryOnContentModified"
      );
    }

    return capabilities;
  }

  auto LspTransformer::anyToRegularExpressionsClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<RegularExpressionsClientCapabilities> {
    assertAnyType(
      "RegularExpressionsClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<RegularExpressionsClientCapabilities> capabilities =
      std::make_unique<RegularExpressionsClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("engine");
    if (iter != object.end()) {
      capabilities->engine = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required RegularExpressionsClientCapabilities attribute: engine"
      );
    }

    iter = object.find("version");
    if (iter != object.end()) {
      capabilities->version = anyToString(*iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToMarkdownClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<MarkdownClientCapabilities> {
    assertAnyType(
      "MarkdownClientCapabilities",
      any, LSPAnyType::LSP_OBJECT
    );

    std::unique_ptr<MarkdownClientCapabilities> capabilities =
      std::make_unique<MarkdownClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("parser");
    if (iter != object.end()) {
      capabilities->parser = anyToString(*iter->second);
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        "Missing required MarkdownClientCapabilities attribute: parser"
      );
    }

    iter = object.find("version");
    if (iter != object.end()) {
      capabilities->version = anyToString(*iter->second);
    }

    iter = object.find("allowedTags");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("MarkdownClientCapabilities::allowedTags", array, LSPAnyType::LSP_ARRAY);
      std::vector<std::string> allowedTags;
      for (const std::unique_ptr<LSPAny> &tag
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        allowedTags.push_back(anyToString(*tag));
      }
      capabilities->allowedTags = std::move(allowedTags);
    }

    return capabilities;
  }

  auto LspTransformer::anyToPositionEncodingKind(
    const LSPAny &any
  ) const -> PositionEncodingKind {
    const std::string value = anyToString(any);
    try {
      return positionEncodingKindByValue(value);
    } catch (std::invalid_argument &e) {
      throw LspException(ErrorCodes::InvalidParams, e.what());
    }
  }

  auto LspTransformer::anyToGeneralCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<GeneralCapabilities> {
    assertAnyType("GeneralCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<GeneralCapabilities> capabilities =
      std::make_unique<GeneralCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("staleRequestSupport");
    if (iter != object.end()) {
      capabilities->staleRequestSupport =
        anyToStaleRequestSupport(*iter->second);
    }

    iter = object.find("regularExpressions");
    if (iter != object.end()) {
      capabilities->regularExpressions =
        anyToRegularExpressionsClientCapabilities(*iter->second);
    }

    iter = object.find("markdown");
    if (iter != object.end()) {
      capabilities->markdown =
        anyToMarkdownClientCapabilities(*iter->second);
    }

    iter = object.find("positionEncodings");
    if (iter != object.end()) {
      const LSPAny &array = *iter->second;
      assertAnyType("GeneralCapabilities::positionEncodings", array, LSPAnyType::LSP_ARRAY);
      std::vector<PositionEncodingKind> positionEncodings;
      for (const std::unique_ptr<LSPAny> &encoding
             : *std::get<std::unique_ptr<LSPArray>>(array.value)) {
        positionEncodings.push_back(anyToPositionEncodingKind(*encoding));
      }
      capabilities->positionEncodings = std::move(positionEncodings);
    }

    return capabilities;
  }

  auto LspTransformer::anyToClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ClientCapabilities> {
    assertAnyType("ClientCapabilities", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<ClientCapabilities> capabilities =
      std::make_unique<ClientCapabilities>();

    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("workspace");
    if (iter != object.end()) {
      capabilities->workspace = anyToWorkspaceCapabilities(*iter->second);
    }

    iter = object.find("textDocument");
    if (iter != object.end()) {
      capabilities->textDocument =
        anyToTextDocumentClientCapabilities(*iter->second);
    }

    iter = object.find("notebookDocument");
    if (iter != object.end()) {
      capabilities->notebookDocument =
        anyToNotebookDocumentClientCapabilities(*iter->second);
    }

    iter = object.find("window");
    if (iter != object.end()) {
      capabilities->window = anyToWindowCapabilities(*iter->second);
    }

    iter = object.find("general");
    if (iter != object.end()) {
      capabilities->general = anyToGeneralCapabilities(*iter->second);
    }

    iter = object.find("experimental");
    if (iter != object.end()) {
      capabilities->experimental = copy(iter->second);
    }

    return capabilities;
  }

  auto LspTransformer::anyToTextDocumentItem(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentItem> {
    assertAnyType("TextDocumentItem", any, LSPAnyType::LSP_OBJECT);

    std::unique_ptr<TextDocumentItem> textDocumentItem =
      std::make_unique<TextDocumentItem>();

    LSPObject &value = *std::get<std::unique_ptr<LSPObject>>(any.value);

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
  ) const -> TextDocumentContentChangeEvent {
    assertAnyType("TextDocumentContentChangeEvent", any, LSPAnyType::LSP_OBJECT);
    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);

    auto iter = object.find("range");
    if (iter != object.end()) {
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

      return value;
    } else {
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

      return value;
    }
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
    switch (static_cast<LSPAnyType>(any.value.index())) {
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
    switch (static_cast<LSPAnyType>(any.value.index())) {
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
    if (static_cast<LSPAnyType>(any.value.index()) != type) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAnyType for a(n) {} must be of type {} but received type {}",
          name,
          static_cast<int>(type),
          any.value.index()
        )
      );
    }
  }

  auto LspTransformer::assertRequestType(
    const std::string &method,
    const MessageParams &params,
    MessageParamsType type
  ) const -> void {
    if (static_cast<MessageParamsType>(params.index()) != type) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "MessageParamsType must be {} for method=\"{}\" but received type {}",
          static_cast<int>(type),
          method,
          params.index()
        )
      );
    }
  }

  auto LspTransformer::assertNotificationType(
    const std::string &method,
    const MessageParams &params,
    MessageParamsType type
  ) const -> void {
    if (static_cast<MessageParamsType>(params.index()) != type) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "MessageParamsType must be {} for method=\"{}\" but received type {}",
          static_cast<int>(type),
          method,
          params.index()
        )
      );
    }
  }

  auto LspTransformer::asCancelParams(
    const MessageParams &notificationParams
  ) const -> CancelParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::CANCEL_REQUEST
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    CancelParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

    auto iter = object.find("id");
    if (iter != object.end()) {
      params.id = anyToRequestId(*iter->second);
    }

    return params;
  }

  auto LspTransformer::asSetTraceParams(
    const MessageParams &notificationParams
  ) const -> SetTraceParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::SET_TRACE
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    SetTraceParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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
    const std::optional<MessageParams> &notificationParams
  ) const -> InitializedParams {
    InitializedParams params;
    // Nothing to initialize at the moment ...
    return params;
  }

  auto LspTransformer::asDidOpenNotebookDocumentParams(
    const MessageParams &notificationParams
  ) const -> DidOpenNotebookDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_OPEN_NOTEBOOK_DOCUMENT
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    DidOpenNotebookDocumentParams params;

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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
      assertAnyType("DidOpenNotebookDocumentParams::cellTextDocuments", anyDocs, LSPAnyType::LSP_ARRAY);
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
    const MessageParams &notificationParams
  ) const -> DidChangeNotebookDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_CHANGE_NOTEBOOK_DOCUMENT
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    DidChangeNotebookDocumentParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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
    const MessageParams &notificationParams
  ) const -> DidSaveNotebookDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_SAVE_NOTEBOOK_DOCUMENT
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    DidSaveNotebookDocumentParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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
    const MessageParams &notificationParams
  ) const -> DidCloseNotebookDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_CLOSE_NOTEBOOK_DOCUMENT
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    DidCloseNotebookDocumentParams params;

    const LSPObject &object =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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
      assertAnyType("DidCloseNotebookDocumentParams::cellTextDocuments", anyCellTextDocuments, LSPAnyType::LSP_ARRAY);
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
    const MessageParams &notificationParams
  ) const -> DidOpenTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_OPEN_TEXT_DOCUMENT
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    DidOpenTextDocumentParams params;

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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
    const MessageParams &notificationParams
  ) const -> DidChangeTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_CHANGE_TEXT_DOCUMENT
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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
      assertAnyType("DidChangeTextDocumentParams::contentChanges", anyContentChanges, LSPAnyType::LSP_ARRAY);
      LSPArray &anyChanges =
        *std::get<std::unique_ptr<LSPArray>>(anyContentChanges.value);
      for (const auto &anyChange : anyChanges) {
        TextDocumentContentChangeEvent change =
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
    const MessageParams &notificationParams
  ) const -> DidSaveTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_SAVE_TEXT_DOCUMENT
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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
    const MessageParams &notificationParams
  ) const -> DidCloseTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_CLOSE_TEXT_DOCUMENT
      ),
      notificationParams,
      MessageParamsType::LSP_OBJECT
    );

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams);

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

  auto LspTransformer::asMessageParams(
    const RegistrationParams &registrationParams
  ) const -> MessageParams {
    return lspToObject(registrationParams);
  }

  auto LspTransformer::asMessageParams(
    const UnregistrationParams &unregistrationParams
  ) const -> MessageParams {
    return lspToObject(unregistrationParams);
  }

  auto LspTransformer::asMessageParams(
    const ProgressParams &progressParams
  ) const -> MessageParams {
    return lspToObject(progressParams);
  }

  auto LspTransformer::asMessageParams(
    const LogTraceParams &logTraceParams
  ) const -> MessageParams {
    return lspToObject(logTraceParams);
  }

  auto LspTransformer::lspToAny(
    const FindReferencesResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();

    switch (static_cast<FindReferencesResultType>(result.index())) {
    case FindReferencesResultType::LOCATION_ARRAY: {
      std::unique_ptr<LSPArray> locations = std::make_unique<LSPArray>();
      for (const std::unique_ptr<Location> &location
             : std::get<ptr_vector<Location>>(result)) {
        locations->push_back(lspToAny(*location));
      }
      any->value = std::move(locations);
      break;
    }
    case FindReferencesResultType::LSP_NULL: {
      any->value = nullptr;
      break;
    }
    }

    return any;
  }

  auto LspTransformer::lspToAny(
    const PrepareCallHierarchyResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();

    switch (static_cast<PrepareCallHierarchyResultType>(result.index())) {
    case PrepareCallHierarchyResultType::CALL_HIERARCHY_ITEM_ARRAY: {
      std::unique_ptr<LSPArray> items = std::make_unique<LSPArray>();
      for (const std::unique_ptr<CallHierarchyItem> &item
             : std::get<ptr_vector<CallHierarchyItem>>(result)) {
        items->push_back(lspToAny(*item));
      }
      any->value = std::move(items);
      break;
    }
    case PrepareCallHierarchyResultType::LSP_NULL: {
      any->value = nullptr;
      break;
    }
    }

    return any;
  }

  auto LspTransformer::lspToAny(
    const CallHierarchyItem &item
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(item);
    return any;
  }

  auto LspTransformer::lspToObject(
    const CallHierarchyItem &item
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("name", stringToAny(item.name));
    object->emplace("kind", intToAny(static_cast<int>(item.kind)));
    if (item.tags.has_value()) {
      std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
      std::unique_ptr<LSPArray> array = std::make_unique<LSPArray>();
      for (SymbolTag tag : item.tags.value()) {
        array->push_back(intToAny(static_cast<int>(tag)));
      }
      any->value = std::move(array);
      object->emplace("tags", std::move(any));
    }
    if (item.detail.has_value()) {
      object->emplace("detail", stringToAny(item.detail.value()));
    }
    object->emplace("uri", stringToAny(item.uri));
    object->emplace("range", lspToAny(*item.range));
    object->emplace("selectionRange", lspToAny(*item.selectionRange));
    if (item.data.has_value()) {
      object->emplace("data", copy(item.data.value()));
    }
    return object;
  }

  auto LspTransformer::lspToAny(
    const ProgressToken &token
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    switch (static_cast<ProgressTokenType>(token.index())) {
    case ProgressTokenType::LSP_INTEGER: {
      any->value = std::get<int>(token);
      break;
    }
    case ProgressTokenType::LSP_STRING: {
      any->value = std::get<std::string>(token);
      break;
    }
    }
    return any;
  }

  auto LspTransformer::lspToAny(
    const ProgressParams &params
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(params);
    return any;
  }

  auto LspTransformer::lspToAny(
    const InitializeResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    std::unique_ptr<LSPObject> value = std::make_unique<LSPObject>();
    value->insert({"capabilities", lspToAny(*result.capabilities)});
    any->value = std::move(value);
    return any;
  }

  auto LspTransformer::lspToAny(
    const WillSaveWaitUntilResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    switch (static_cast<WillSaveWaitUntilResultType>(result.index())) {
    case WillSaveWaitUntilResultType::TEXT_EDITS: {
      std::unique_ptr<LSPArray> edits;
      for (const std::unique_ptr<TextEdit> &edit
             : std::get<ptr_vector<TextEdit>>(result)) {
        edits->push_back(lspToAny(*edit));
      }
      any->value = std::move(edits);
      break;
    }
    case WillSaveWaitUntilResultType::LSP_NULL: {
      any->value = nullptr;
      break;
    }
    }
    return any;
  }

  auto LspTransformer::lspToAny(
    const GotoResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();

    switch (static_cast<GotoResultType>(result.index())) {
    case GotoResultType::LOCATION: {
      any->value = lspToObject(*std::get<std::unique_ptr<Location>>(result));
      break;
    }
    case GotoResultType::LOCATION_ARRAY: {
      std::unique_ptr<LSPArray> locations = std::make_unique<LSPArray>();
      for (const std::unique_ptr<Location> &location
             : std::get<ptr_vector<Location>>(result)) {
        locations->push_back(lspToAny(*location));
      }
      any->value = std::move(locations);
      break;
    }
    case GotoResultType::LOCATION_LINK_ARRAY: {
      std::unique_ptr<LSPArray> links = std::make_unique<LSPArray>();
      for (const std::unique_ptr<LocationLink> &link
             : std::get<ptr_vector<LocationLink>>(result)) {
        links->push_back(lspToAny(*link));
      }
      any->value = std::move(links);
      break;
    }
    case GotoResultType::LSP_NULL: {
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
    std::unique_ptr<LSPObject> value = std::make_unique<LSPObject>();
    if (capabilities.textDocumentSync.has_value()) {
      (*value)["textDocumentSync"] =
        lspToAny(capabilities.textDocumentSync.value());
    }
    any->value = std::move(value);
    return any;
  }

  auto LspTransformer::lspToAny(
    const TextDocumentSync &textDocumentSync
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    switch (static_cast<TextDocumentSyncType>(textDocumentSync.index())) {
    case TextDocumentSyncType::TEXT_DOCUMENT_SYNC_KIND: {
      any->value = static_cast<uinteger>(
        std::get<TextDocumentSyncKind>(textDocumentSync)
      );
      break;
    }
    case TextDocumentSyncType::TEXT_DOCUMENT_SYNC_OPTIONS: {
      any->value = lspToObject(
        *std::get<std::unique_ptr<TextDocumentSyncOptions>>(textDocumentSync)
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
    any->value = lspToObject(edit);
    return any;
  }

  auto LspTransformer::lspToAny(
    const LocationLink &link
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(link);
    return any;
  }

  auto LspTransformer::lspToAny(
    const Location &location
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(location);
    return any;
  }

  auto LspTransformer::lspToAny(
    const Range &range
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(range);
    return any;
  }

  auto LspTransformer::lspToAny(
    const Position &position
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(position);
    return any;
  }

  auto LspTransformer::intToAny(
    int value
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = value;
    return any;
  }

  auto LspTransformer::unsignedIntToAny(
    uinteger value
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = value;
    return any;
  }

  auto LspTransformer::stringToAny(
    const std::string &value
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = value;
    return any;
  }

  auto LspTransformer::lspToObject(
    const RegistrationParams &registrationParams
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    std::unique_ptr<LSPAny> array = std::make_unique<LSPAny>();
    std::unique_ptr<LSPArray> registrations = std::make_unique<LSPArray>();
    for (const std::unique_ptr<Registration> &registration
           : registrationParams.registrations) {
      registrations->push_back(lspToAny(*registration));
    }
    array->value = std::move(registrations);
    return object;
  }

  auto LspTransformer::lspToObject(
    const UnregistrationParams &unregistrationParams
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    std::unique_ptr<LSPAny> array = std::make_unique<LSPAny>();
    std::unique_ptr<LSPArray> unregistrations = std::make_unique<LSPArray>();
    for (const std::unique_ptr<Unregistration> &unregistration
           : unregistrationParams.unregisterations) {
      unregistrations->push_back(lspToAny(*unregistration));
    }
    array->value = std::move(unregistrations);
    return object;
  }

  auto LspTransformer::lspToAny(
    const Unregistration &unregistration
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(unregistration);
    return any;
  }

  auto LspTransformer::lspToObject(
    const Unregistration &unregistration
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("id", stringToAny(unregistration.id));
    object->emplace("method", stringToAny(unregistration.method));
    return object;
  }

  auto LspTransformer::lspToAny(
    const LogTraceParams &logTraceParams
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(logTraceParams);
    return any;
  }

  auto LspTransformer::lspToObject(
    const LogTraceParams &logTraceParams
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("message", stringToAny(logTraceParams.message));
    if (logTraceParams.verbose.has_value()) {
      object->emplace("verbose", stringToAny(logTraceParams.verbose.value()));
    }
    return object;
  }

  auto LspTransformer::lspToAny(
    const Registration &registration
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->value = lspToObject(registration);
    return any;
  }

  auto LspTransformer::lspToObject(
    const Registration &registration
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("id", stringToAny(registration.id));
    object->emplace("method", stringToAny(registration.method));
    if (registration.registerOptions.has_value()) {
      object->emplace(
        "registerOptions",
        copy(registration.registerOptions.value())
      );
    }
    return object;
  }

  auto LspTransformer::lspToObject(
    const ProgressParams &params
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    object->emplace("token", lspToAny(params.token));
    object->emplace("value", copy(params.value));
    return object;
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
    object->emplace("line", unsignedIntToAny(position.line));
    object->emplace("character", unsignedIntToAny(position.character));
    return object;
  }

  auto LspTransformer::lspToObject(
    const TextDocumentSyncOptions &options
  ) const -> std::unique_ptr<LSPObject> {
    std::unique_ptr<LSPObject> object = std::make_unique<LSPObject>();
    if (options.openClose.has_value()) {
      std::unique_ptr<LSPAny> openClose = std::make_unique<LSPAny>();
      openClose->value = options.openClose.value();
      object->insert({"openClose", std::move(openClose)});
    }
    if (options.change.has_value()) {
      std::unique_ptr<LSPAny> change = std::make_unique<LSPAny>();
      change->value = static_cast<uinteger>(options.change.value());
      object->insert({"change", std::move(change)});
    }
    if (options.willSave.has_value()) {
      std::unique_ptr<LSPAny> willSave = std::make_unique<LSPAny>();
      willSave->value = options.willSave.value();
      object->insert({"willSave", std::move(willSave)});
    }
    if (options.willSaveWaitUntil.has_value()) {
      std::unique_ptr<LSPAny> willSaveWaitUntil = std::make_unique<LSPAny>();
      willSaveWaitUntil->value = options.willSaveWaitUntil.value();
      object->insert({"willSaveWaitUntil", std::move(willSaveWaitUntil)});
    }
    if (options.save.has_value()) {
      std::unique_ptr<LSPAny> save = std::make_unique<LSPAny>();
      const SaveOrOptions &saveOrOptions = options.save.value();
      switch (static_cast<SaveOrOptionsType>(saveOrOptions.index())) {
      case SaveOrOptionsType::BOOLEAN: {
        save->value = std::get<boolean>(saveOrOptions);
        break;
      }
      case SaveOrOptionsType::SAVE_OPTIONS: {
        save->value = lspToObject(
          *std::get<std::unique_ptr<SaveOptions>>(saveOrOptions)
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
      includeText->value = options.includeText.value();
      object->insert({"includeText", std::move(includeText)});
    }
    return object;
  }

  auto LspTransformer::requireMessageParams(
    const RequestMessage &request
  ) const -> const MessageParams & {
    if (request.params.has_value()) {
      return request.params.value();
    }
    throw LspException(
      ErrorCodes::InvalidParams,
      std::format(
        "RequestMessage.params must be defined for method=\"{}\"",
        request.method
      )
    );
  }

  auto LspTransformer::requireMessageParams(
    const NotificationMessage &notification
  ) const -> const MessageParams & {
    if (notification.params.has_value()) {
      return notification.params.value();
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
    switch (static_cast<LSPAnyType>(any->value.index())) {
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
