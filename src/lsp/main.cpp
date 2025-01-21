#include "lsp/message_queue.h"
#include <cstddef>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#define CLI11_HAS_FILESYSTEM 0
#include <bin/CLI11.hpp>

#include <lsp/language_server.h>
#include <lsp/lfortran_lsp_language_server.h>
#include <lsp/request_parser.h>
#include <lsp/lsp_request_parser.h>
#include <lsp/communication_protocol.h>
#include <lsp/thread_pool.h>
#include <lsp/message_queue.h>

namespace ls = LCompilers::LanguageServer;
namespace lsp = LCompilers::LanguageServerProtocol;
namespace lst = LCompilers::LanguageServer::Threading;

enum class ExitCode {
  SUCCESS = 0,
  INVALID_VALUE = 1,
  BAD_ARG_COMBO = 2,
  UNHANDLED_EXCEPTION = 3,
};

enum class Language {
  FORTRAN,
};

enum class DataFormat {
  JSON_RPC,
};

enum class CommunicationProtocol {
  STDIO,
  TCP,
  WEBSOCKET,
};

enum class ServerProtocol {
  LSP,
};

struct CommandLineOptions {
  Language language;
  DataFormat dataFormat;
  CommunicationProtocol communicationProtocol;
  ServerProtocol serverProtocol;
  short unsigned int tcpPort;
  bool interactive;
  std::size_t numThreads;
};

ExitCode validateAndSetLanguage(
  CommandLineOptions &opts,
  const std::string &language
) {
  if (language == "fortran") {
    opts.language = Language::FORTRAN;
    return ExitCode::SUCCESS;
  }
  std::cerr
    << "Unsupported value for --language: "
    << language
    << ". Must be one of: fortran."
    << std::endl;
  return ExitCode::INVALID_VALUE;
}

ExitCode validateAndSetDataFormat(
  CommandLineOptions &opts,
  const std::string &dataFormat
) {
  if (dataFormat == "json-rpc") {
    opts.dataFormat = DataFormat::JSON_RPC;
    return ExitCode::SUCCESS;
  }
  std::cerr
    << "Unsupported value for --data-format: "
    << dataFormat
    << ". Must be one of: json-rpc."
    << std::endl;
  return ExitCode::INVALID_VALUE;
}

ExitCode validateAndSetCommunicationProtocol(
  CommandLineOptions &opts,
  const std::string &communicationProtocol
) {
  if (communicationProtocol == "stdio") {
    opts.communicationProtocol = CommunicationProtocol::STDIO;
    return ExitCode::SUCCESS;
  }
  if (communicationProtocol == "tcp") {
    opts.communicationProtocol = CommunicationProtocol::TCP;
    return ExitCode::SUCCESS;
  }
  if (communicationProtocol == "websocket") {
    opts.communicationProtocol = CommunicationProtocol::WEBSOCKET;
    return ExitCode::SUCCESS;
  }
  std::cerr
    << "Unsupported value for --communication-protocol: "
    << communicationProtocol
    << ". Must be one of: stdio; tcp; websocket."
    << std::endl;
  return ExitCode::INVALID_VALUE;
}

ExitCode validateAndSetServerProtocol(
  CommandLineOptions &opts,
  const std::string &serverProtocol
) {
  if (serverProtocol == "lsp") {
    if (opts.dataFormat != DataFormat::JSON_RPC) {
      std::cerr
        << "Only JSON-RPC is supported with LSP; must set --data-format=json-rpc."
        << std::endl;
      return ExitCode::BAD_ARG_COMBO;
    }
    opts.serverProtocol = ServerProtocol::LSP;
    return ExitCode::SUCCESS;
  }
  std::cerr
    << "Unsupported value for --server-protocol: "
    << serverProtocol
    << ". Must be one of: lsp."
    << std::endl;
  return ExitCode::INVALID_VALUE;
}

ExitCode validateAndSetTcpPort(
  CommandLineOptions &opts,
  short unsigned int tcpPort
) {
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

ExitCode validateAndSetInteractive(
  CommandLineOptions &opts,
  bool interactive
) {
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

ExitCode validateAndSetNumThreads(
  CommandLineOptions &opts,
  std::size_t numThreads
) {
  opts.numThreads = numThreads;
  return ExitCode::SUCCESS;
}

/**
 * Parses command-line arguments and stores their values in global variables.
 * @param argc Number of command-line arguments.
 * @param argv Vector of command-line arguments.
 * @return How successful the parser was at parsing the command-line arguments.
 */
int parse(CommandLineOptions &opts, int argc, char *argv[]) {
  std::string language;
  std::string dataFormat;
  std::string communicationProtocol = "stdio";
  short unsigned int tcpPort = 2078;
  std::string serverProtocol = "lsp";
  bool interactive;
  std::size_t numThreads = 5;

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
  return static_cast<int>(exitCode);
}

std::unique_ptr<ls::RequestParserFactory> buildRequestParserFactory(
  CommandLineOptions &opts
) {
  switch (opts.serverProtocol) {
  case ServerProtocol::LSP: {
    return std::make_unique<lsp::LspRequestParserFactory>(opts.interactive);
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
  ls::MessageQueue &outgoingMessages
) {
  if (opts.language == Language::FORTRAN) {
    if (opts.dataFormat == DataFormat::JSON_RPC) {
      if (opts.serverProtocol == ServerProtocol::LSP) {
        return std::make_unique<lsp::LFortranLspLanguageServer>(outgoingMessages);
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
  CommandLineOptions &opts
) -> std::unique_ptr<lst::ThreadPool> {
  return std::make_unique<lst::ThreadPool>(opts.numThreads);
}

auto buildCommunicationProtocol(
  CommandLineOptions &opts,
  ls::LanguageServer &languageServer,
  ls::RequestParserFactory &requestParserFactory,
  ls::MessageQueue &incomingMessages
) -> std::unique_ptr<ls::CommunicationProtocol> {
  switch (opts.communicationProtocol) {
  case CommunicationProtocol::STDIO: {
    std::unique_ptr<lst::ThreadPool> threadPool = buildThreadPool(opts);
    return std::make_unique<ls::StdIOCommunicationProtocol>(
      languageServer,
      requestParserFactory,
      std::move(threadPool),
      incomingMessages
    );
  }
  case CommunicationProtocol::TCP: {
    return std::make_unique<ls::TcpCommunicationProtocol>(
      languageServer,
      requestParserFactory,
      opts.tcpPort,
      incomingMessages
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
  CommandLineOptions &opts
) -> std::unique_ptr<ls::MessageQueue> {
  return std::make_unique<ls::MessageQueue>();
}

int main(int argc, char *argv[]) {
  CommandLineOptions opts;
  int retval = parse(opts, argc, argv);

  if (retval != 0) {
    return retval;
  }

  try {
    std::unique_ptr<ls::RequestParserFactory> requestParserFactory =
      buildRequestParserFactory(opts);
    std::unique_ptr<ls::MessageQueue> messageQueue = buildMessageQueue(opts);
    std::unique_ptr<ls::LanguageServer> languageServer = buildLanguageServer(opts, *messageQueue);
    std::unique_ptr<ls::CommunicationProtocol> communicationProtocol =
      buildCommunicationProtocol(opts, *languageServer, *requestParserFactory, *messageQueue);
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
