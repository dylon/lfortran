#include <chrono>
#include <iostream>
#include <exception>

#include <lsp/thread_pool.h>

namespace LCompilers::LanguageServer::Threading {
  using namespace std::literals::chrono_literals;

  ThreadPool::ThreadPool(
    const std::string &name,
    std::size_t numThreads,
    lsl::Logger &logger
  ) : _name(name)
    , _numThreads(numThreads)
    , logger(logger)
  {
    for (std::size_t threadId = 0; threadId < numThreads; ++threadId) {
      {
        auto loggerLock = logger.lock();
        logger << "Starting thread with id=" << threadId << std::endl;
      }
      workers.emplace_back([this, threadId]() {
        run(threadId);
      });
    }
  }

  auto ThreadPool::name() const -> const std::string & {
    return _name;
  }

  auto ThreadPool::numThreads() const -> std::size_t {
    return _numThreads;
  }

  auto ThreadPool::isRunning() const -> bool {
    return running;
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
      auto loggerLock = logger.lock();
      logger
        << "Thread pool will no longer accept new tasks and will shut down "
        << "once those pending have returned."
        << std::endl;
    }
    stopRunning = true;
    taskAvailable.notify_all();
  }

  auto ThreadPool::stopNow() -> void {
    {
      auto loggerLock = logger.lock();
      logger
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
            task(_name, threadId);
          } catch (std::exception &e) {
            auto loggerLock = logger.lock();
            logger
              << "[" << _name << "_" << threadId << "] "
              << "Failed to execute task: " << e.what()
              << std::endl;
          }
        }
      }
      {
        auto loggerLock = logger.lock();
        logger
          << "[" << _name << "_" << threadId << "] "
          << "Shutting down thread."
          << std::endl;
      }
    } catch (std::exception &e) {
      auto loggerLock = logger.lock();
      logger
        << "[" << _name << "_" << threadId << "] "
        << "Unhandled exception caught: " << e.what()
        << std::endl;
    }
  }

} // namespace LCompilers::LanguageServer::Threading
