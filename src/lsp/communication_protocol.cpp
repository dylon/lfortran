#include <asio/io_context.hpp>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>

#include <lsp/communication_protocol.h>

namespace LCompilers::LanguageServer {
  namespace this_coro = asio::this_coro;

  CommunicationProtocol::CommunicationProtocol(
    LanguageServer &languageServer)
    : languageServer(languageServer)
  {
    // empty
  }

  StdIOCommunicationProtocol::StdIOCommunicationProtocol(
    LanguageServer &languageServer,
    RequestParser &parser)
    : CommunicationProtocol(languageServer)
    , parser(parser)
  {
    // empty
  }

  void StdIOCommunicationProtocol::serve() {
    parser.reset();
    std::string request;
    while (std::getline(std::cin, request)) {
      std::cout << "request = " << request << std::endl;
      bool done = false;
      for (std::size_t i = 0; (i < request.length()) && !done; i++) {
        unsigned char c = request[i];
        done = parser.parse(c);
      }
      if (done) {
        for (auto const &[header, value] : parser.headers()) {
          std::cout << "request.headers[\"" << header << "\"] = \"" << value << "\"" << std::endl;
        }
        const std::string &body = parser.body();
        std::cout << "request.body = '" << body << "'" << std::endl;
        std::string response = languageServer.serve(body);
        std::cout << "response = '" << response << "'" << std::endl;
        parser.reset();
      } else {
        parser.parse('\r');
        parser.parse('\n');
      }
    }
  }

  TcpRequestMatchCondition::TcpRequestMatchCondition(
    RequestParser &parser)
    : parser(parser)
  {
    // empty
  }

  template <typename Iterator>
  auto TcpRequestMatchCondition::operator()(
    Iterator begin,
    Iterator end) -> std::pair<Iterator, bool> {
    std::cout << "Parsing request:" << std::endl;
    parser.reset();
    Iterator iter = begin;
    for (; iter != end; iter++) {
      unsigned char c = *iter;
      if (c == '\r') {
        std::cout << "\\r";
      } else if (c == '\n') {
        std::cout << "\\n";
      } else {
        std::cout << c;
      }
      if (parser.parse(c)) {
        iter++;
        return std::make_pair(iter, true);
      }
    }
    return std::make_pair(iter, false);
  }

  auto tcpDispatch(RequestParser &parser, tcp::socket socket) -> awaitable<void> {
    try {
      TcpRequestMatchCondition matchCondition(parser);
      for (std::string buffer;;) {
        std::size_t n =
          co_await asio::async_read_until(
            socket,
            asio::dynamic_buffer(buffer, 1024),
            matchCondition,
            use_awaitable
          );
        std::cout << std::endl;
        // std::string message = buffer.substr(0, n);
        for (auto const &[header, value] : parser.headers()) {
          std::cout << "headers[\"" << header << "\"] = \"" << value << "\"" << std::endl;
        }
        const std::string &body = parser.body();
        std::cout << "request.body = " << body << std::endl;
        buffer.erase(0, n);
      }
    } catch (std::exception &e) {
      std::fprintf(stderr, "tcpDispatch failed with error: %s\n", e.what());
    }
  }

  auto tcpListener(RequestParser &parser, short unsigned int port) -> awaitable<void> {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, {tcp::v4(), port});
    for (;;) {
      tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
      co_spawn(executor, tcpDispatch(parser, std::move(socket)), detached);
    }
  }

  TcpCommunicationProtocol::TcpCommunicationProtocol(
    LanguageServer &languageServer,
    RequestParser &parser,
    short unsigned int port)
    : CommunicationProtocol(languageServer)
    , parser(parser)
    , port(port)
  {
    // empty
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

      co_spawn(io_context, tcpListener(parser, port), detached);

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
    LanguageServer &languageServer)
    : CommunicationProtocol(languageServer)
  {
    // empty
  }

  void WebSocketCommunicationProtocol::serve() {
    throw std::runtime_error(
      "WebSocketCommunicationProtocol::serve is not yet implemented");
  }

} // namespace LCompilers::LanguageServer
