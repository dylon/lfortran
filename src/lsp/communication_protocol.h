#pragma once

#include <atomic>
#include <thread>

#include <lsp/message_queue.h>
#include <lsp/language_server.h>
#include <lsp/logger.h>
#include <lsp/request_parser.h>

namespace LCompilers::LanguageServer {
  namespace lsl = LCompilers::LanguageServer::Logging;

  class CommunicationProtocol {
  public:
    CommunicationProtocol(
      LanguageServer &languageServer,
      RequestParserFactory &parserFactory,
      MessageQueue &incomingMessages,
      MessageQueue &outgoingMessages,
      lsl::Logger &logger);
    virtual void serve() = 0;
  protected:
    LanguageServer &languageServer;
    RequestParserFactory &parserFactory;
    MessageQueue &incomingMessages;
    MessageQueue &outgoingMessages;
    lsl::Logger &logger;
  };

  class StdIOCommunicationProtocol : public CommunicationProtocol {
  public:
    StdIOCommunicationProtocol(
      LanguageServer &languageServer,
      RequestParserFactory &parserFactory,
      MessageQueue &incomingMessages,
      MessageQueue &outgoingMessages,
      lsl::Logger &logger);
    void serve() override;
  private:
    std::thread listener;
    std::atomic_bool running = true;
    std::stringstream ss;

    auto listen() -> void;
  };

} // namespace LCompilers::LanguageServer
