#include <cstddef>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#define CLI11_HAS_FILESYSTEM 0
#include <bin/CLI11.hpp>

#include <lsp/communication_protocol.h>
#include <lsp/language_server.h>
#include <lsp/lfortran_lsp_language_server.h>
#include <lsp/logger.h>
#include <lsp/lsp_request_parser.h>
#include <lsp/message_queue.h>
#include <lsp/request_parser.h>
#include <lsp/thread_pool.h>

namespace fs = std::filesystem;

namespace ls = LCompilers::LanguageServer;
namespace lsp = LCompilers::LanguageServerProtocol;
namespace lst = LCompilers::LanguageServer::Threading;
namespace lsl = LCompilers::LanguageServer::Logging;

enum class ExitCode {
  SUCCESS = 0,
  INVALID_VALUE = 1,
  BAD_ARG_COMBO = 2,
  UNHANDLED_EXCEPTION = 3,
};

std::map<ExitCode, std::string> ExitCodeNames = {
  {ExitCode::SUCCESS, "SUCCESS"},
  {ExitCode::INVALID_VALUE, "INVALID_VALUE"},
  {ExitCode::BAD_ARG_COMBO, "BAD_ARG_COMBO"},
  {ExitCode::UNHANDLED_EXCEPTION, "UNHANDLED_EXCEPTION"},
};

enum class Language {
  FORTRAN,
};

std::map<Language, std::string> LanguageNames = {
  {Language::FORTRAN, "FORTRAN"},
};

std::map<Language, std::string> LanguageValues = {
  {Language::FORTRAN, "fortran"},
};

auto languageByValue(const std::string &value) -> Language {
  for (const auto &[lang_key, lang_value] : LanguageValues) {
    if (lang_value == value) {
      return lang_key;
    }
  }
  throw std::invalid_argument(
    std::format("Invalid Language value: \"{}\"", value)
  );
}

enum class DataFormat {
  JSON_RPC,
};

std::map<DataFormat, std::string> DataFormatNames = {
  {DataFormat::JSON_RPC, "JSON_RPC"},
};

std::map<DataFormat, std::string> DataFormatValues = {
  {DataFormat::JSON_RPC, "json-rpc"},
};

auto dataFormatByValue(const std::string &value) -> DataFormat {
  for (const auto &[fmt_key, fmt_value] : DataFormatValues) {
    if (fmt_value == value) {
      return fmt_key;
    }
  }
  throw std::invalid_argument(
    std::format("Invalid DataFormat value: \"{}\"", value)
  );
}

enum class CommunicationProtocol {
  STDIO,
  TCP,
  WEBSOCKET,
};

std::map<CommunicationProtocol, std::string> CommunicationProtocolNames = {
  {CommunicationProtocol::STDIO, "STDIO"},
  {CommunicationProtocol::TCP, "TCP"},
  {CommunicationProtocol::WEBSOCKET, "WEBSOCKET"},
};

std::map<CommunicationProtocol, std::string> CommunicationProtocolValues = {
  {CommunicationProtocol::STDIO, "stdio"},
  {CommunicationProtocol::TCP, "tcp"},
  {CommunicationProtocol::WEBSOCKET, "websocket"},
};

auto communicationProtocolByValue(const std::string &value) -> CommunicationProtocol {
  for (const auto &[protocol_key, protocol_value] : CommunicationProtocolValues) {
    if (protocol_value == value) {
      return protocol_key;
    }
  }
  throw std::invalid_argument(
    std::format("Invalid CommunicationProtocol value: \"{}\"", value)
  );
}

enum class ServerProtocol {
  LSP,
};

std::map<ServerProtocol, std::string> ServerProtocolNames = {
  {ServerProtocol::LSP, "LSP"},
};

std::map<ServerProtocol, std::string> ServerProtocolValues = {
  {ServerProtocol::LSP, "lsp"},
};

auto serverProtocolByValue(const std::string &value) -> ServerProtocol {
  for (const auto &[protocol_key, protocol_value] : ServerProtocolValues) {
    if (protocol_value == value) {
      return protocol_key;
    }
  }
  throw std::invalid_argument(
    std::format("Invalid ServerProtocol value: \"{}\"", value)
  );
}

