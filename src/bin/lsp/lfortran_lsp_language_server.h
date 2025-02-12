#pragma once

#include <libasr/utils.h>

#include <lsp/logger.h>
#include <lsp/base_lsp_language_server.h>
#include <lsp/message_queue.h>
#include <map>
#include <shared_mutex>

#include <libasr/utils.h>

#include <lsp/specification.h>

#include "bin/lsp/lfortran_accessor.h"

namespace LCompilers::LanguageServerProtocol {
  namespace ls = LCompilers::LanguageServer;
  namespace lsl = LCompilers::LanguageServer::Logging;

  class LFortranLspLanguageServer : public BaseLspLanguageServer {
  public:
    LFortranLspLanguageServer(
      ls::MessageQueue &incomingMessages,
      ls::MessageQueue &outgoingMessages,
      std::size_t numRequestThreads,
      std::size_t numWorkerThreads,
      lsl::Logger &logger,
      const std::string &configSection
    );
  protected:

    auto invalidateConfigCache() -> void override;

    // ================= //
    // Incoming Requests //
    // ================= //

    InitializeResult receiveInitialize(
      InitializeParams &params
    ) override;

    // ====================== //
    // Incoming Notifications //
    // ====================== //

    void receiveWorkspace_didDeleteFiles(
      DeleteFilesParams &params
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

    void receiveWorkspace_didChangeWatchedFiles(
      DidChangeWatchedFilesParams &params
    ) override;

  private:
    const std::string source = "lfortran";
    ls::LFortranAccessor lfortran;
    std::map<DocumentUri, CompilerOptions> optionsByUri;
    std::shared_mutex optionMutex;

    auto validate(TextDocument &document) -> void;
    auto getCompilerOptions(const DocumentUri &uri) -> const CompilerOptions &;
  };

} // namespace LCompilers::LanguageServerProtocol
