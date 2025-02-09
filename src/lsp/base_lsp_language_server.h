#pragma once

#include <map>
#include <shared_mutex>

#include <lsp/logger.h>
#include <lsp/lsp_language_server.h>
#include <lsp/lsp_serializer.h>
#include <lsp/lsp_transformer.h>
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
    std::shared_mutex configMutex;

    bool clientSupportsWorkspaceDidChangeConfigurationNotifications = false;
    bool clientSupportsWorkspaceConfigurationRequests = false;

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
