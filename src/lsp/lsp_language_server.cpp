#include <cmath>
#include <format>
#include <iostream>
#include <mutex>
#include <stdexcept>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  std::string LspLanguageServer::serve(const std::string &request) {
    ResponseMessage response;
    try {
      // The language server protocol always uses “2.0” as the jsonrpc version.
      response.jsonrpc = "2.0";

      response.id = std::make_unique<ResponseId>();
      response.id->type = ResponseIdType::LSP_NULL;
      response.id->value = nullptr;

      rapidjson::Document document = deserializer.deserialize(request);
      if (document.HasParseError()) {
        throw LspException(
          ErrorCodes::ParseError,
          std::format(
            "Invalid JSON request (error={}): {}",
            static_cast<int>(document.GetParseError()),
            request
          )
        );
      }

      if (document.HasMember("id")) {
        const rapidjson::Value &idValue = document["id"];
        if (idValue.IsString()) {
          response.id->type = ResponseIdType::LSP_STRING;
          response.id->value = idValue.GetString();
        } else if (idValue.IsInt()) {
          response.id->type = ResponseIdType::LSP_INTEGER;
          response.id->value = idValue.GetInt();
        } else if (!idValue.IsNull()) { // null => notification
          throw LspException(
            ErrorCodes::InvalidParams,
            std::format(
              "Unsupported type for id attribute: {}",
              static_cast<int>(idValue.GetType())
            )
          );
        }
      }

      if (document.HasMember("method")) {
        const std::string &method = document["method"].GetString();
        if (isRequestMethod(method)) {
          if (response.id->type == ResponseIdType::LSP_NULL) {
            throw LspException(
              ErrorCodes::InvalidParams,
              std::format(
                "Missing request method=\"{}\" attribute: id",
                method
              )
            );
          }
          RequestMessage request = deserializer.deserializeRequest(document);
          response.jsonrpc = request.jsonrpc;
          dispatch(response, request);
        } else if (isNotificationMethod(method)) {
          if (response.id->type != ResponseIdType::LSP_NULL) {
            throw LspException(
              ErrorCodes::InvalidParams,
              std::format(
                "Notification method=\"{}\" must not contain the attribute: id",
                method
              )
            );
          }
          NotificationMessage notification =
            deserializer.deserializeNotification(document);
          response.jsonrpc = notification.jsonrpc;
          dispatch(response, notification);
        } else {
          throw LspException(
            ErrorCodes::InvalidRequest,
            std::format("Unsupported method: {}", method)
          );
        }
      } else {
        throw LspException(
          ErrorCodes::InvalidRequest,
          "Missing required attribute: method"
        );
      }
    } catch (const LspException &e) {
      const std::source_location &where = e.where();
      std::cerr
        << "[" << where.file_name() << ":" << where.line() << ":" << where.column() << "] "
        << e.what()
        << std::endl;
      std::unique_ptr<ResponseError> error =
        std::make_unique<ResponseError>();
      error->code = static_cast<int>(e.code());
      error->message = e.what();
      response.error = std::move(error);
    } catch (const std::exception &e) {
      std::cerr << "Caught unhandled exception: " << e.what() << std::endl;
      std::unique_ptr<ResponseError> error =
        std::make_unique<ResponseError>();
      error->code = static_cast<int>(ErrorCodes::InternalError);
      error->message =
        "An unexpected exception occurred. If it continues, please file a ticket.";
      response.error = std::move(error);
    }
    return serializer.serializeResponse(response);
  }

  auto LspLanguageServer::dispatch(
    ResponseMessage &response,
    const RequestMessage &request
  ) -> void {
    RequestMethod method;
    try {
      method = requestMethodByValue(request.method);
    } catch (std::invalid_argument &e) {
      goto invalidMethod;
    }
    if (method != RequestMethod::INITIALIZE) {
      assertInitialized();
    }
    switch (method) {
    case RequestMethod::INITIALIZE: {
      const RequestParams &requestParams =
        requireRequestParams(request);
      InitializeParams params =
        asInitializeParams(requestParams);
      InitializeResult result = initialize(params);
      resultToResponseMessage(response, result);
      break;
    }
    default: {
    invalidMethod:
      throw LspException(
        ErrorCodes::MethodNotFound,
        std::format(
          "Unsupported request method: \"{}\"",
          request.method
        )
      );
    }
    }
  }

  auto LspLanguageServer::dispatch(
    ResponseMessage &response,
    const NotificationMessage &notification
  ) -> void {
    assertInitialized();

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
      throw LspException(
        ErrorCodes::MethodNotFound,
        std::format(
          "Unsupported notification method: \"{}\"",
          notification.method
        )
      );
    }
    }
  }

  void LspLanguageServer::assertInitialized() {
    if (!_initialized) {
      throw LspException(
        ErrorCodes::ServerNotInitialized,
        "Request method=\"initialize\" has not been called yet!"
      );
    }
  }

  // request: "initialize"
  auto LspLanguageServer::initialize(
    const InitializeParams &params
  ) -> InitializeResult {
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

    _initialized = true;

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
    {
      std::unique_lock<std::shared_mutex> writeLock(readWriteMutex);
      textDocuments.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(uri),
        std::forward_as_tuple(uri, text)
      );
    }
  }

  // notification: "textDocument/didChange"
  void LspLanguageServer::didChangeTextDocument(
    DidChangeTextDocumentParams &params
  ) {
    const DocumentUri &uri = params.textDocument->uri;
    {
      std::shared_lock<std::shared_mutex> readLock(readWriteMutex);
      TextDocument &textDocument = textDocuments.at(uri);
      readLock.unlock();
      textDocument.apply(params.contentChanges);
    }
  }

  // notification: "textDocument/didSave"
  void LspLanguageServer::didSaveTextDocument(
    const DidSaveTextDocumentParams &params
  ) {
    if (params.text.has_value()) {
      const std::string &text = params.text.value();
      const DocumentUri &uri = params.textDocument->uri;
      {
        std::shared_lock<std::shared_mutex> readLock(readWriteMutex);
        TextDocument &textDocument = textDocuments.at(uri);
        readLock.unlock();
        textDocument.setText(text);
      }
    }
  }

  // notification: "textDocument/didClose"
  void LspLanguageServer::didCloseTextDocument(
    const DidCloseTextDocumentParams &params
  ) {
    const DocumentUri &uri = params.textDocument->uri;
    {
      std::shared_lock<std::shared_mutex> readLock(readWriteMutex);
      auto pos = textDocuments.find(uri);
      readLock.unlock();
      if (pos != textDocuments.end()) {
        std::unique_lock<std::shared_mutex> writeLock(readWriteMutex);
        pos = textDocuments.find(uri);
        if (pos != textDocuments.end()) {
          textDocuments.erase(pos);
        }
      }
    }
  }

  auto LspLanguageServer::resultToResponseMessage(
    ResponseMessage &response,
    const InitializeResult &result
  ) const -> void {
    response.result = lspToAny(result);
  }

  auto LspLanguageServer::asInitializeParams(
    const RequestParams &requestParams
  ) const -> InitializeParams {
    InitializeParams initializeParams;
    if (requestParams.type == RequestParamsType::LSP_OBJECT) {
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
    } else {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "Expected request parameters to be of type {} but received {}",
          static_cast<int>(RequestParamsType::LSP_OBJECT),
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

  auto LspLanguageServer::anyToTextDocumentContentChangeEvent(
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

  auto LspLanguageServer::anyToRange(
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

  auto LspLanguageServer::anyToPosition(
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

  auto LspLanguageServer::anyToString(
    const LSPAny &any
  ) const -> const std::string & {
    assertAnyType("string", any, LSPAnyType::LSP_STRING);
    return std::get<std::string>(any.value);
  }

  auto LspLanguageServer::anyToInt(
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
      "LspLanguageServer::anyToInt: This should not be reached."
    );
  }

  auto LspLanguageServer::anyToUnsignedInt(
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
      "LspLanguageServer::anyToUnsignedInt: This should not be reached."
    );
  }

  auto LspLanguageServer::assertAnyType(
    const std::string &name,
    const LSPAny &any,
    LSPAnyType type
  ) const -> void {
    if (any.type != type) {
      throw LspException(
        ErrorCodes::InvalidParams,
        std::format(
          "LSPAny.type for a(n) {} must be of type {} but received type {}",
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
      throw LspException(
        ErrorCodes::InvalidParams,
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
    throw LspException(
      ErrorCodes::InvalidParams,
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
    throw LspException(
      ErrorCodes::InvalidParams,
      std::format(
        "NotificationMessage.params must be defined for method=\"{}\"",
        notification.method
      )
    );
  }

} // namespace LCompilers::LanguageServerProtocol
