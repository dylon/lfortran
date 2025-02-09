#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include <lsp/logger.h>

namespace LCompilers::LanguageServer::Threading {
  namespace lsl = LCompilers::LanguageServer::Logging;

  typedef std::function<void(
    const std::string &name,
    const std::size_t threadId
  )> Task;

  class ThreadPool {
  public:
    ThreadPool(
      const std::string &name,
      std::size_t numThreads,
      lsl::Logger &logger
    );
    auto execute(Task task) -> bool;
    auto name() const -> const std::string &;
    auto numThreads() const -> std::size_t;
    auto isRunning() const -> bool;
    auto stop() -> void;
    auto stopNow() -> void;
    auto join() -> void;
  protected:
    const std::string _name;
    std::size_t _numThreads;
    lsl::Logger &logger;
    std::vector<std::thread> workers;
    std::atomic_bool running = true;
    std::atomic_bool stopRunning = false;
    std::atomic_bool stopRunningNow = false;
    std::mutex taskMutex;
    std::condition_variable taskAvailable;
    std::queue<Task> tasks;

    auto run(const std::size_t threadId) -> void;
  };

} // namespace LCompilers::LanguageServer::Threading
