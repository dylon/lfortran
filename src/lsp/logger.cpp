#include <format>
#include <stdexcept>

#include <lsp/logger.h>

namespace LCompilers::LanguageServer::Logging {

  Logger::Logger(const fs::path &logPath)
    : _logPath(logPath)
    , logFile(logPath, std::ios::out | std::ios::trunc)
  {
    if (!logFile.is_open()) {
      throw std::invalid_argument(
        std::format("Failed to open log file for writing: {}", logPath.string())
      );
    }
  }

  Logger::~Logger() {
    if (isOpen()) {
      close();
    }
  }

  auto Logger::logPath() const -> const fs::path & {
    return _logPath;
  }

  auto Logger::isOpen() const -> bool {
    return logFile.is_open();
  }

  auto Logger::close() -> void {
    if (logFile.is_open()) {
      logFile.flush();
      logFile.close();
    } else {
      throw std::runtime_error("Logger has already been closed.");
    }
  }

  auto Logger::operator<<(unsigned char c) -> Logger & {
    logFile << c;
    std::cerr << c;
    return *this;
  }

  auto Logger::operator<<(char c) -> Logger & {
    logFile << c;
    std::cerr << c;
    return *this;
  }

  auto Logger::operator<<(short unsigned int value) -> Logger & {
    logFile << value;
    std::cerr << value;
    return *this;
  }

  auto Logger::operator<<(int value) -> Logger & {
    logFile << value;
    std::cerr << value;
    return *this;
  }

  auto Logger::operator<<(unsigned int value) -> Logger & {
    logFile << value;
    std::cerr << value;
    return *this;
  }

  auto Logger::operator<<(std::size_t value) -> Logger & {
    logFile << value;
    std::cerr << value;
    return *this;
  }

  auto Logger::operator<<(const char *str) -> Logger & {
    logFile << str;
    std::cerr << str;
    return *this;
  }

  auto Logger::operator<<(const std::string &str) -> Logger & {
    logFile << str;
    std::cerr << str;
    return *this;
  }

  auto Logger::operator<<(std::ostream& (*manip)(std::ostream&)) -> Logger & {
    logFile << manip;
    std::cerr << manip;
    return *this;
  }

} // namespace LCompilers::LanguageServer::Logging
