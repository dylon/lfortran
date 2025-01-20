#include <cctype>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <format>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <system_error>

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <asio/error.hpp>
#include <asio/io_context.hpp>

#include <lsp/communication_protocol.h>
#include <lsp/request_parser.h>

namespace LCompilers::LanguageServer {
  using namespace std::literals::chrono_literals;

  namespace this_coro = asio::this_coro;

  CommunicationProtocol::CommunicationProtocol(
    LanguageServer &languageServer,
    RequestParserFactory &parserFactory)
    : languageServer(languageServer)
    , parserFactory(parserFactory)
  {
    // empty
  }

  StdIOCommunicationProtocol::StdIOCommunicationProtocol(
    LanguageServer &languageServer,
    RequestParserFactory &parserFactory,
    std::unique_ptr<lst::ThreadPool> threadPool)
    : CommunicationProtocol(languageServer, parserFactory)
    , threadPool(std::move(threadPool))
  {
    // empty
  }

  void StdIOCommunicationProtocol::serve() {
    std::cerr
      << "Serving stdio requests with " << threadPool->getNumThreads() << " threads."
      << std::endl;
    std::unique_ptr<RequestParser> parser = parserFactory.build();
    std::string line;
    std::mutex &stdoutMutex = threadPool->getStdoutMutex();
    std::mutex &stderrMutex = threadPool->getStderrMutex();
    std::mutex printMutex;
    std::condition_variable responsePrinted;
    std::size_t requestId = 0;
    std::size_t pendingId = 0;
    try {
      bool pending = true;
      while (!languageServer.isTerminated()
             && std::getline(std::cin, line)
             // NOTE: This double-check logic is intentional because of
             // how conditionals are short-circuited.
             && !languageServer.isTerminated()) {
        for (std::size_t i = 0; (i < line.length()) && pending; i++) {
          unsigned char c = line[i];
          pending = !parser->parse(c);
        }
        if (!pending) {
          if (parser->state() == RequestParserState::COMPLETE) {
            for (auto const &[header, value] : parser->headers()) {
              std::unique_lock<std::mutex> stderrLock(stderrMutex);
              std::cerr
                << "request.headers[\"" << header << "\"] = \"" << value << "\""
                << std::endl;
            }
            const std::string &request = parser->body();
            {
              std::unique_lock<std::mutex> stderrLock(stderrMutex);
              std::cerr << "request.body = '" << request << "'" << std::endl;
            }
            if (request.length() > 0) {
              threadPool->execute([this,
                                  request,
                                  &stdoutMutex,
                                  &stderrMutex,
                                  &pendingId,
                                  requestId,
                                  &responsePrinted,
                                  &printMutex](const std::size_t threadId) {
                try {
                  const std::string response = languageServer.serve(request);
                  {
                    std::unique_lock<std::mutex> stderrLock(stderrMutex);
                    std::cerr
                      << "[thread.id=" << threadId << "] "
                      << "response = '" << response << "'"
                      << std::endl;
                  }
                  // ----------------------------------------------------------------
                  // NOTE: The LSP spec requires responses to be returned in roughly
                  // the same order of receipt of their corresponding requests. Some
                  // types of responses may be returned out-of-order, but in order to
                  // support those we will need to implement a sort of dependency
                  // graph. Without knowledge of their dependencies, we must respond
                  // to all requests in order of receipt.
                  // ----------------------------------------------------------------
                  while (pendingId < requestId) {
                    std::unique_lock<std::mutex> printLock(printMutex);
                    responsePrinted.wait_for(
                      printLock, 100ms, [&pendingId, &requestId]() {
                        return pendingId == requestId;
                      }
                    );
                  }
                  if (pendingId == requestId) {
                    {
                      std::unique_lock<std::mutex> stdoutLock(stdoutMutex);
                      std::unique_lock<std::mutex> stderrLock(stderrMutex);
                      std::cout << response << std::flush;
                      std::cerr << std::endl;
                    }
                    ++pendingId;
                  }
                } catch (std::exception &e) {
                  std::unique_lock<std::mutex> stderrLock(stderrMutex);
                  std::cerr
                    << "[thread.id=" << threadId << "] "
                    << "Failed to serve request: " << request
                    << std::endl;
                  std::cerr
                    << "[thread.id=" << threadId << "] "
                    << "Caught unhandled exception: " << e.what()
                    << std::endl;
                }
              });
            } else {
              std::unique_lock<std::mutex> stderrLock(stderrMutex);
              std::cerr << "Cannot parse an empty request body." << std::endl;
            }
          } else if (parser->state() == RequestParserState::ERROR) {
            const std::string &error = parser->error();
            std::unique_lock<std::mutex> stderrLock(stderrMutex);
            std::cerr << "Failed to parse request: " << error << std::endl;
          } else {
            std::unique_lock<std::mutex> stderrLock(stderrMutex);
            std::cerr
              << "Parser in unexpected state: " << static_cast<int>(parser->state())
              << std::endl;
          }
          parser->reset();
          ++requestId;
          pending = true;
        } else {
          parser->parse('\n');
        }
      }
      if (pending) {
        parser->finish();
        for (auto const &[header, value] : parser->headers()) {
          std::unique_lock<std::mutex> stderrLock(stderrMutex);
          std::cerr
            << "request.headers[\"" << header << "\"] = \"" << value << "\""
            << std::endl;
        }
        const std::string &request = parser->body();
        {
          std::unique_lock<std::mutex> stderrLock(stderrMutex);
          std::cerr << "request.body = " << request << std::endl;
        }
        const std::string response = languageServer.serve(request);
        {
          std::unique_lock<std::mutex> stderrLock(stderrMutex);
          std::cerr << "response = " << response << std::endl;
        }
        {
          std::unique_lock<std::mutex> stdoutLock(stdoutMutex);
          std::unique_lock<std::mutex> stderrLock(stderrMutex);
          std::cout << response << std::flush;
          std::cerr << std::endl;
        }
      }
    } catch (std::exception &e) {
      std::cerr
        << "Caught unhandled exception while serving requests: " << e.what()
        << std::endl;
    }
    threadPool->stop();
    threadPool->join();
  }

