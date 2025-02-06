#ifndef LCOMPILERS_LS_THREAD_POOL_H
#define LCOMPILERS_LS_THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include <lsp/logger.h>

namespace LCompilers::LanguageServer::Threading {
  namespace lsl = LCompilers::LanguageServer::Logging;

  typedef std::function<void(const std::size_t threadId)> Task;

  class ThreadPool {
  public:
    ThreadPool(std::size_t numThreads, lsl::Logger &logger);
    auto execute(Task task) -> bool;
    auto getNumThreads() -> std::size_t;
    auto isRunning() -> bool;
    auto stop() -> void;
    auto stopNow() -> void;
    auto join() -> void;
    auto getStdoutMutex() -> std::mutex &;
    auto getStderrMutex() -> std::mutex &;
  protected:
    std::size_t numThreads;
    lsl::Logger &logger;
    std::mutex stdoutMutex;
    std::mutex stderrMutex;
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

#endif // LCOMPILERS_LS_THREAD_POOL_H
