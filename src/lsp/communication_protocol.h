#ifndef LCOMPILERS_LS_COMMUNICATION_PROTOCOL_H
#define LCOMPILERS_LS_COMMUNICATION_PROTOCOL_H

#include <atomic>
#include <memory>
#include <thread>
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

#include <lsp/thread_pool.h>
#include <lsp/message_queue.h>
#include <lsp/language_server.h>
#include <lsp/request_parser.h>

namespace LCompilers::LanguageServer {
  using asio::ip::tcp;
  using asio::awaitable;
  using asio::co_spawn;
  using asio::detached;
  using asio::use_awaitable;

  namespace lst = LCompilers::LanguageServer::Threading;

  class CommunicationProtocol {
  public:
    CommunicationProtocol(
      LanguageServer &languageServer,
      RequestParserFactory &parserFactory,
      MessageQueue &incomingMessages);
    virtual void serve() = 0;
  protected:
    LanguageServer &languageServer;
    RequestParserFactory &parserFactory;
    MessageQueue &incomingMessages;
  };

  class StdIOCommunicationProtocol : public CommunicationProtocol {
  public:
    StdIOCommunicationProtocol(
      LanguageServer &languageServer,
      RequestParserFactory &parserFactory,
      std::unique_ptr<lst::ThreadPool> threadPool,
      MessageQueue &incomingMessages);
    void serve() override;
  private:
    std::unique_ptr<lst::ThreadPool> threadPool;
    std::thread messageListener;
    std::atomic_bool running = true;

    auto listen() -> void;
  };

  class TcpRequestMatchCondition {
  public:
    TcpRequestMatchCondition(
      RequestParser &parser,
      LanguageServer &languageServer
    );
    template <typename Iterator>
    auto operator()(Iterator begin, Iterator end) -> std::pair<Iterator, bool>;
  private:
    RequestParser &parser;
    LanguageServer &languageServer;
  };

  class TcpCommunicationProtocol : public CommunicationProtocol {
  public:
    TcpCommunicationProtocol(
      LanguageServer &languageServer,
      RequestParserFactory &parserFactory,
      short unsigned int port,
      MessageQueue &incomingMessages
    );
    void serve() override;
  private:
    short unsigned int port;
    std::thread messageListener;
    std::atomic_bool running = true;

    auto listener() -> awaitable<void>;
    auto dispatch(tcp::socket socket) -> awaitable<void>;
    auto prepareResponse(
      std::stringstream &ss,
      int statusCode,
      const std::string &statusText,
      const std::string &error
    ) -> void;
    auto listen() -> void;
  };

  class WebSocketCommunicationProtocol : public CommunicationProtocol {
  public:
    WebSocketCommunicationProtocol(
      LanguageServer &languageServer,
      RequestParserFactory &parserFactory,
      MessageQueue &incomingMessages
    );
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