  TcpRequestMatchCondition::TcpRequestMatchCondition(
    RequestParser &parser,
    LanguageServer &languageServer
  ) : parser(parser)
    , languageServer(languageServer)
  {
    // empty
  }

  template <typename Iterator>
  auto TcpRequestMatchCondition::operator()(
    Iterator begin,
    Iterator end) -> std::pair<Iterator, bool> {
    Iterator iter = begin;
    for (; (iter != end) && !languageServer.isTerminated(); iter++) {
      unsigned char c = *iter;
      if (parser.parse(c)) {
        iter++;
        return std::make_pair(iter, true);
      }
    }
    return std::make_pair(iter, false);
  }

  TcpCommunicationProtocol::TcpCommunicationProtocol(
    LanguageServer &languageServer,
    RequestParserFactory &parserFactory,
    short unsigned int port)
    : CommunicationProtocol(languageServer, parserFactory)
    , port(port)
  {
    // empty
  }

  auto TcpCommunicationProtocol::prepareResponse(
    std::stringstream &ss,
    int statusCode,
    const std::string &statusText,
    const std::string &response
  ) -> void {
    ss << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
    ss << "Content-Type: application/json\r\n";
    ss << "Content-Length: " << response.length() << "\r\n";
    ss << "\r\n";
    ss << response;
  }

