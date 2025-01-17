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

namespace LCompilers::LanguageServer::Threading {

  typedef std::function<void(const std::size_t threadId)> Task;

  class ThreadPool {
  public:
    ThreadPool(std::size_t numThreads);
    auto execute(Task task) -> bool;
    auto numThreads() -> std::size_t;
    auto running() -> bool;
    auto stop() -> void;
    auto stopNow() -> void;
    auto join() -> void;
    auto stdoutMutex() -> std::mutex &;
    auto stderrMutex() -> std::mutex &;
  protected:
    std::size_t _numThreads;
    std::mutex _stdoutMutex;
    std::mutex _stderrMutex;
    std::vector<std::thread> workers;
    std::atomic_bool _running = true;
    std::atomic_bool _stop = false;
    std::atomic_bool _stopNow = false;
    std::mutex taskMutex;
    std::condition_variable taskAvailable;
    std::queue<Task> tasks;

    auto run(const std::size_t threadId) -> void;
  };

} // namespace LCompilers::LanguageServer::Threading

#endif // LCOMPILERS_LS_THREAD_POOL_H
