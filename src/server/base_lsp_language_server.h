#pragma once

#include <functional>
#include <future>
#include <optional>
#include <queue>
#include <shared_mutex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#include <server/logger.h>
#include <server/lsp_language_server.h>
#include <server/lsp_specification.h>
#include <server/lsp_text_document.h>

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
        std::unordered_map<DocumentUri, LspTextDocument> documentsByUri;
        std::shared_mutex documentMutex;
        std::unordered_map<DocumentUri, LSPAny> configsByUri;
        std::unordered_map<
            DocumentUri,
            std::pair<int, std::optional<std::shared_future<std::reference_wrapper<LSPAny>>>>
        > pendingConfigsByUri;
        std::queue<
            std::tuple<DocumentUri, int, std::promise<std::reference_wrapper<LSPAny>>>
        > pendingConfigs;
        std::shared_mutex configMutex;
        std::unique_ptr<LSPAny> workspaceConfig;
        std::shared_mutex workspaceMutex;

        std::atomic_bool clientSupportsWorkspaceDidChangeConfigurationNotifications = false;
        std::atomic_bool clientSupportsWorkspaceConfigurationRequests = false;

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

        auto receiveInitialize(
            InitializeParams &params
        ) -> InitializeResult override;

        auto receiveInitialized(
            InitializedParams &params
        ) -> void override;

        // ====================== //
        // Incoming Notifications //
        // ====================== //

        auto receiveWorkspace_didRenameFiles(
            RenameFilesParams &params
        ) -> void override;

        auto receiveWorkspace_didChangeConfiguration(
            DidChangeConfigurationParams &params
        ) -> void override;

        auto receiveWorkspace_configuration(
            WorkspaceConfigurationResult &params
        ) -> void override;

        auto receiveTextDocument_didOpen(
            DidOpenTextDocumentParams &params
        ) -> void override;

        auto receiveTextDocument_didChange(
            DidChangeTextDocumentParams &params
        ) -> void override;

        auto receiveTextDocument_didClose(
            DidCloseTextDocumentParams &params
        ) -> void override;

        auto receiveTextDocument_didSave(
            DidSaveTextDocumentParams &params
        ) -> void override;

    }; // class LspLanguageServer

} // namespace LCompilers::LanguageServerProtocol
