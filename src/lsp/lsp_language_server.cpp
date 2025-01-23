#include <cctype>
#include <format>
#include <iostream>
#include <mutex>
#include <stdexcept>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  LspLanguageServer::LspLanguageServer(ls::MessageQueue &outgoingMessages)
    : ls::LanguageServer(outgoingMessages)
  {
    // empty
  }

  auto LspLanguageServer::nextId() -> RequestId {
    return serialId++;
  }

  std::string LspLanguageServer::serve(const std::string &request) {
    ResponseMessage response;
    try {
      // The language server protocol always uses “2.0” as the jsonrpc version.
      response.jsonrpc = JSON_RPC_VERSION;
      response.id = nullptr;

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
          response.id = idValue.GetString();
        } else if (idValue.IsInt()) {
          response.id = idValue.GetInt();
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
          if (static_cast<ResponseIdType>(response.id.index()) == ResponseIdType::LSP_NULL) {
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
          if (static_cast<ResponseIdType>(response.id.index()) != ResponseIdType::LSP_NULL) {
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

  auto LspLanguageServer::initializeParams() const -> const InitializeParams & {
    if (_initializeParams.has_value()) {
      return _initializeParams.value();
    }
    throw std::logic_error(
      "LspLanguageServer::initialize must be called before LspLanguageServer::initializeParams"
    );
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
    } else {
      bool expected = false;  // a reference is required ...
      if (!_initialized.compare_exchange_strong(expected, true)) {
        throw LspException(
          ErrorCodes::InvalidRequest,
          "Server may be initialized only once."
        );
      }
    }
    switch (method) {
    case RequestMethod::INITIALIZE: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      _initializeParams = transformer.asInitializeParams(requestParams);
      InitializeResult result = initialize(_initializeParams.value());
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::WILL_SAVE_WAIT_UNTIL: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      WillSaveTextDocumentParams params =
        transformer.asWillSaveTextDocumentParams(requestParams);
      WillSaveWaitUntilResult result = willSaveWaitUntil(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::GOTO_DECLARATION: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      DeclarationParams params =
        transformer.asDeclarationParams(requestParams);
      GotoResult result = gotoDeclaration(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::GOTO_DEFINITION: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      DefinitionParams params =
        transformer.asDefinitionParams(requestParams);
      GotoResult result = gotoDefinition(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::GOTO_TYPE_DEFINITION: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      TypeDefinitionParams params =
        transformer.asTypeDefinitionParams(requestParams);
      GotoResult result = gotoTypeDefinition(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::GOTO_IMPLEMENTATION: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      ImplementationParams params =
        transformer.asImplementationParams(requestParams);
      GotoResult result = gotoImplementation(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::FIND_REFERENCES: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      ReferenceParams params =
        transformer.asReferenceParams(requestParams);
      FindReferencesResult result = findReferences(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::PREPARE_CALL_HIERARCHY: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      CallHierarchyPrepareParams params =
        transformer.asCallHierarchyPrepareParams(requestParams);
      PrepareCallHierarchyResult result = prepareCallHierarchy(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::CALL_HIERARCHY_INCOMING_CALLS: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      CallHierarchyIncomingCallsParams params =
        transformer.asCallHierarchyIncomingCallsParams(requestParams);
      CallHierarchyIncomingCallsResult result =
        callHierarchyIncomingCalls(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::CALL_HIERARCHY_OUTGOING_CALLS: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      CallHierarchyOutgoingCallsParams params =
        transformer.asCallHierarchyOutgoingCallsParams(requestParams);
      CallHierarchyOutgoingCallsResult result =
        callHierarchyOutgoingCalls(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::PREPARE_TYPE_HIERARCHY: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      TypeHierarchyPrepareParams params =
        transformer.asTypeHierarchyPrepareParams(requestParams);
      TypeHierarchyResult result = prepareTypeHierarchy(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::TYPE_HIERARCHY_SUPERTYPES: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      TypeHierarchySupertypesParams params =
        transformer.asTypeHierarchySupertypesParams(requestParams);
      TypeHierarchyResult result = typeHierarchySupertypes(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::TYPE_HIERARCHY_SUBTYPES: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      TypeHierarchySubtypesParams params =
        transformer.asTypeHierarchySubtypesParams(requestParams);
      TypeHierarchyResult result = typeHierarchySubtypes(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::HIGHLIGHT_DOCUMENT: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      DocumentHighlightParams params =
        transformer.asDocumentHighlightParams(requestParams);
      DocumentHighlightResult result = highlightDocument(params);
      response.result = transformer.lspToAny(result);
      break;
    }
    case RequestMethod::EXTRACT_DOCUMENT_LINKS: {
      const MessageParams &requestParams =
        transformer.requireMessageParams(request);
      DocumentLinkParams params =
        transformer.asDocumentLinkParams(requestParams);
      DocumentLinkResult result = extractDocumentLinks(params);
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
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      CancelParams params = transformer.asCancelParams(notificationParams);
      cancelRequest(params);
      break;
    }
    case NotificationMethod::SET_TRACE: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      SetTraceParams params = transformer.asSetTraceParams(notificationParams);
      setTrace(params);
      break;
    }
    case NotificationMethod::DID_OPEN_NOTEBOOK_DOCUMENT: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      DidOpenNotebookDocumentParams params =
        transformer.asDidOpenNotebookDocumentParams(notificationParams);
      didOpenNotebookDocument(params);
      break;
    }
    case NotificationMethod::DID_CHANGE_NOTEBOOK_DOCUMENT: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      DidChangeNotebookDocumentParams params =
        transformer.asDidChangeNotebookDocumentParams(notificationParams);
      didChangeNotebookDocument(params);
      break;
    }
    case NotificationMethod::DID_SAVE_NOTEBOOK_DOCUMENT: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      DidSaveNotebookDocumentParams params =
        transformer.asDidSaveNotebookDocumentParams(notificationParams);
      didSaveNotebookDocument(params);
      break;
    }
    case NotificationMethod::DID_CLOSE_NOTEBOOK_DOCUMENT: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      DidCloseNotebookDocumentParams params =
        transformer.asDidCloseNotebookDocumentParams(notificationParams);
      didCloseNotebookDocument(params);
      break;
    }
    case NotificationMethod::DID_OPEN_TEXT_DOCUMENT: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      DidOpenTextDocumentParams params =
        transformer.asDidOpenTextDocumentParams(notificationParams);
      didOpenTextDocument(params);
      break;
    }
    case NotificationMethod::DID_CHANGE_TEXT_DOCUMENT: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      DidChangeTextDocumentParams params =
        transformer.asDidChangeTextDocumentParams(notificationParams);
      didChangeTextDocument(params);
      break;
    }
    case NotificationMethod::DID_SAVE_TEXT_DOCUMENT: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
      DidSaveTextDocumentParams params =
        transformer.asDidSaveTextDocumentParams(notificationParams);
      didSaveTextDocument(params);
      break;
    }
    case NotificationMethod::DID_CLOSE_TEXT_DOCUMENT: {
      const MessageParams &notificationParams =
        transformer.requireMessageParams(notification);
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

  auto LspLanguageServer::prepare(
    std::ostream &os,
    const std::string &response
  ) const -> void {
    os << "Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n"
       << "Content-Length: " << response.length() << "\r\n"
       << "\r\n"
       << response;
  }

  auto LspLanguageServer::prepare(
    std::stringstream &ss,
    const std::string &response
  ) const -> void {
    ss << "Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n"
       << "Content-Length: " << response.length() << "\r\n"
       << "\r\n"
       << response;
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

  // ========================= //
  // request: client -> server //
  // ========================= //

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
    TextDocumentSync textDocumentSync;
    std::unique_ptr<TextDocumentSyncOptions> textDocumentSyncOptions =
      std::make_unique<TextDocumentSyncOptions>();
    textDocumentSyncOptions->openClose = true;
    textDocumentSyncOptions->change = TextDocumentSyncKind::Incremental;
    SaveOrOptions save;
    std::unique_ptr<SaveOptions> saveOptions = std::make_unique<SaveOptions>();
    saveOptions->includeText = true;
    save = std::move(saveOptions);
    textDocumentSyncOptions->save = std::move(save);
    textDocumentSync = std::move(textDocumentSyncOptions);
    capabilities->textDocumentSync = std::move(textDocumentSync);
    result.capabilities = std::move(capabilities);

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
  ) -> GotoResult {
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
  ) -> GotoResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::GOTO_DEFINITION)
      )
    );
  }

  // request: "textDocument/typeDefinition"
  auto LspLanguageServer::gotoTypeDefinition(
    const TypeDefinitionParams &params
  ) -> GotoResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::GOTO_TYPE_DEFINITION)
      )
    );
  }

  // request: "textDocument/implementation"
  auto LspLanguageServer::gotoImplementation(
    const ImplementationParams &params
  ) -> GotoResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::GOTO_IMPLEMENTATION)
      )
    );
  }

  // request: "textDocument/references"
  auto LspLanguageServer::findReferences(
    const ReferenceParams &params
  ) -> FindReferencesResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::FIND_REFERENCES)
      )
    );
  }

  // request: "textDocument/prepareCallHierarchy"
  auto LspLanguageServer::prepareCallHierarchy(
    const CallHierarchyPrepareParams &params
  ) -> PrepareCallHierarchyResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::PREPARE_CALL_HIERARCHY)
      )
    );
  }

  // request: "callHierarchy/incomingCalls"
  auto LspLanguageServer::callHierarchyIncomingCalls(
    const CallHierarchyIncomingCallsParams &params
  ) -> CallHierarchyIncomingCallsResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::CALL_HIERARCHY_INCOMING_CALLS)
      )
    );
  }

  // request: "callHierarchy/outgoingCalls"
  auto LspLanguageServer::callHierarchyOutgoingCalls(
    const CallHierarchyOutgoingCallsParams &params
  ) -> CallHierarchyOutgoingCallsResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::CALL_HIERARCHY_OUTGOING_CALLS)
      )
    );
  }

  // request: "textDocument/prepareTypeHierarchy"
  auto LspLanguageServer::prepareTypeHierarchy(
    const TypeHierarchyPrepareParams &params
  ) -> TypeHierarchyResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::PREPARE_TYPE_HIERARCHY)
      )
    );
  }

  // request: "typeHierarchy/supertypes"
  auto LspLanguageServer::typeHierarchySupertypes(
    const TypeHierarchySupertypesParams &params
  ) -> TypeHierarchyResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::TYPE_HIERARCHY_SUPERTYPES)
      )
    );
  }

  // request: "textDocument/documentHighlight"
  auto LspLanguageServer::highlightDocument(
    const DocumentHighlightParams &params
  ) -> DocumentHighlightResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::HIGHLIGHT_DOCUMENT)
      )
    );
  }

  // request: "textDocument/documentLink"
  auto LspLanguageServer::extractDocumentLinks(
    const DocumentLinkParams &params
  ) -> DocumentLinkResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::HIGHLIGHT_DOCUMENT)
      )
    );
  }

  // request: "typeHierarchy/subtypes"
  auto LspLanguageServer::typeHierarchySubtypes(
    const TypeHierarchySubtypesParams &params
  ) -> TypeHierarchyResult {
    throw LspException(
      ErrorCodes::MethodNotFound,
      std::format(
        "No handler exists for request=\"{}\"",
        RequestMethodValues.at(RequestMethod::TYPE_HIERARCHY_SUBTYPES)
      )
    );
  }

  // request: "shutdown"
  auto LspLanguageServer::shutdown() -> void {
    std::cerr << "Shutting down server." << std::endl;
    _shutdown = true;
  }

  // ============================== //
  // notification: client -> server //
  // ============================== //

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

  // ========================= //
  // request: server -> client //
  // ========================= //

  // request: "client/registerCapability"
  auto LspLanguageServer::registerCapability(
    const RegistrationParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "client/registerCapability";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // request: "client/unregisterCapability"
  auto LspLanguageServer::unregisterCapability(
    const UnregistrationParams &params
  ) -> void {
    RequestMessage request;
    request.jsonrpc = JSON_RPC_VERSION;
    request.id = nextId();
    request.method = "client/unregisterCapability";
    request.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeRequest(request);
    outgoingMessages.enqueue(message);
  }

  // ============================== //
  // notification: server -> client //
  // ============================== //

  // notification: "$/progress"
  auto LspLanguageServer::reportProgress(
    const ProgressParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "$/progress";
    notification.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeNotification(notification);
    outgoingMessages.enqueue(message);
  }

  // notification: "$/logTrace"
  auto LspLanguageServer::logTrace(
    const LogTraceParams &params
  ) -> void {
    NotificationMessage notification;
    notification.jsonrpc = JSON_RPC_VERSION;
    notification.method = "$/logTrace";
    notification.params = transformer.asMessageParams(params);
    const std::string message = serializer.serializeNotification(notification);
    outgoingMessages.enqueue(message);
  }

} // namespace LCompilers::LanguageServerProtocol