  auto TcpCommunicationProtocol::dispatch(
    tcp::socket socket
  ) -> awaitable<void> {
    try {
      std::unique_ptr<RequestParser> parser = parserFactory.build();
      TcpRequestMatchCondition matchCondition(*parser, languageServer);
      std::stringstream ss;
      for (std::string buffer; !languageServer.isTerminated();) {
        std::size_t n =
          co_await asio::async_read_until(
            socket,
            asio::dynamic_buffer(buffer, 1024),
            matchCondition,
            use_awaitable
          );

        if (languageServer.isTerminated()) {
          break;
        }

        const std::string message = buffer.substr(0, n);
        buffer.erase(0, n);

        std::cout << "~~~~~~~~" << std::endl;
        std::cout << "request:" << std::endl;
        std::cout << "~~~~~~~~" << std::endl;
        std::cout << message << std::endl;

        ss.str("");
        if (parser->state() == RequestParserState::COMPLETE) {
          const std::string &startLine = parser->startLine();
          std::cout << "request.startLine = '" << startLine << "'" << std::endl;
          for (auto const &[header, value] : parser->headers()) {
            std::cout
              << "request.headers[\"" << header << "\"] = \"" << value << "\""
              << std::endl;
          }
          const std::string &body = parser->body();
          std::cout << "request.body = '" << body << "'" << std::endl;

          try {
            const std::string response = languageServer.serve(body);
            prepareResponse(ss, 200, "OK", response);
          } catch (std::exception &e) {
            prepareResponse(
              ss, 400, "Bad Request",
              std::format("Failed to serve request: {}", e.what())
            );
          }
        } else {
          rapidjson::Document document;
          rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

          document.AddMember(
            rapidjson::Value("error", allocator),
            rapidjson::Value("Bad request", allocator),
            allocator
          );

          const std::string &error = parser->error();
          document.AddMember(
            rapidjson::Value("message", allocator),
            rapidjson::Value(error.c_str(), error.length(), allocator),
            allocator
          );

          rapidjson::StringBuffer buffer;
          rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
          document.Accept(writer);
          const std::string response = buffer.GetString();

          prepareResponse(ss, 400, "Bad Request", response);
        }

        std::cout << "~~~~~~~~~" << std::endl;
        std::cout << "response:" << std::endl;
        std::cout << "~~~~~~~~~" << std::endl;
        std::cout << ss.str() << std::endl;

        co_await async_write(socket, asio::buffer(ss.str()), use_awaitable);
        parser->reset();
      }
    } catch (std::system_error &e) {
      if (e.code() == asio::error::eof) {
        std::cout << "Connection closed." << std::endl;
      } else {
        std::fprintf(stderr, "tcpDispatch failed with error: %s\n", e.what());
      }
    } catch (std::exception &e) {
      std::fprintf(stderr, "tcpDispatch failed with error: %s\n", e.what());
    }
  }

  auto TcpCommunicationProtocol::listener() -> awaitable<void> {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), port});
    for (; !languageServer.isTerminated();) {
      tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
      co_spawn(executor, dispatch(std::move(socket)), detached);
    }
  }

  void TcpCommunicationProtocol::serve() {
    try {
      std::cout
        << "Starting TCP server; listening on port: "
        << port
        << std::endl;

      asio::io_context io_context(1);

      asio::signal_set signals(io_context, SIGINT, SIGTERM);
      signals.async_wait([&](auto, auto) { io_context.stop(); });

      co_spawn(io_context, listener(), detached);

      io_context.run();
    } catch (std::exception &e) {
      std::fprintf(
        stderr,
        "TcpCommunicationProtocol::serve() failed with error: %s\n",
        e.what()
      );
    }
  }

  WebSocketCommunicationProtocol::WebSocketCommunicationProtocol(
    LanguageServer &languageServer,
    RequestParserFactory &parserFactory)
    : CommunicationProtocol(languageServer, parserFactory)
  {
    // empty
  }

  void WebSocketCommunicationProtocol::serve() {
    throw std::runtime_error(
      "WebSocketCommunicationProtocol::serve is not yet implemented");
  }

} // namespace LCompilers::LanguageServer
