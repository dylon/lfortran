#include <lsp/language_server.h>
#include <mutex>

namespace LCompilers::LanguageServer {

  LanguageServer::LanguageServer(
    MessageQueue &incomingMessages,
    MessageQueue &outgoingMessages,
    std::size_t numRequestThreads,
    std::size_t numWorkerThreads,
    lsl::Logger &logger
  ) : incomingMessages(incomingMessages)
    , outgoingMessages(outgoingMessages)
    , listener([this]() {
      listen();
    })
    , requestPool("request", numRequestThreads, logger)
    , workerPool("worker", numWorkerThreads, logger)
    , logger(logger)
  {
    // empty
  }

  auto LanguageServer::nextSendId() -> std::size_t {
    return serialSendId++;
  }

  auto LanguageServer::listen() -> void {
    try {
      while (!isTerminated()) {
        const std::string message = incomingMessages.dequeue();
        if (!isTerminated()) {
          std::size_t sendId = nextSendId();
          requestPool.execute([this, message, sendId](
            const std::string &threadName,
            const std::size_t threadId
          ) {
            try {
              handle(message, sendId);
            } catch (std::exception &e) {
              auto loggerLock = logger.lock();
              logger
                << "[" << threadName << "_" << threadId << "] "
                << "Failed to handle message: " << message
                << std::endl;
              logger
                << "[" << threadName << "_" << threadId << "] "
                << "Caught unhandled exception: " << e.what()
                << std::endl;
            }
          });
        }
      }
    } catch (std::runtime_error &e) {
      auto loggerLock = logger.lock();
      logger
        << "Interrupted while dequeuing messages: "
        << e.what()
        << std::endl;
    }
  }

  auto LanguageServer::notifySent() -> void {
    ++pendingSendId;
    std::unique_lock<std::mutex> sentLock(sentMutex);
    sent.notify_all();
  }

  auto LanguageServer::send(
    const std::string &message,
    std::size_t sendId
  ) -> void {
    // -------------------------------------------------------------------------
    // NOTE: The LSP spec requires responses to be returned in roughly the same
    // order of receipt of their corresponding requests. Some types of responses
    // may be returned out-of-order, but in order to support those we will need
    // to implement a sort of dependency graph. Without knowledge of their
    // dependencies, we must respond to all requests in order of receipt.
    // -------------------------------------------------------------------------
    while ((pendingSendId < sendId) && !isTerminated()) {
      std::unique_lock<std::mutex> sentLock(sentMutex);
      if ((pendingSendId < sendId) && !isTerminated()) {
        sent.wait(sentLock);
      }
    }
    if (!isTerminated()) {
      send(message);
      notifySent();
    }
  }

  auto LanguageServer::send(const std::string &message) -> void {
    ss.str("");
    prepare(ss, message);
    outgoingMessages.enqueue(ss.str());
  }

} // namespace LCompilers::LanguageServer