struct CommandLineOptions {
  Language language;
  DataFormat dataFormat;
  CommunicationProtocol communicationProtocol;
  ServerProtocol serverProtocol;
  short unsigned int tcpPort;
  bool interactive;
  std::size_t numThreads;
  fs::path logPath;
};

auto validateAndSetLogPath(
  CommandLineOptions &opts,
  const std::string &logPathStr
) -> ExitCode {
  fs::path logPath = fs::absolute(logPathStr).lexically_normal();
  fs::path logDir = logPath.parent_path();
  if (!fs::exists(logDir) && !fs::create_directories(logDir)) {
    std::cerr
      << "Cannot create log directory: " << logDir
      << std::endl;
    return ExitCode::INVALID_VALUE;
  }

  std::ofstream ofs(logPath);
  if (ofs.is_open()) {
    ofs.close();
    opts.logPath = logPath;
    std::cerr
      << "Logging to: " << logPath
      << std::endl;
    return ExitCode::SUCCESS;
  }

  std::cerr
    << "Log path is not writable: " << logPath
    << std::endl;
  return ExitCode::INVALID_VALUE;
}

auto validateAndSetLanguage(
  CommandLineOptions &opts,
  const std::string &language
) -> ExitCode {
  try {
    opts.language = languageByValue(language);
    return ExitCode::SUCCESS;
  } catch (std::invalid_argument &e) {
    std::cerr
      << "Unsupported value for --language (\""
      << language
      << "\"), it must be one of: ";
    auto iter = LanguageValues.begin();
    for (int index = 0; iter != LanguageValues.end(); ++index, ++iter) {
      if (index > 0) {
        std::cerr << "; ";
      }
      std::cerr << iter->second;
    }
    std::cerr << std::endl;
    return ExitCode::INVALID_VALUE;
  }
}

auto validateAndSetDataFormat(
  CommandLineOptions &opts,
  const std::string &dataFormat
) -> ExitCode {
  try {
    opts.dataFormat = dataFormatByValue(dataFormat);
    return ExitCode::SUCCESS;
  } catch (std::invalid_argument &e) {
    std::cerr
      << "Unsupported value for --data-format: (\""
      << dataFormat
      << "\"), it must be one of: ";
    auto iter = DataFormatValues.begin();
    for (int index = 0; iter != DataFormatValues.end(); ++index, ++iter) {
      if (index > 0) {
        std::cerr << "; ";
      }
      std::cerr << iter->second;
    }
    std::cerr << std::endl;
    return ExitCode::INVALID_VALUE;
  }
}

auto validateAndSetCommunicationProtocol(
  CommandLineOptions &opts,
  const std::string &communicationProtocol
) -> ExitCode {
  try {
    opts.communicationProtocol =
      communicationProtocolByValue(communicationProtocol);
    return ExitCode::SUCCESS;
  } catch (std::invalid_argument &e) {
    std::cerr
      << "Unsupported value for --communication-protocol: (\""
      << communicationProtocol
      << "\"), it must be one of: ";
    auto iter = CommunicationProtocolValues.begin();
    for (int index = 0; iter != CommunicationProtocolValues.end(); ++index, ++iter) {
      if (index > 0) {
        std::cerr << "; ";
      }
      std::cerr << iter->second;
    }
    std::cerr << std::endl;
    return ExitCode::INVALID_VALUE;
  }
}

auto validateAndSetServerProtocol(
  CommandLineOptions &opts,
  const std::string &serverProtocol
) -> ExitCode {
  try {
    opts.serverProtocol = serverProtocolByValue(serverProtocol);
  } catch (std::invalid_argument &e) {
    std::cerr
      << "Unsupported value for --server-protocol: (\""
      << serverProtocol
      << "\"), it must be one of: "
      << std::endl;
    auto iter = ServerProtocolValues.begin();
    for (int index = 0; iter != ServerProtocolValues.end(); ++index, ++iter) {
      if (index > 0) {
        std::cerr << "; ";
      }
      std::cerr << iter->second;
    }
    std::cerr << std::endl;
    return ExitCode::INVALID_VALUE;
  }
  if (opts.serverProtocol == ServerProtocol::LSP) {
    if (opts.dataFormat != DataFormat::JSON_RPC) {
      std::cerr
        << "Only JSON-RPC is supported with LSP; must set --data-format=json-rpc."
        << std::endl;
      return ExitCode::BAD_ARG_COMBO;
    }
  }
  return ExitCode::SUCCESS;
}

