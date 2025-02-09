#include <cctype>
#include <mutex>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/base_lsp_language_server.h>

namespace LCompilers::LanguageServerProtocol {

  BaseLspLanguageServer::BaseLspLanguageServer(
    ls::MessageQueue &incomingMessages,
    ls::MessageQueue &outgoingMessages,
    std::size_t numRequestThreads,
    std::size_t numWorkerThreads,
    lsl::Logger &logger,
    const std::string &configSection
  ) : LspLanguageServer(
      incomingMessages,
      outgoingMessages,
      numRequestThreads,
      numWorkerThreads,
      logger,
      configSection
    )
  {
    // empty
  }

  // ================= //
  // Incoming Requests //
  // ================= //

  // request: "initialize"
  auto BaseLspLanguageServer::receiveInitialize(
    InitializeParams &params
  ) -> InitializeResult {
    { // Initialize internal parameters
      const ClientCapabilities &capabilities = *params.capabilities;
      if (capabilities.workspace.has_value()) {
        const WorkspaceClientCapabilities &workspace =
          *capabilities.workspace.value();

        clientSupportsWorkspaceConfigurationRequests =
          workspace.configuration.has_value()
          && workspace.configuration.value();

        if (workspace.didChangeConfiguration.has_value()) {
          const DidChangeConfigurationClientCapabilities &didChangeConfiguration =
            *workspace.didChangeConfiguration.value();
          clientSupportsWorkspaceDidChangeConfigurationNotifications =
            didChangeConfiguration.dynamicRegistration.has_value()
            && didChangeConfiguration.dynamicRegistration.value();
        }
      }
    }
    { // Notify client of server capabilities
      InitializeResult result = LspLanguageServer::receiveInitialize(params);

      if (!result.capabilities) {
        result.capabilities = std::make_unique<ServerCapabilities>();
      }
      ServerCapabilities &capabilities = *result.capabilities;

      {
        // ------------------------- //
        // TextDocument Sync Options //
        // ------------------------- //
        ServerCapabilities_textDocumentSync textDocumentSync;
        std::unique_ptr<TextDocumentSyncOptions> textDocumentSyncOptions =
          std::make_unique<TextDocumentSyncOptions>();
        textDocumentSyncOptions->openClose = true;
        textDocumentSyncOptions->change = TextDocumentSyncKind::INCREMENTAL;
        TextDocumentSyncOptions_save save;
        std::unique_ptr<SaveOptions> saveOptions = std::make_unique<SaveOptions>();
        saveOptions->includeText = false;
        save = std::move(saveOptions);
        textDocumentSyncOptions->save = std::move(save);
        textDocumentSync = std::move(textDocumentSyncOptions);
        capabilities.textDocumentSync = std::move(textDocumentSync);
      }

      return result;
    }
  }

  // ====================== //
  // Incoming Notifications //
  // ====================== //

  // notification: "initialized"
  auto BaseLspLanguageServer::receiveInitialized(
    InitializedParams &params
  ) -> void {
    LspLanguageServer::receiveInitialized(params);
    if (clientSupportsWorkspaceDidChangeConfigurationNotifications) {
      const std::string method = "workspace/didChangeConfiguration";
      std::unique_ptr<Registration> registration =
        std::make_unique<Registration>();
      registration->id = method;
      registration->method = method;
      RegistrationParams params;
      params.registrations.push_back(std::move(registration));
      sendClient_registerCapability(params);
    }
  }

  // notification: "workspace/didRenameFiles"
  auto BaseLspLanguageServer::receiveWorkspace_didRenameFiles(
    RenameFilesParams &params
  ) -> void {
    for (const std::unique_ptr<FileRename> &param : params.files) {
      const std::string &oldUri = param->oldUri;
      const std::string &newUri = param->newUri;
      std::shared_lock<std::shared_mutex> readLock(documentMutex);
      auto iter = documentsByUri.find(oldUri);
      if (iter != documentsByUri.end()) {
        readLock.unlock();
        std::unique_lock<std::shared_mutex> writeLock(documentMutex);
        TextDocument &textDocument = iter->second;
        documentsByUri.emplace(newUri, std::move(textDocument));
        documentsByUri.erase(iter);
      }
    }
  }

  // notification: "workspace/didChangeConfiguration"
  auto BaseLspLanguageServer::receiveWorkspace_didChangeConfiguration(
    DidChangeConfigurationParams &/*params*/
  ) -> void {
    std::unique_lock<std::shared_mutex> writeLock(configMutex);
    configsByUri.clear();
  }

  // notification: "textDocument/didOpen"
  auto BaseLspLanguageServer::receiveTextDocument_didOpen(
    DidOpenTextDocumentParams &params
  ) -> void {
    const TextDocumentItem &textDocumentItem = *params.textDocument;
    const DocumentUri &uri = textDocumentItem.uri;
    const std::string &languageId = textDocumentItem.languageId;
    int version = textDocumentItem.version;
    const std::string &text = textDocumentItem.text;
    {
      std::unique_lock<std::shared_mutex> writeLock(documentMutex);
      documentsByUri.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(uri),
        std::forward_as_tuple(uri, languageId, version, text, logger)
      );
    }
  }

  // notification: "textDocument/didChange"
  auto BaseLspLanguageServer::receiveTextDocument_didChange(
    DidChangeTextDocumentParams &params
  ) -> void {
    const VersionedTextDocumentIdentifier &versionedDocId = *params.textDocument;
    const DocumentUri &uri = versionedDocId.uri;
    integer_t version = versionedDocId.version;
    {
      std::shared_lock<std::shared_mutex> readLock(documentMutex);
      TextDocument &textDocument = documentsByUri.at(uri);
      readLock.unlock();
      textDocument.apply(params.contentChanges, version);
    }
  }

  // notification: "textDocument/didClose"
  auto BaseLspLanguageServer::receiveTextDocument_didClose(
    DidCloseTextDocumentParams &params
  ) -> void {
    const DocumentUri &uri = params.textDocument->uri;
    {
      std::shared_lock<std::shared_mutex> readLock(documentMutex);
      auto pos = documentsByUri.find(uri);
      readLock.unlock();
      if (pos != documentsByUri.end()) {
        std::unique_lock<std::shared_mutex> writeLock(documentMutex);
        pos = documentsByUri.find(uri);
        if (pos != documentsByUri.end()) {
          documentsByUri.erase(pos);
        }
      }
    }
  }

  // notification: "$/setTrace"
  auto BaseLspLanguageServer::receiveSetTrace(
    SetTraceParams &/*params*/
  ) -> void {
    throw LSP_EXCEPTION(
      ErrorCodes::METHOD_NOT_FOUND,
      "No handler exists for method: \"$/setTrace\""
    );
  }

} // namespace LCompilers::LanguageServerProtocol
