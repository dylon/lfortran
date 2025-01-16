#ifndef LCOMPILERS_LS_COMMUNICATION_PROTOCOL_H
#define LCOMPILERS_LS_COMMUNICATION_PROTOCOL_H

#include <memory>
#include <type_traits>
#include <utility>

#include <asio/awaitable.hpp>
#include <asio/detached.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/write.hpp>
#include <asio/read_until.hpp>

#include <lsp/language_server.h>
#include <lsp/request_parser.h>

namespace LCompilers::LanguageServer {
  using asio::ip::tcp;
  using asio::awaitable;
  using asio::co_spawn;
  using asio::detached;
  using asio::use_awaitable;

  class CommunicationProtocol {
  public:
    CommunicationProtocol(
      LanguageServer &languageServer);
    virtual void serve() = 0;
  protected:
    LanguageServer &languageServer;
  };

  class StdIOCommunicationProtocol : public CommunicationProtocol {
  public:
    StdIOCommunicationProtocol(
      LanguageServer &languageServer,
      RequestParser &parser);
    void serve() override;
  private:
    RequestParser &parser;
  };

  class TcpRequestMatchCondition {
  public:
    TcpRequestMatchCondition(RequestParser &parser);
    template <typename Iterator>
    auto operator()(Iterator begin, Iterator end) -> std::pair<Iterator, bool>;
  private:
    RequestParser &parser;
  };

  auto tcpDispatch(RequestParser &parser, tcp::socket socket) -> awaitable<void>;

  auto tcpListener(short unsigned int port) -> awaitable<void>;

  class TcpCommunicationProtocol : public CommunicationProtocol {
  public:
    TcpCommunicationProtocol(
      LanguageServer &languageServer,
      RequestParser &parser,
      short unsigned int port);
    void serve() override;
  private:
    RequestParser &parser;
    short unsigned int port;
  };

  class WebSocketCommunicationProtocol : public CommunicationProtocol {
  public:
    WebSocketCommunicationProtocol(
      LanguageServer &languageServer);
    void serve() override;
  };

} // namespace LCompilers::LanguageServer

namespace asio {
  template <> struct is_match_condition<LCompilers::LanguageServer::TcpRequestMatchCondition>
    : public std::true_type {
    // empty
  };
} // namespace asio

#endif // LCOMPILERS_LS_COMMUNICATION_PROTOCOL_H
