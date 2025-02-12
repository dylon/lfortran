#pragma once

#include <atomic>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <string_view>

namespace LCompilers::LanguageServer::Logging {
  namespace fs = std::filesystem;

  enum class Level {
    OFF,
    FATAL,
    ERROR,
    WARN,
    INFO,
    DEBUG,
    TRACE,
    ALL,
  };

  extern std::map<Level, std::string> LevelNames;
  extern std::map<Level, std::string> LevelValues;

  auto levelByName(const std::string &name) -> Level;
  auto levelByValue(const std::string &value) -> Level;
  auto levelByValue(int value) -> Level;

  class Logger {
  public:
    Logger(const fs::path &logPath);
    ~Logger();

    inline auto mutex() -> std::mutex & {
      return _mutex;
    }

    auto logPath() const -> const fs::path &;
    auto isOpen() const -> bool;
    auto close() -> void;
    auto setLevel(Level level) -> void;

    inline auto level() const -> Level {
      return _level;
    }

    inline auto isOff() const -> bool {
      return _level == Level::OFF;
    }

    inline auto isFatalEnabled() const -> bool {
      return _level >= Level::FATAL;
    }

    inline auto isErrorEnabled() const -> bool {
      return _level >= Level::ERROR;
    }

    inline auto isWarnEnabled() const -> bool {
      return _level >= Level::WARN;
    }

    inline auto isInfoEnabled() const -> bool {
      return _level >= Level::INFO;
    }

    inline auto isDebugEnabled() const -> bool {
      return _level >= Level::DEBUG;
    }

    inline auto isTraceEnabled() const -> bool {
      return _level >= Level::TRACE;
    }

    inline auto areAllEnabled() const -> bool {
      return _level == Level::ALL;
    }

    auto operator<<(unsigned char c) -> Logger &;
    auto operator<<(char c) -> Logger &;
    auto operator<<(short unsigned int value) -> Logger &;
    auto operator<<(int value) -> Logger &;
    auto operator<<(unsigned int value) -> Logger &;
    auto operator<<(std::size_t value) -> Logger &;
    auto operator<<(const char *str) -> Logger &;
    auto operator<<(const std::string &str) -> Logger &;
    auto operator<<(const std::string_view &view) -> Logger &;
    auto operator<<(std::ostream& (*manip)(std::ostream&)) -> Logger &;
  private:
    fs::path _logPath;
    std::ofstream logFile;
    std::mutex _mutex;
    std::atomic<Level> _level{Level::INFO};
  };

} // namespace LCompilers::LanguageServer::Logging
