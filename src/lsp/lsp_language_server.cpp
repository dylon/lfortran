#include <format>
#include <stdexcept>

#include <lsp/specification.h>
#include <lsp/lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  std::string LspLanguageServer::serve(const std::string &request) {
    rapidjson::Document document = deserializer.deserialize(request);
    ResponseMessage response;
    if (document.HasMember("id")) {
      RequestMessage request = deserializer.deserializeRequest(document);
      response = dispatch(request);
      response.jsonrpc = request.jsonrpc;
      std::unique_ptr<ResponseId> responseId = std::make_unique<ResponseId>();
      switch (request.id->type) {
      case RequestIdType::LSP_INTEGER: {
        responseId->type = ResponseIdType::LSP_INTEGER;
        responseId->value = std::get<integer>(request.id->value);
        break;
      }
      case RequestIdType::LSP_STRING: {
        responseId->type = ResponseIdType::LSP_STRING;
        responseId->value = std::get<string>(request.id->value);
        break;
      }
      }
      response.id = std::move(responseId);
    } else {
      NotificationMessage notification =
        deserializer.deserializeNotification(document);
      response = dispatch(notification);
      response.jsonrpc = notification.jsonrpc;
      response.id = std::make_unique<ResponseId>();
      response.id->type = ResponseIdType::LSP_NULL;
      response.id->value = nullptr;
    }
    return serializer.serializeResponse(response);
  }

  auto LspLanguageServer::dispatch(
    const RequestMessage &request
  ) -> ResponseMessage {
    ResponseMessage response;
    RequestMethod method;
    try {
      method = requestMethodByValue(request.method);
    } catch (std::invalid_argument &e) {
      goto invalidMethod;
    }
    switch (method) {
    case RequestMethod::INITIALIZE: {
      const RequestParams &requestParams =
        requireRequestParams(request);
      InitializeParams params =
        asInitializeParams(requestParams);
      InitializeResult result = initialize(params);
      response = resultToResponseMessage(result);
      break;
    }
    default: {
    invalidMethod:
      std::unique_ptr<ResponseError> error =
        std::make_unique<ResponseError>();
      error->code = static_cast<int>(ErrorCodes::MethodNotFound);
      error->message = std::format(
        "Unsupported request method: \"{}\"",
        request.method
      );
      response.error = std::move(error);
    }
    }
    return response;
  }

  auto LspLanguageServer::dispatch(
    const NotificationMessage &notification
  ) -> ResponseMessage {
    ResponseMessage response;
    NotificationMethod method;
    try {
      method = notificationMethodByValue(notification.method);
    } catch (std::invalid_argument &e) {
      goto invalidMethod;
    }
    switch (method) {
    case NotificationMethod::INITIALIZED: {
      InitializedParams params =
        asInitializedParams(notification.params);
      initialized(params);
      break;
    }
    case NotificationMethod::DID_OPEN_TEXT_DOCUMENT: {
      const NotificationParams &notificationParams =
        requireNotificationParams(notification);
      DidOpenTextDocumentParams params =
        asDidOpenTextDocumentParams(notificationParams);
      didOpenTextDocument(params);
      break;
    }
    case NotificationMethod::DID_CHANGE_TEXT_DOCUMENT: {
      const NotificationParams &notificationParams =
        requireNotificationParams(notification);
      DidChangeTextDocumentParams params =
        asDidChangeTextDocumentParams(notificationParams);
      didChangeTextDocument(params);
      break;
    }
    case NotificationMethod::DID_SAVE_TEXT_DOCUMENT: {
      const NotificationParams &notificationParams =
        requireNotificationParams(notification);
      DidSaveTextDocumentParams params =
        asDidSaveTextDocumentParams(notificationParams);
      didSaveTextDocument(params);
      break;
    }
    case NotificationMethod::DID_CLOSE_TEXT_DOCUMENT: {
      const NotificationParams &notificationParams =
        requireNotificationParams(notification);
      DidCloseTextDocumentParams params =
        asDidCloseTextDocumentParams(notificationParams);
      didCloseTextDocument(params);
      break;
    }
    default: {
    invalidMethod:
      std::unique_ptr<ResponseError> error =
        std::make_unique<ResponseError>();
      error->code = static_cast<int>(ErrorCodes::MethodNotFound);
      error->message = std::format(
        "Unsupported notification method: \"{}\"",
        notification.method
      );
      response.error = std::move(error);
    }
    }
    return response;
  }

  // request: "initialize"
  InitializeResult LspLanguageServer::initialize(
    const InitializeParams &params
  ) {
    InitializeResult result;
    std::unique_ptr<ServerCapabilities> capabilities =
      std::make_unique<ServerCapabilities>();

    // ------------------------- //
    // TextDocument Sync Options //
    // ------------------------- //
    std::unique_ptr<TextDocumentSync> textDocumentSync =
      std::make_unique<TextDocumentSync>();
    textDocumentSync->type = TextDocumentSyncType::TEXT_DOCUMENT_SYNC_OPTIONS;
    std::unique_ptr<TextDocumentSyncOptions> textDocumentSyncOptions =
      std::make_unique<TextDocumentSyncOptions>();
    textDocumentSyncOptions->openClose = true;
    textDocumentSyncOptions->change = TextDocumentSyncKind::Incremental;
    std::unique_ptr<SaveOrOptions> save = std::make_unique<SaveOrOptions>();
    save->type = SaveOrOptionsType::SAVE_OPTIONS;
    std::unique_ptr<SaveOptions> saveOptions = std::make_unique<SaveOptions>();
    saveOptions->includeText = true;
    save->value = std::move(saveOptions);
    textDocumentSyncOptions->save = std::move(save);
    textDocumentSync->value = std::move(textDocumentSyncOptions);
    capabilities->textDocumentSync = std::move(textDocumentSync);

    result.capabilities = std::move(capabilities);
    return result;
  }

  // notification: "initialized"
  void LspLanguageServer::initialized(
    const InitializedParams &params
  ) {
    // empty
  }

  // notification: "textDocument/didOpen"
  void LspLanguageServer::didOpenTextDocument(
    const DidOpenTextDocumentParams &params
  ) {
    const TextDocumentItem &textDocumentItem = *params.textDocument;
    const DocumentUri &uri = textDocumentItem.uri;
    const std::string &text = textDocumentItem.text;
    textDocuments.insert({uri, TextDocument(uri, text)});
  }

  // notification: "textDocument/didChange"
  void LspLanguageServer::didChangeTextDocument(
    DidChangeTextDocumentParams &params
  ) {
    const DocumentUri &uri = params.textDocument->uri;
    TextDocument &textDocument = textDocuments.at(uri);
    textDocument.apply(params.contentChanges);
  }

  // notification: "textDocument/didSave"
  void LspLanguageServer::didSaveTextDocument(
    const DidSaveTextDocumentParams &params
  ) {
    if (params.text.has_value()) {
      const std::string &text = params.text.value();
      const DocumentUri &uri = params.textDocument->uri;
      TextDocument &textDocument = textDocuments.at(uri);
      textDocument.setText(text);
    }
  }

  // notification: "textDocument/didClose"
  void LspLanguageServer::didCloseTextDocument(
    const DidCloseTextDocumentParams &params
  ) {
    const DocumentUri &uri = params.textDocument->uri;
    auto pos = textDocuments.find(uri);
    if (pos != textDocuments.end()) {
      textDocuments.erase(pos);
    }
  }

  auto LspLanguageServer::resultToResponseMessage(
    const InitializeResult &result
  ) const -> ResponseMessage {
    ResponseMessage response;
    response.result = lspToAny(result);
    return response;
  }

  auto LspLanguageServer::asInitializeParams(
    const RequestParams &requestParams
  ) const -> InitializeParams {
    InitializeParams initializeParams;
    if (requestParams.type == RequestParamsType::LSP_OBJECT) {
      LSPObject &lspObject =
        *std::get<std::unique_ptr<LSPObject>>(requestParams.value);
      if (lspObject.contains("capabilities")) {
        initializeParams.capabilities =
          anyToClientCapabilities(*lspObject.at("capabilities"));
      } else {
        throw std::logic_error(
          "Missing required attribute: InitializeParams.capabilities"
        );
      }
    } else {
      throw std::logic_error(
        std::format(
          "Unsupported RequestParams.type: {}",
          static_cast<int>(requestParams.type)
        )
      );
    }
    return initializeParams;
  }

  auto LspLanguageServer::anyToClientCapabilities(
    const LSPAny &any
  ) const -> std::unique_ptr<ClientCapabilities> {
    std::unique_ptr<ClientCapabilities> capabilities =
      std::make_unique<ClientCapabilities>();
    return capabilities;
  }

  auto LspLanguageServer::anyToTextDocumentItem(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentItem> {
    assertAnyType("TextDocumentItem", any, LSPAnyType::LSP_OBJECT);
    LSPObject &value = *std::get<std::unique_ptr<LSPObject>>(any.value);
    std::unique_ptr<TextDocumentItem> textDocumentItem =
      std::make_unique<TextDocumentItem>();
    textDocumentItem->uri = anyToString(*value.at("uri"));
    textDocumentItem->languageId = anyToString(*value.at("languageId"));
    textDocumentItem->version = anyToInt(*value.at("version"));
    textDocumentItem->text = anyToString(*value.at("text"));
    return textDocumentItem;
  }

  auto LspLanguageServer::anyToTextDocumentIdentifier(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentIdentifier> {
    assertAnyType(
      "TextDocumentIdentifier",
      any, LSPAnyType::LSP_OBJECT
    );
    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);
    std::unique_ptr<TextDocumentIdentifier> textDocumentId =
      std::make_unique<TextDocumentIdentifier>();
    textDocumentId->uri = anyToString(*object.at("uri"));
    return textDocumentId;
  }

  auto LspLanguageServer::anyToVersionedTextDocumentIdentifier(
    const LSPAny &any
  ) const -> std::unique_ptr<VersionedTextDocumentIdentifier> {
    assertAnyType(
      "VersionedTextDocumentIdentifier",
      any, LSPAnyType::LSP_OBJECT
    );
    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);
    std::unique_ptr<VersionedTextDocumentIdentifier> textDocumentId =
      std::make_unique<VersionedTextDocumentIdentifier>();
    textDocumentId->uri = anyToString(*object.at("uri"));
    textDocumentId->version = anyToInt(*object.at("version"));
    return textDocumentId;
  }

  auto LspLanguageServer::anyToTextDocumentContentChangeEvent(
    const LSPAny &any
  ) const -> std::unique_ptr<TextDocumentContentChangeEvent> {
    assertAnyType("TextDocumentContentChangeEvent", any, LSPAnyType::LSP_OBJECT);
    LSPObject &object = *std::get<std::unique_ptr<LSPObject>>(any.value);
    std::unique_ptr<TextDocumentContentChangeEvent> change =
      std::make_unique<TextDocumentContentChangeEvent>();
    return change;
  }

  auto LspLanguageServer::anyToString(
    const LSPAny &any
  ) const -> const std::string & {
    assertAnyType("string", any, LSPAnyType::LSP_STRING);
    return std::get<std::string>(any.value);
  }

  auto LspLanguageServer::anyToInt(
    const LSPAny &any
  ) const -> int {
    assertAnyType("int", any, LSPAnyType::LSP_INTEGER);
    return std::get<int>(any.value);
  }

  auto LspLanguageServer::assertAnyType(
    const std::string &name,
    const LSPAny &any,
    LSPAnyType type
  ) const -> void {
    if (any.type != type) {
      throw std::logic_error(
        std::format(
          "LSPAny.type for a {} must be of type {} but received type {}",
          name,
          static_cast<int>(type),
          static_cast<int>(any.type)
        )
      );
    }
  }

  auto LspLanguageServer::assertNotificationType(
    const std::string &method,
    const NotificationParams &params,
    NotificationParamsType type
  ) const -> void {
    if (params.type != type) {
      throw std::logic_error(
        std::format(
          "NotificationParams.type must be {} for method=\"{}\" but received type {}",
          static_cast<int>(type),
          method,
          static_cast<int>(params.type)
        )
      );
    }
  }

  auto LspLanguageServer::asInitializedParams(
    const optional_ptr<NotificationParams> &notificationParams
  ) const -> InitializedParams {
    InitializedParams params;
    // Nothing to initialize at the moment ...
    return params;
  }

  auto LspLanguageServer::asDidOpenTextDocumentParams(
    const NotificationParams &notificationParams
  ) const -> DidOpenTextDocumentParams {
    assertNotificationType(
      NotificationMethodValues.at(
        NotificationMethod::DID_OPEN_TEXT_DOCUMENT
      ),
      notificationParams,
      NotificationParamsType::LSP_OBJECT
    );

    const LSPObject &value =
      *std::get<std::unique_ptr<LSPObject>>(notificationParams.value);
    const LSPAny &textDocumentItem = *value.at("textDocumentItem");

    DidOpenTextDocumentParams params;
    params.textDocument = anyToTextDocumentItem(textDocumentItem);
    return params;
  }

  auto LspLanguageServer::asDidChangeTextDocumentParams(
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

    const LSPAny &textDocument = *value.at("textDocument");
    params.textDocument = anyToVersionedTextDocumentIdentifier(textDocument);

    const LSPAny &anyContentChanges = *value.at("contentChanges");
    assertAnyType("std::vector", anyContentChanges, LSPAnyType::LSP_ARRAY);
    LSPArray &anyChanges =
      *std::get<std::unique_ptr<LSPArray>>(anyContentChanges.value);
    for (const auto &anyChange : anyChanges) {
      std::unique_ptr<TextDocumentContentChangeEvent> change =
        anyToTextDocumentContentChangeEvent(*anyChange);
      params.contentChanges.push_back(std::move(change));
    }

    return params;
  }

  auto LspLanguageServer::asDidSaveTextDocumentParams(
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

    const LSPAny &textDocument = *value.at("textDocument");
    params.textDocument = anyToTextDocumentIdentifier(textDocument);

    const LSPAny &text = *value.at("text");
    params.text = anyToString(text);

    return params;
  }

  auto LspLanguageServer::asDidCloseTextDocumentParams(
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

    const LSPAny &textDocument = *value.at("textDocument");
    params.textDocument = anyToTextDocumentIdentifier(textDocument);

    return params;
  }

  auto LspLanguageServer::lspToAny(
    const InitializeResult &result
  ) const -> std::unique_ptr<LSPAny> {
    std::unique_ptr<LSPAny> any = std::make_unique<LSPAny>();
    any->type = LSPAnyType::LSP_OBJECT;
    std::unique_ptr<LSPObject> value = std::make_unique<LSPObject>();
    value->insert({"capabilities", lspToAny(*result.capabilities)});
    any->value = std::move(value);
    return any;
  }

  auto LspLanguageServer::lspToAny(
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

  auto LspLanguageServer::lspToAny(
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

  auto LspLanguageServer::lspToObject(
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

  auto LspLanguageServer::lspToObject(
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

  auto LspLanguageServer::requireRequestParams(
    const RequestMessage &request
  ) const -> const RequestParams & {
    if (request.params.has_value()) {
      return *request.params.value();
    }
    throw std::logic_error(
      std::format(
        "RequestMessage.params must be defined for method=\"{}\"",
        request.method
      )
    );
  }

  auto LspLanguageServer::requireNotificationParams(
    const NotificationMessage &notification
  ) const -> const NotificationParams & {
    if (notification.params.has_value()) {
      return *notification.params.value();
    }
    throw std::logic_error(
      std::format(
        "NotificationMessage.params must be defined for method=\"{}\"",
        notification.method
      )
    );
  }

} // namespace LCompilers::LanguageServerProtocol
