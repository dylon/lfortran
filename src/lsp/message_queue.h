#ifndef LCOMPILERS_LS_MESSAGE_QUEUE_H
#define LCOMPILERS_LS_MESSAGE_QUEUE_H

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <string>

#include <lsp/logger.h>

namespace LCompilers::LanguageServer {
  namespace lsl = LCompilers::LanguageServer::Logging;

  const std::size_t MESSAGE_QUEUE_CAPACITY = 64;

  class MessageQueue {
  public:
    MessageQueue(lsl::Logger &logger);
    auto enqueue(const std::string &message) -> bool;
    auto dequeue() -> const std::string;
    auto size() const -> std::size_t;
    auto isRunning() const -> bool;
    auto isStopped() const -> bool;
    auto stop() -> void;
  private:
    lsl::Logger &logger;
    std::string buffer[MESSAGE_QUEUE_CAPACITY];
    std::atomic_bool running = true;
    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t _size = 0;
    std::mutex mutex;
    std::condition_variable enqueued;
    std::condition_variable dequeued;
  };

} // namespace LCompilers::LanguageServer

#endif // LCOMPILERS_LS_MESSAGE_QUEUE_H
