#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>

#include <lsp/communication_protocol.h>

namespace LCompilers::LanguageServer {

  CommunicationProtocol::CommunicationProtocol(
    LanguageServer &languageServer,
    MessageStream &messageStream,
    MessageQueue &incomingMessages,
    MessageQueue &outgoingMessages,
    lsl::Logger &logger)
    : languageServer(languageServer)
    , messageStream(messageStream)
    , incomingMessages(incomingMessages)
    , outgoingMessages(outgoingMessages)
    , logger(logger)
    , listener([this]() {
      listen();
    })
  {
    // empty
  }

  auto CommunicationProtocol::listen() -> void {
    try {
      do {
        const std::string message = incomingMessages.dequeue();
        {
          std::unique_lock<std::mutex> loggerLock(logger.mutex());
          logger
            << ">>> OUTGOING:" << std::endl
            << message << std::endl;
        }
        std::cout << message << std::flush;
      } while (running);
    } catch (std::exception &e) {
      std::unique_lock<std::mutex> loggerLock(logger.mutex());
      logger
        << "[CommunicationProtocol] Interrupted while dequeuing messages: "
        << e.what()
        << std::endl;
    }
    {
      std::unique_lock<std::mutex> loggerLock(logger.mutex());
      logger
        << "[CommunicationProtocol] Incoming-message listener terminated."
        << std::endl;
    }
  }

  void CommunicationProtocol::serve() {
    {
      std::unique_lock<std::mutex> loggerLock(logger.mutex());
      logger << "[CommunicationProtocol] Serving requests." << std::endl;
    }
    try {
      while (!languageServer.isTerminated()) {
        std::string message = messageStream.next();
        if (message.length() > 0) {
          outgoingMessages.enqueue(message);
        } else {
          std::unique_lock<std::mutex> loggerLock(logger.mutex());
          logger
            << "[CommunicationProtocol] Cannot parse an empty request body."
            << std::endl;
        }
      }
    } catch (std::exception &e) {
      std::unique_lock<std::mutex> loggerLock(logger.mutex());
      logger
        << "[CommunicationProtocol] Caught unhandled exception while serving requests: "
        << e.what()
        << std::endl;
    }
    running = false;
    incomingMessages.stopNow();
    languageServer.join();
    listener.join();
    {
      std::unique_lock<std::mutex> loggerLock(logger.mutex());
      logger << "[CommunicationProtocol] Terminated." << std::endl;
    }
  }

} // namespace LCompilers::LanguageServer
