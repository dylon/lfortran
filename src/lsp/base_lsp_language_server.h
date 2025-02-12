#pragma once

#include <functional>
#include <future>
#include <map>
#include <queue>
#include <shared_mutex>
#include <tuple>
#include <utility>

#include <lsp/logger.h>
#include <lsp/lsp_language_server.h>
#include <lsp/specification.h>
#include <lsp/text_document.h>

namespace LCompilers::LanguageServerProtocol {

  class BaseLspLanguageServer : public LspLanguageServer {
  public:
    BaseLspLanguageServer(
      ls::MessageQueue &incomingMessages,
      ls::MessageQueue &outgoingMessages,
      std::size_t numRequestThreads,
      std::size_t numWorkerThreads,
      lsl::Logger &logger,
      const std::string &configSection
    );
  protected:
    std::map<DocumentUri, TextDocument> documentsByUri;
    std::shared_mutex documentMutex;
    std::map<DocumentUri, std::unique_ptr<LSPAny>> configsByUri;
    std::map<
      DocumentUri,
      std::pair<int, std::shared_future<std::reference_wrapper<std::unique_ptr<LSPAny>>>>
    > pendingConfigsByUri;
    std::queue<
      std::tuple<DocumentUri, int, std::promise<std::reference_wrapper<std::unique_ptr<LSPAny>>>>
    > pendingConfigs;
    std::shared_mutex configMutex;
    std::unique_ptr<LSPAny> workspaceConfig;
    std::shared_mutex workspaceMutex;

    bool clientSupportsWorkspaceDidChangeConfigurationNotifications = false;
    bool clientSupportsWorkspaceConfigurationRequests = false;

    auto getConfig(
      const DocumentUri &uri,
      const std::string &configSection
    ) -> const LSPAny &;

    inline auto getConfig(
      const DocumentUri &uri
    ) -> const LSPAny & {
      return getConfig(uri, configSection);
    }

    virtual auto invalidateConfigCache() -> void;
    auto updateLogLevel() -> void;

    // ================= //
    // Incoming Requests //
    // ================= //

    InitializeResult receiveInitialize(
      InitializeParams &params
    ) override;

    void receiveInitialized(
      InitializedParams &params
    ) override;

    // ====================== //
    // Incoming Notifications //
    // ====================== //

    void receiveWorkspace_didRenameFiles(
      RenameFilesParams &params
    ) override;

    void receiveWorkspace_didChangeConfiguration(
      DidChangeConfigurationParams &params
    ) override;

    void receiveWorkspace_configuration(
      WorkspaceConfigurationResult &params
    ) override;

    void receiveTextDocument_didOpen(
      DidOpenTextDocumentParams &params
    ) override;

    void receiveTextDocument_didChange(
      DidChangeTextDocumentParams &params
    ) override;

    void receiveTextDocument_didClose(
      DidCloseTextDocumentParams &params
    ) override;

    void receiveSetTrace(
      SetTraceParams &params
    ) override;

  }; // class LspLanguageServer

} // namespace LCompilers::LanguageServerProtocol
