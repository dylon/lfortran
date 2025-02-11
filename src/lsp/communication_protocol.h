#pragma once

#include <atomic>
#include <thread>

#include <lsp/message_queue.h>
#include <lsp/language_server.h>
#include <lsp/logger.h>
#include <lsp/message_stream.h>

namespace LCompilers::LanguageServer {
  namespace lsl = LCompilers::LanguageServer::Logging;

  class CommunicationProtocol {
  public:
    CommunicationProtocol(
      LanguageServer &languageServer,
      MessageStream &messageStream,
      MessageQueue &incomingMessages,
      MessageQueue &outgoingMessages,
      lsl::Logger &logger);
    auto serve() -> void;
  private:
    LanguageServer &languageServer;
    MessageStream &messageStream;
    MessageQueue &incomingMessages;
    MessageQueue &outgoingMessages;
    lsl::Logger &logger;
    std::thread listener;
    std::atomic_bool running = true;

    auto listen() -> void;
  };

} // namespace LCompilers::LanguageServer
