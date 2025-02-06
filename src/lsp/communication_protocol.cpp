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
    RequestParserFactory &parserFactory,
    MessageQueue &incomingMessages,
    lsl::Logger &logger)
    : languageServer(languageServer)
    , parserFactory(parserFactory)
    , incomingMessages(incomingMessages)
    , logger(logger)
  {
    // empty
  }

  StdIOCommunicationProtocol::StdIOCommunicationProtocol(
    LanguageServer &languageServer,
    RequestParserFactory &parserFactory,
    std::unique_ptr<lst::ThreadPool> threadPool,
    MessageQueue &incomingMessages,
    lsl::Logger &logger)
    : CommunicationProtocol(
      languageServer,
      parserFactory,
      incomingMessages,
      logger
    )
    , threadPool(std::move(threadPool))
    , messageListener([this]() {
      listen();
    })
  {
    // empty
  }

  auto StdIOCommunicationProtocol::listen() -> void {
    std::mutex &stdoutMutex = threadPool->getStdoutMutex();
    std::mutex &stderrMutex = threadPool->getStderrMutex();
    do {
      const std::string message = incomingMessages.dequeue();
      std::unique_lock<std::mutex> stdoutLock(stdoutMutex);
      std::unique_lock<std::mutex> stderrLock(stderrMutex);
      ss.str("");
      languageServer.prepare(ss, message);
      std::cout << ss.str() << std::flush;
      logger << ss.str() << std::endl;
    } while (running);
  }

  void StdIOCommunicationProtocol::serve() {
    logger
      << "Serving stdio requests with " << threadPool->getNumThreads() << " threads."
      << std::endl;
    std::unique_ptr<RequestParser> parser = parserFactory.build();
    // std::string line;
    std::mutex &stdoutMutex = threadPool->getStdoutMutex();
    std::mutex &stderrMutex = threadPool->getStderrMutex();
    std::mutex printMutex;
    std::condition_variable responsePrinted;
    std::size_t requestId = 0;
    std::size_t pendingId = 0;
    try {
      bool pending = true;
      while (!languageServer.isTerminated()) {
        char c;
        std::cin.get(c);
        if (!languageServer.isTerminated()) {
          pending = !parser->parse(static_cast<unsigned char>(c));
          if (!pending) {
            if (parser->state() == RequestParserState::COMPLETE) {
              for (auto const &[header, value] : parser->headers()) {
                std::unique_lock<std::mutex> stderrLock(stderrMutex);
                logger
                  << "request.headers[\"" << header << "\"] = \"" << value << "\""
                  << std::endl;
              }
              const std::string &request = parser->body();
              {
                std::unique_lock<std::mutex> stderrLock(stderrMutex);
                logger << "request.body = '" << request << "'" << std::endl;
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
                      logger
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
                        ss.str("");
                        languageServer.prepare(ss, response);
                        std::cout << ss.str() << std::flush;
                        logger << ss.str() << std::endl;
                      }
                      ++pendingId;
                    }
                  } catch (std::exception &e) {
                    std::unique_lock<std::mutex> stderrLock(stderrMutex);
                    logger
                      << "[thread.id=" << threadId << "] "
                      << "Failed to serve request: " << request
                      << std::endl;
                    logger
                      << "[thread.id=" << threadId << "] "
                      << "Caught unhandled exception: " << e.what()
                      << std::endl;
                  }
                });
              } else {
                std::unique_lock<std::mutex> stderrLock(stderrMutex);
                logger << "Cannot parse an empty request body." << std::endl;
              }
            } else if (parser->state() == RequestParserState::ERROR) {
              const std::string &error = parser->error();
              std::unique_lock<std::mutex> stderrLock(stderrMutex);
              logger << "Failed to parse request: " << error << std::endl;
            } else {
              std::unique_lock<std::mutex> stderrLock(stderrMutex);
              logger
                << "Parser in unexpected state: " << static_cast<int>(parser->state())
                << std::endl;
            }
            parser->reset();
            ++requestId;
            pending = true;
          }
        }
      }
      if (pending) {
        parser->finish();
        for (auto const &[header, value] : parser->headers()) {
          std::unique_lock<std::mutex> stderrLock(stderrMutex);
          logger
            << "request.headers[\"" << header << "\"] = \"" << value << "\""
            << std::endl;
        }
        const std::string &request = parser->body();
        {
          std::unique_lock<std::mutex> stderrLock(stderrMutex);
          logger << "request.body = " << request << std::endl;
        }
        const std::string response = languageServer.serve(request);
        {
          std::unique_lock<std::mutex> stderrLock(stderrMutex);
          logger << "response = " << response << std::endl;
        }
        {
          std::unique_lock<std::mutex> stdoutLock(stdoutMutex);
          std::unique_lock<std::mutex> stderrLock(stderrMutex);
          logger << response << std::flush;
          logger << std::endl;
        }
      }
    } catch (std::exception &e) {
      logger
        << "Caught unhandled exception while serving requests: " << e.what()
        << std::endl;
    }
    running = false;
    incomingMessages.stop();
    threadPool->stop();
    threadPool->join();
    messageListener.join();
  }

  TcpRequestMatchCondition::TcpRequestMatchCondition(
    RequestParser &parser,
    LanguageServer &languageServer,
    lsl::Logger &logger
  ) : parser(parser)
    , languageServer(languageServer)
    , logger(logger)
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
    short unsigned int port,
    std::size_t numThreads,
    MessageQueue &incomingMessages,
    lsl::Logger &logger)
    : CommunicationProtocol(
      languageServer,
      parserFactory,
      incomingMessages,
      logger
    )
    , io_context(numThreads)
    , port(port)
    , messageListener([this]() {
      listen();
    })
  {
    // empty
  }

  auto TcpCommunicationProtocol::listen() -> void {
    std::stringstream ss;
    try {
      do {
        const std::string message = incomingMessages.dequeue();
        ss.str("");
        prepareResponse(ss, 200, "OK", message);
        const std::string output = ss.str();
        logger << output << std::endl;
      } while (running);
    } catch (std::runtime_error &e) {
      if (running) {
        throw e;
      }
    }
  }

  auto TcpCommunicationProtocol::prepareResponse(
    std::stringstream &ss,
    int statusCode,
    const std::string &statusText,
    const std::string &response
  ) -> void {
    ss << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
    languageServer.prepare(ss, response);
  }

  auto TcpCommunicationProtocol::dispatch(
    tcp::socket socket
  ) -> awaitable<void> {
    try {
      std::unique_ptr<RequestParser> parser = parserFactory.build();
      TcpRequestMatchCondition matchCondition(*parser, languageServer, logger);
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

        logger << "~~~~~~~~" << std::endl;
        logger << "request:" << std::endl;
        logger << "~~~~~~~~" << std::endl;
        logger << message << std::endl;

        ss.str("");
        if (parser->state() == RequestParserState::COMPLETE) {
          const std::string &startLine = parser->startLine();
          logger << "request.startLine = '" << startLine << "'" << std::endl;
          for (auto const &[header, value] : parser->headers()) {
            logger
              << "request.headers[\"" << header << "\"] = \"" << value << "\""
              << std::endl;
          }
          const std::string &body = parser->body();
          logger << "request.body = '" << body << "'" << std::endl;

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

        logger << "~~~~~~~~~" << std::endl;
        logger << "response:" << std::endl;
        logger << "~~~~~~~~~" << std::endl;
        logger << ss.str() << std::endl;

        co_await async_write(socket, asio::buffer(ss.str()), use_awaitable);
        parser->reset();
      }
    } catch (std::system_error &e) {
      if (e.code() == asio::error::eof) {
        logger << "Connection closed." << std::endl;
      } else {
        logger
          << "tcpDispatch failed with error: " << e.what()
          << std::endl;
      }
    } catch (std::exception &e) {
      logger
        << "tcpDispatch failed with error: " << e.what()
        << std::endl;
    }
    if (languageServer.isTerminated()) {
      io_context.stop();
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
      logger
        << "Starting TCP server; listening on port: "
        << port
        << std::endl;

      asio::signal_set signals(io_context, SIGINT, SIGTERM);
      signals.async_wait([&](auto, auto) { io_context.stop(); });

      co_spawn(io_context, listener(), detached);

      io_context.run();

      running = false;
      incomingMessages.stop();
      messageListener.join();
    } catch (std::exception &e) {
      logger
        << "TcpCommunicationProtocol::serve() failed with error: " << e.what()
        << std::endl;
    }
  }

  WebSocketCommunicationProtocol::WebSocketCommunicationProtocol(
    LanguageServer &languageServer,
    RequestParserFactory &parserFactory,
    MessageQueue &incomingMessages,
    lsl::Logger &logger
  ) : CommunicationProtocol(
      languageServer,
      parserFactory,
      incomingMessages,
      logger
    )
  {
    // empty
  }

  void WebSocketCommunicationProtocol::serve() {
    throw std::runtime_error(
      "WebSocketCommunicationProtocol::serve is not yet implemented");
  }

} // namespace LCompilers::LanguageServer
