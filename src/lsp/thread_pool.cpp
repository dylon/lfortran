#include <chrono>
#include <iostream>
#include <exception>

#include <lsp/thread_pool.h>

namespace LCompilers::LanguageServer::Threading {
  using namespace std::literals::chrono_literals;

  ThreadPool::ThreadPool(std::size_t numThreads)
    : numThreads(numThreads)
  {
    for (std::size_t threadId = 0; threadId < numThreads; ++threadId) {
      std::cerr << "Starting thread with id=" << threadId << std::endl;
      workers.emplace_back([this, threadId]() {
        run(threadId);
      });
    }
  }

  auto ThreadPool::getNumThreads() -> std::size_t {
    return numThreads;
  }

  auto ThreadPool::isRunning() -> bool {
    return running;
  }

  auto ThreadPool::getStdoutMutex() -> std::mutex & {
    return stdoutMutex;
  }

  auto ThreadPool::getStderrMutex() -> std::mutex & {
    return stderrMutex;
  }

  auto ThreadPool::execute(Task task) -> bool {
    if (!stopRunning) {
      std::unique_lock<std::mutex> taskLock(taskMutex);
      if (!stopRunning) {
        tasks.push(task);
        taskAvailable.notify_one();
        return true;
      }
    }
    return false;
  }

  auto ThreadPool::stop() -> void {
    {
      std::unique_lock<std::mutex> stderrLock(stderrMutex);
      std::cerr
        << "Thread pool will no longer accept new tasks and will shut down "
        << "once those pending have returned."
        << std::endl;
    }
    stopRunning = true;
    taskAvailable.notify_all();
  }

  auto ThreadPool::stopNow() -> void {
    {
      std::unique_lock<std::mutex> stderrLock(stderrMutex);
      std::cerr
        << "Stopping thread pool as quickly as possible."
        << std::endl;
    }
    stopRunning = true;
    stopRunningNow = true;
    taskAvailable.notify_all();
  }

  auto ThreadPool::join() -> void {
    for (auto &worker : workers) {
      worker.join();
    }
    running = false;
  }

  auto ThreadPool::run(const std::size_t threadId) -> void {
    try {
      while (!stopRunningNow && (!stopRunning || !tasks.empty())) {
        std::unique_lock<std::mutex> taskLock(taskMutex);
        taskAvailable.wait(taskLock, [this]() {
          return !tasks.empty() || stopRunning;
        });
        if (!stopRunningNow && !tasks.empty()) {
          Task task = tasks.front();
          tasks.pop();
          taskLock.unlock();
          try {
            task(threadId);
          } catch (std::exception &e) {
            std::unique_lock<std::mutex> stderrLock(stderrMutex);
            std::cerr
              << "[thread.id=" << threadId << "] "
              << "Failed to execute task: " << e.what()
              << std::endl;
          }
        }
      }
      {
        std::unique_lock<std::mutex> stderrLock(stderrMutex);
        std::cerr
          << "[thread.id=" << threadId << "] "
          << "Shutting down thread."
          << std::endl;
      }
    } catch (std::exception &e) {
      std::unique_lock<std::mutex> stderrLock(stderrMutex);
      std::cerr
        << "[thread.id=" << threadId << "] "
        << "Unhandled exception caught: " << e.what()
        << std::endl;
    }
  }

} // namespace LCompilers::LanguageServer::Threading