auto validateAndSetTcpPort(
  CommandLineOptions &opts,
  short unsigned int tcpPort
) -> ExitCode {
  // NOTE: The following is always true due to the data type of tcpPort:
  // -------------------------------------------------------------------
  // if ((opts.communicationProtocol != CommunicationProtocol::TCP)
  //     || ((0 <= tcpPort) && (tcpPort <= 65535))) {
  //   opts.tcpPort = tcpPort;
  //   return ExitCode::SUCCESS;
  // }
  // std::cerr
  //   << "Invalid value for --tcp-port: "
  //   << tcpPort
  //   << ". Must be between 0 and 65535."
  //   << std::endl;
  // return ExitCode::INVALID_VALUE;
  opts.tcpPort = tcpPort;
  return ExitCode::SUCCESS;
}

auto validateAndSetInteractive(
  CommandLineOptions &opts,
  bool interactive
) -> ExitCode {
  if (!interactive ||
      (opts.communicationProtocol == CommunicationProtocol::STDIO)) {
    opts.interactive = interactive;
    return ExitCode::SUCCESS;
  }
  std::cerr
    << "Only --communication-protocol=stdio supports --interactive"
    << std::endl;
  return ExitCode::BAD_ARG_COMBO;
}

auto validateAndSetNumThreads(
  CommandLineOptions &opts,
  std::size_t numThreads
) -> ExitCode {
  opts.numThreads = numThreads;
  return ExitCode::SUCCESS;
}

/**
 * Parses command-line arguments and stores their values in global variables.
 * @param argc Number of command-line arguments.
 * @param argv Vector of command-line arguments.
 * @return How successful the parser was at parsing the command-line arguments.
 */
auto parse(CommandLineOptions &opts, int argc, char *argv[]) -> int {
  std::string language;
  std::string dataFormat;
  std::string communicationProtocol = "stdio";
  short unsigned int tcpPort = 2078;
  std::string serverProtocol = "lsp";
  bool interactive;
  std::size_t numThreads = 5;
  std::string logPath = "llanguage-server.log";

  CLI::App app {
    "LCompilers Language Server: Serves requests from language extensions in supported editors."
  };

  app.add_option(
    "--language", language,
    "Specifies the language to serve (fortran)."
  );

  app.add_option(
    "--data-format", dataFormat,
    "Specifies the data exchange format for requests (json-rpc)"
  );

  app.add_option(
    "--communication-protocol", communicationProtocol,
    "Specifies the communication protocol over which to serve requests (stdio; tcp; websocket)."
  )->capture_default_str();

  app.add_option(
    "--tcp-port", tcpPort,
    "Specifies the port number when serving requests over TCP (0 through 65535)."
  )->capture_default_str();

  app.add_option(
    "--server-protocol", serverProtocol,
    "Specifies the language server protocol that defines how to serve requests (lsp)."
  )->capture_default_str();

  app.add_flag(
    "--interactive", interactive,
    "Whether to serve stdio requests from an interactive shell (REPL)."
  );

  app.add_option(
    "--num-threads", numThreads,
    "Number of pooled threads (for applicable communication protocols)."
  )->capture_default_str();

  app.add_option(
    "--log-path", logPath,
    "Path to where logs should be written."
  )->capture_default_str();

  CLI11_PARSE(app, argc, argv);

  ExitCode exitCode = validateAndSetLanguage(opts, language);
  if (exitCode == ExitCode::SUCCESS) {
    exitCode = validateAndSetDataFormat(opts, dataFormat);
  }
  if (exitCode == ExitCode::SUCCESS) {
    exitCode = validateAndSetCommunicationProtocol(opts, communicationProtocol);
  }
  if (exitCode == ExitCode::SUCCESS) {
    exitCode = validateAndSetTcpPort(opts, tcpPort);
  }
  if (exitCode == ExitCode::SUCCESS) {
    exitCode = validateAndSetServerProtocol(opts, serverProtocol);
  }
  if (exitCode == ExitCode::SUCCESS) {
    exitCode = validateAndSetNumThreads(opts, numThreads);
  }
  if (exitCode == ExitCode::SUCCESS) {
    exitCode = validateAndSetLogPath(opts, logPath);
  }
  return static_cast<int>(exitCode);
}

