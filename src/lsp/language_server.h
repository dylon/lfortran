#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <sstream>

#include <lsp/logger.h>
#include <lsp/message_queue.h>
#include <lsp/thread_pool.h>

namespace LCompilers::LanguageServer {
  namespace lsl = LCompilers::LanguageServer::Logging;
  namespace lst = LCompilers::LanguageServer::Threading;

  class LanguageServer {
  public:
    LanguageServer(
      MessageQueue &incomingMessages,
      MessageQueue &outgoingMessages,
      std::size_t numRequestThreads,
      std::size_t numWorkerThreads,
      lsl::Logger &logger
    );
    virtual bool isTerminated() const = 0;
  protected:
    MessageQueue &incomingMessages;
    MessageQueue &outgoingMessages;
    std::thread listener;
    lst::ThreadPool requestPool;
    lst::ThreadPool workerPool;
    lsl::Logger &logger;
    std::stringstream ss;
    std::atomic_size_t serialSendId = 0;
    std::atomic_size_t pendingSendId = 0;
    std::condition_variable sent;
    std::mutex sentMutex;

    auto listen() -> void;
    virtual void handle(const std::string &message, std::size_t sendId) = 0;
    auto nextSendId() -> std::size_t;
    auto notifySent() -> void;
    auto send(const std::string &request, std::size_t sendId) -> void;
    auto send(const std::string &request) -> void;

    virtual void prepare(
      std::stringstream &ss,
      const std::string &message
    ) const = 0;
  };

} // namespace LCompilers::LanguageServer
