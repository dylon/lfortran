#include <chrono>
#include <iostream>
#include <exception>

#include <lsp/thread_pool.h>

namespace LCompilers::LanguageServer::Threading {
  using namespace std::literals::chrono_literals;

  ThreadPool::ThreadPool(std::size_t numThreads)
    : _numThreads(numThreads)
  {
    for (std::size_t threadId = 0; threadId < _numThreads; ++threadId) {
      std::cerr << "Starting thread with id=" << threadId << std::endl;
      workers.emplace_back([this, threadId]() {
        run(threadId);
      });
    }
  }

  auto ThreadPool::numThreads() -> std::size_t {
    return _numThreads;
  }

  auto ThreadPool::running() -> bool {
    return _running;
  }

  auto ThreadPool::stdoutMutex() -> std::mutex & {
    return _stdoutMutex;
  }

  auto ThreadPool::stderrMutex() -> std::mutex & {
    return _stderrMutex;
  }

  auto ThreadPool::execute(Task task) -> bool {
    if (!_stop) {
      std::unique_lock<std::mutex> taskLock(taskMutex);
      if (!_stop) {
        tasks.push(task);
        taskAvailable.notify_one();
        return true;
      }
    }
    return false;
  }

  auto ThreadPool::stop() -> void {
    {
      std::unique_lock<std::mutex> stderrLock(_stderrMutex);
      std::cerr
        << "Thread pool will no longer accept new tasks and will shut down "
        << "once those pending have returned."
        << std::endl;
    }
    _stop = true;
  }

  auto ThreadPool::stopNow() -> void {
    {
      std::unique_lock<std::mutex> stderrLock(_stderrMutex);
      std::cerr
        << "Stopping thread pool as quickly as possible."
        << std::endl;
    }
    _stop = true;
    _stopNow = true;
  }

  auto ThreadPool::join() -> void {
    for (auto &worker : workers) {
      worker.join();
    }
    _running = false;
  }

  auto ThreadPool::run(const std::size_t threadId) -> void {
    try {
      while (!_stopNow) {
        std::unique_lock<std::mutex> taskLock(taskMutex);
        if (taskAvailable.wait_for(taskLock, 200ms, [this]() {
          return tasks.size() > 0;
        })) {
          if (!_stopNow) {
            Task task = tasks.front();
            tasks.pop();
            taskLock.unlock();
            try {
              task(threadId);
            } catch (std::exception &e) {
              std::unique_lock<std::mutex> stderrLock(_stderrMutex);
              std::cerr
                << "[thread.id=" << threadId << "] "
                << "Failed to execute task: " << e.what()
                << std::endl;
            }
          }
        } else if (_stop) {
          break;
        }
      }
      {
        std::unique_lock<std::mutex> stdoutLock(_stdoutMutex);
        std::cerr
          << "[thread.id=" << threadId << "] "
          << "Shutting down thread."
          << std::endl;
      }
    } catch (std::exception &e) {
      std::unique_lock<std::mutex> stderrLock(_stderrMutex);
      std::cerr
        << "[thread.id=" << threadId << "] "
        << "Unhandled exception caught: " << e.what()
        << std::endl;
    }
  }

} // namespace LCompilers::LanguageServer::Threading
