#include <lsp/lsp_language_server.h>
#include <lsp/specification.h>

#include "bin/lsp/utils.h"
#include "bin/lsp/lfortran_lsp_language_server.h"

namespace LCompilers::LanguageServerProtocol {

  LFortranLspLanguageServer::LFortranLspLanguageServer(
    ls::MessageQueue &incomingMessages,
    ls::MessageQueue &outgoingMessages,
    std::size_t numRequestThreads,
    std::size_t numWorkerThreads,
    lsl::Logger &logger,
    const std::string &configSection
  ) : BaseLspLanguageServer(
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

  auto LFortranLspLanguageServer::validate(
    const TextDocument &document
  ) -> void {
    CompilerOptions compiler_options;
    compiler_options.continue_compilation = true;
    std::vector<LCompilers::error_highlight> highlights =
      lfortran.showErrors(
        document.path().string(),
        document.text(),
        compiler_options
      );

    std::vector<std::unique_ptr<Diagnostic>> diagnostics;
    for (const LCompilers::error_highlight &highlight : highlights) {
      std::unique_ptr<Position> start = std::make_unique<Position>();
      start->line = highlight.first_line - 1;
      start->character = highlight.first_column - 1;

      std::unique_ptr<Position> end = std::make_unique<Position>();
      end->line = highlight.last_line - 1;
      end->character = highlight.last_column;

      std::unique_ptr<Range> range = std::make_unique<Range>();
      range->start = std::move(start);
      range->end = std::move(end);

      std::unique_ptr<Diagnostic> diagnostic = std::make_unique<Diagnostic>();
      diagnostic->range = std::move(range);
      diagnostic->severity =
        diagnostic_level_to_lsp_severity(highlight.severity);
      diagnostic->message = highlight.message;
      diagnostic->source = source;

      diagnostics.push_back(std::move(diagnostic));
    }

    PublishDiagnosticsParams params;
    params.uri = document.uri();
    params.diagnostics = std::move(diagnostics);
    sendTextDocument_publishDiagnostics(params);
  }

  // request: "initialize"
  auto LFortranLspLanguageServer::receiveInitialize(
    InitializeParams &params
  ) -> InitializeResult {
    InitializeResult result = BaseLspLanguageServer::receiveInitialize(params);
    // add additional function for lfortran
    return result;
  }

  // notification: "workspace/didDeleteFiles"
  auto LFortranLspLanguageServer::receiveWorkspace_didDeleteFiles(
    DeleteFilesParams &/*params*/
  ) -> void {
    std::shared_lock<std::shared_mutex> readLock(documentMutex);
    for (const auto &[uri, document] : documentsByUri) {
      workerPool.execute([this, uri, &document](
        const std::string &threadName,
        const std::size_t threadId
      ) {
        try {
          validate(document);
        } catch (std::exception &e) {
          logger
            << "[" << threadName << "_" << threadId << "] "
            << "Failed to validate document (uri=\""
            << uri << "\"): " << e.what()
            << std::endl;
        }
      });
    }
  }

  // notification: "workspace/didChangeConfiguration"
  auto LFortranLspLanguageServer::receiveWorkspace_didChangeConfiguration(
    DidChangeConfigurationParams &params
  ) -> void {
    BaseLspLanguageServer::receiveWorkspace_didChangeConfiguration(params);
    std::shared_lock<std::shared_mutex> readLock(documentMutex);
    for (const auto &[uri, document] : documentsByUri) {
      workerPool.execute([this, uri, &document](
        const std::string &threadName,
        const std::size_t threadId
      ) {
        try {
          validate(document);
        } catch (std::exception &e) {
          logger
            << "[" << threadName << "_" << threadId << "] "
            << "Failed to validate document (uri=\""
            << uri << "\"): " << e.what()
            << std::endl;
        }
      });
    }
  }

  // notification: "textDocument/didOpen"
  auto LFortranLspLanguageServer::receiveTextDocument_didOpen(
    DidOpenTextDocumentParams &params
  ) -> void {
    BaseLspLanguageServer::receiveTextDocument_didOpen(params);
    {
      std::shared_lock<std::shared_mutex> readLock(documentMutex);
      const DocumentUri &uri = params.textDocument->uri;
      const TextDocument &document = documentsByUri.at(uri);
      validate(document);
    }
  }

  // notification: "textDocument/didChange"
  auto LFortranLspLanguageServer::receiveTextDocument_didChange(
    DidChangeTextDocumentParams &params
  ) -> void {
    BaseLspLanguageServer::receiveTextDocument_didChange(params);
    {
      std::shared_lock<std::shared_mutex> readLock(documentMutex);
      const DocumentUri &uri = params.textDocument->uri;
      const TextDocument &document = documentsByUri.at(uri);
      validate(document);
    }
  }

  // notification: "workspace/didChangeWatchedFiles"
  auto LFortranLspLanguageServer::receiveWorkspace_didChangeWatchedFiles(
    DidChangeWatchedFilesParams &/*params*/
  ) -> void {
    std::shared_lock<std::shared_mutex> readLock(documentMutex);
    for (const auto &[uri, document] : documentsByUri) {
      workerPool.execute([this, uri, &document](
        const std::string &threadName,
        const std::size_t threadId
      ) {
        try {
          validate(document);
        } catch (std::exception &e) {
          logger
            << "[" << threadName << "_" << threadId << "] "
            << "Failed to validate document (uri=\""
            << uri << "\"): " << e.what()
            << std::endl;
        }
      });
    }
  }

} // namespace LCompilers::LanguageServerProtocol