std::unique_ptr<ls::RequestParserFactory> buildRequestParserFactory(
  CommandLineOptions &opts,
  lsl::Logger &logger
) {
  switch (opts.serverProtocol) {
  case ServerProtocol::LSP: {
    return std::make_unique<lsp::LspRequestParserFactory>(
      opts.interactive,
      logger
    );
  }
  default: {
    throw std::runtime_error(
      std::format(
        "Unsupported server protocol: {}",
        static_cast<int>(opts.serverProtocol)
      )
    );
  }
  }
}

std::unique_ptr<ls::LanguageServer> buildLanguageServer(
  CommandLineOptions &opts,
  ls::MessageQueue &outgoingMessages,
  lsl::Logger &logger
) {
  if (opts.language == Language::FORTRAN) {
    if (opts.dataFormat == DataFormat::JSON_RPC) {
      if (opts.serverProtocol == ServerProtocol::LSP) {
        return std::make_unique<lsp::LFortranLspLanguageServer>(
          outgoingMessages,
          logger
        );
      } else {
        throw std::runtime_error(
          std::format(
            "Unsupported server protocol for fortran: {}",
            static_cast<int>(opts.serverProtocol)
          )
        );
      }
    } else {
      throw std::runtime_error(
        std::format(
          "Unsupported data format for fortran: {}",
          static_cast<int>(opts.dataFormat)
        )
      );
    }
  } else {
    throw std::runtime_error(
      std::format(
        "Unsupported language: {}",
        static_cast<int>(opts.language)
      )
    );
  }
}

auto buildThreadPool(
  CommandLineOptions &opts,
  lsl::Logger &logger
) -> std::unique_ptr<lst::ThreadPool> {
  return std::make_unique<lst::ThreadPool>(opts.numThreads, logger);
}

auto buildCommunicationProtocol(
  CommandLineOptions &opts,
  ls::LanguageServer &languageServer,
  ls::RequestParserFactory &requestParserFactory,
  ls::MessageQueue &incomingMessages,
  lsl::Logger &logger
) -> std::unique_ptr<ls::CommunicationProtocol> {
  switch (opts.communicationProtocol) {
  case CommunicationProtocol::STDIO: {
    std::unique_ptr<lst::ThreadPool> threadPool = buildThreadPool(opts, logger);
    return std::make_unique<ls::StdIOCommunicationProtocol>(
      languageServer,
      requestParserFactory,
      std::move(threadPool),
      incomingMessages,
      logger
    );
  }
  case CommunicationProtocol::TCP: {
    return std::make_unique<ls::TcpCommunicationProtocol>(
      languageServer,
      requestParserFactory,
      opts.tcpPort,
      opts.numThreads,
      incomingMessages,
      logger
    );
  }
  case CommunicationProtocol::WEBSOCKET: {
    throw std::runtime_error("Unsupported communication protocol: WEBSOCKET");
  }
  default: {
    throw std::runtime_error(
      std::format(
        "Unsupported communication protocol: {}",
        static_cast<int>(opts.communicationProtocol)
      )
    );
  }
  }
}

auto buildMessageQueue(
  CommandLineOptions &opts,
  lsl::Logger &logger
) -> std::unique_ptr<ls::MessageQueue> {
  return std::make_unique<ls::MessageQueue>(logger);
}

int main(int argc, char *argv[]) {
  CommandLineOptions opts;
  int retval = parse(opts, argc, argv);

  if (retval != 0) {
    return retval;
  }

  try {
    lsl::Logger logger(opts.logPath);
    std::unique_ptr<ls::RequestParserFactory> requestParserFactory =
      buildRequestParserFactory(opts, logger);
    std::unique_ptr<ls::MessageQueue> messageQueue =
      buildMessageQueue(opts, logger);
    std::unique_ptr<ls::LanguageServer> languageServer =
      buildLanguageServer(opts, *messageQueue, logger);
    std::unique_ptr<ls::CommunicationProtocol> communicationProtocol =
      buildCommunicationProtocol(
        opts,
        *languageServer,
        *requestParserFactory,
        *messageQueue,
        logger
      );
    communicationProtocol->serve();
  } catch (std::exception &e) {
    std::cerr
      << "Caught unhandled exception: "
      << e.what()
      << std::endl;
    retval = static_cast<int>(ExitCode::UNHANDLED_EXCEPTION);
  }

  return retval;
}
