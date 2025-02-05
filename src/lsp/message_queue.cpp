#include <stdexcept>

#include <lsp/message_queue.h>

namespace LCompilers::LanguageServer {

  auto MessageQueue::enqueue(const std::string &message) -> bool {
    std::unique_lock<std::mutex> lock(mutex);
    while ((_size == MESSAGE_QUEUE_CAPACITY) && running) {
      dequeued.wait(lock);
    }
    if ((_size < MESSAGE_QUEUE_CAPACITY) && running) {
      buffer[tail] = message;
      tail = (tail + 1) % MESSAGE_QUEUE_CAPACITY;
      ++_size;
      enqueued.notify_one();
      return true;
    }
    return false;
  }

  auto MessageQueue::dequeue() -> const std::string {
    std::unique_lock<std::mutex> lock(mutex);
    while ((_size == 0) && running) {
      enqueued.wait(lock);
    }
    if ((_size > 0) && running) {
      const std::string &message = buffer[head];
      head = (head + 1) % MESSAGE_QUEUE_CAPACITY;
      --_size;
      dequeued.notify_one();
      return message;
    }
    throw std::runtime_error("Failed to read message from queue.");
  }

  auto MessageQueue::size() const -> std::size_t {
    return _size;
  }

  auto MessageQueue::isRunning() const -> bool {
    return running;
  }

  auto MessageQueue::isStopped() const -> bool {
    return !running;
  }

  auto MessageQueue::stop() -> void {
    bool expected = true;
    if (running.compare_exchange_strong(expected, false)) {
      enqueued.notify_all();
      dequeued.notify_all();
    } else {
      throw std::runtime_error("MessageQueue has already been stopped!");
    }
  }

} // namespace LCompilers::LanguageServer
