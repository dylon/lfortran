#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace LCompilers::LanguageServer::Logging {
  namespace fs = std::filesystem;

  class Logger {
  public:
    Logger(const fs::path &logPath);
    ~Logger();
    auto logPath() const -> const fs::path &;
    auto isOpen() const -> bool;
    auto close() -> void;
    auto lock() -> std::unique_lock<std::mutex>;
    auto operator<<(unsigned char c) -> Logger &;
    auto operator<<(char c) -> Logger &;
    auto operator<<(short unsigned int value) -> Logger &;
    auto operator<<(int value) -> Logger &;
    auto operator<<(unsigned int value) -> Logger &;
    auto operator<<(std::size_t value) -> Logger &;
    auto operator<<(const char *str) -> Logger &;
    auto operator<<(const std::string &str) -> Logger &;
    auto operator<<(std::ostream& (*manip)(std::ostream&)) -> Logger &;
  private:
    fs::path _logPath;
    std::ofstream logFile;
    std::mutex mutex;
  };

} // namespace LCompilers::LanguageServer::Logging
