#include <cctype>
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

  auto LspLanguageServer::isInitialized() const -> bool {
    return _initialized;
  }

  auto LspLanguageServer::isShutdown() const -> bool {
    return _shutdown;
  }

  auto LspLanguageServer::isTerminated() const -> bool {
    return _exit;
  }

  auto LspLanguageServer::isRunning() const -> bool {
    return !_shutdown;
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
    assertRunning();
    if (method != RequestMethod::INITIALIZE) {
      assertInitialized();
    } else if (_initialized) {
      throw LspException(
        ErrorCodes::InvalidRequest,
        "Server may be initialized only once."
      );
    }
    switch (method) {
    case RequestMethod::INITIALIZE: {
      const RequestParams &requestParams = transformer.requireRequestParams(request);
      InitializeParams params = transformer.asInitializeParams(requestParams);
      InitializeResult result = initialize(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::WILL_SAVE_WAIT_UNTIL: {
      const RequestParams &requestParams = transformer.requireRequestParams(request);
      WillSaveTextDocumentParams params =
        transformer.asWillSaveTextDocumentParams(requestParams);
      WillSaveWaitUntilResult result = willSaveWaitUntil(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::GOTO_DECLARATION: {
      const RequestParams &requestParams = transformer.requireRequestParams(request);
      DeclarationParams params =
        transformer.asDeclarationParams(requestParams);
      GotoDeclarationResult result = gotoDeclaration(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::GOTO_DEFINITION: {
      const RequestParams &requestParams = transformer.requireRequestParams(request);
      DefinitionParams params =
        transformer.asDefinitionParams(requestParams);
      GotoDefinitionResult result = gotoDefinition(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::SHUTDOWN: {
      shutdown();
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
    NotificationMethod method;
    try {
      method = notificationMethodByValue(notification.method);
    } catch (std::invalid_argument &e) {
      goto invalidMethod;
    }
    assertRunning();
    if (!_initialized && (method != NotificationMethod::EXIT)) {
      // Notifications should be dropped, except for the exit notification. This
      // will allow the exit of a server without an initialize request.
      return;
    }
    switch (method) {
    case NotificationMethod::INITIALIZED: {
      InitializedParams params =
        transformer.asInitializedParams(notification.params);
      initialized(params);
      break;
    }
    case NotificationMethod::CANCEL_REQUEST: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      CancelParams params = transformer.asCancelParams(notificationParams);
      cancelRequest(params);
      break;
    }
    case NotificationMethod::SET_TRACE: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      SetTraceParams params = transformer.asSetTraceParams(notificationParams);
      setTrace(params);
      break;
    }
    case NotificationMethod::DID_OPEN_NOTEBOOK_DOCUMENT: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      DidOpenNotebookDocumentParams params =
        transformer.asDidOpenNotebookDocumentParams(notificationParams);
      didOpenNotebookDocument(params);
      break;
    }
    case NotificationMethod::DID_CHANGE_NOTEBOOK_DOCUMENT: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      DidChangeNotebookDocumentParams params =
        transformer.asDidChangeNotebookDocumentParams(notificationParams);
      didChangeNotebookDocument(params);
      break;
    }
    case NotificationMethod::DID_SAVE_NOTEBOOK_DOCUMENT: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      DidSaveNotebookDocumentParams params =
        transformer.asDidSaveNotebookDocumentParams(notificationParams);
      didSaveNotebookDocument(params);
      break;
    }
    case NotificationMethod::DID_CLOSE_NOTEBOOK_DOCUMENT: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      DidCloseNotebookDocumentParams params =
        transformer.asDidCloseNotebookDocumentParams(notificationParams);
      didCloseNotebookDocument(params);
      break;
    }
    case NotificationMethod::DID_OPEN_TEXT_DOCUMENT: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      DidOpenTextDocumentParams params =
        transformer.asDidOpenTextDocumentParams(notificationParams);
      didOpenTextDocument(params);
      break;
    }
    case NotificationMethod::DID_CHANGE_TEXT_DOCUMENT: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      DidChangeTextDocumentParams params =
        transformer.asDidChangeTextDocumentParams(notificationParams);
      didChangeTextDocument(params);
      break;
    }
    case NotificationMethod::DID_SAVE_TEXT_DOCUMENT: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      DidSaveTextDocumentParams params =
        transformer.asDidSaveTextDocumentParams(notificationParams);
      didSaveTextDocument(params);
      break;
    }
    case NotificationMethod::DID_CLOSE_TEXT_DOCUMENT: {
      const NotificationParams &notificationParams =
        transformer.requireNotificationParams(notification);
      DidCloseTextDocumentParams params =
        transformer.asDidCloseTextDocumentParams(notificationParams);
      didCloseTextDocument(params);
      break;
    }
    case NotificationMethod::EXIT: {
      exit();
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

  auto LspLanguageServer::assertInitialized() -> void{
    if (!_initialized) {
      throw LspException(
        ErrorCodes::ServerNotInitialized,
        "Request method=\"initialize\" has not been called yet!"
      );
    }
  }

  auto LspLanguageServer::assertRunning() -> void {
    if (_shutdown) {
      throw LspException(
        ErrorCodes::RequestFailed,
        "Server has shutdown and cannot accept new requests."
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

  // request: "textDocument/willSaveWaitUntil"
  auto LspLanguageServer::willSaveWaitUntil(
    const WillSaveTextDocumentParams &params
  ) -> WillSaveWaitUntilResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::WILL_SAVE_WAIT_UNTIL)
      )
    );
  }

  // request: "textDocument/declaration"
  auto LspLanguageServer::gotoDeclaration(
    const DeclarationParams &params
  ) -> GotoDeclarationResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::GOTO_DECLARATION)
      )
    );
  }

  // request: "textDocument/definition"
  auto LspLanguageServer::gotoDefinition(
    const DefinitionParams &params
  ) -> GotoDefinitionResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::GOTO_DEFINITION)
      )
    );
  }

  // request: "shutdown"
  auto LspLanguageServer::shutdown() -> void {
    std::cerr << "Shutting down server." << std::endl;
    _shutdown = true;
  }

  // notification: "exit"
  auto LspLanguageServer::exit() -> void {
    std::cerr << "Exiting server." << std::endl;
    _exit = true;
    if (!_shutdown) {
      std::cerr
        << "Server exited before being notified to shutdown!"
        << std::endl;
      _shutdown = true;
    }
  }

  // notification: "$/cancelRequest"
  auto LspLanguageServer::cancelRequest(const CancelParams &params) -> void {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for notification=\"{}\"",
        NotificationMethodValues.at(NotificationMethod::CANCEL_REQUEST)
      )
    );
  }

  // notification: "initialized"
  auto LspLanguageServer::initialized(
    const InitializedParams &params
  ) -> void {
    // empty
  }

  // notification: "$/setTrace"
  auto LspLanguageServer::setTrace(const SetTraceParams &params) -> void {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for notification=\"{}\"",
        NotificationMethodValues.at(NotificationMethod::SET_TRACE)
      )
    );
  }

  // notification: "textDocument/didOpen"
  auto LspLanguageServer::didOpenTextDocument(
    const DidOpenTextDocumentParams &params
  ) -> void {
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
  auto LspLanguageServer::didChangeTextDocument(
    DidChangeTextDocumentParams &params
  ) -> void {
    const DocumentUri &uri = params.textDocument->uri;
    {
      std::shared_lock<std::shared_mutex> readLock(readWriteMutex);
      TextDocument &textDocument = textDocuments.at(uri);
      readLock.unlock();
      textDocument.apply(params.contentChanges);
    }
  }

  // notification: "textDocument/didSave"
  auto LspLanguageServer::didSaveTextDocument(
    const DidSaveTextDocumentParams &params
  ) -> void {
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
  auto LspLanguageServer::didCloseTextDocument(
    const DidCloseTextDocumentParams &params
  ) -> void {
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

  // notification: "notebookDocument/didOpen"
  auto LspLanguageServer::didOpenNotebookDocument(
    const DidOpenNotebookDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for notification=\"{}\"",
        NotificationMethodValues.at(
          NotificationMethod::DID_OPEN_NOTEBOOK_DOCUMENT
        )
      )
    );
  }

  // notification: "notebookDocument/didChange"
  auto LspLanguageServer::didChangeNotebookDocument(
    const DidChangeNotebookDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for notification=\"{}\"",
        NotificationMethodValues.at(
          NotificationMethod::DID_CHANGE_NOTEBOOK_DOCUMENT
        )
      )
    );
  }

  // notification: "notebookDocument/didSave"
  auto LspLanguageServer::didSaveNotebookDocument(
    const DidSaveNotebookDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for notification=\"{}\"",
        NotificationMethodValues.at(
          NotificationMethod::DID_SAVE_NOTEBOOK_DOCUMENT
        )
      )
    );
  }

  // notification: "notebookDocument/didClose"
  auto LspLanguageServer::didCloseNotebookDocument(
    const DidCloseNotebookDocumentParams &params
  ) -> void {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for notification=\"{}\"",
        NotificationMethodValues.at(
          NotificationMethod::DID_CLOSE_NOTEBOOK_DOCUMENT
        )
      )
    );
  }

} // namespace LCompilers::LanguageServerProtocol
