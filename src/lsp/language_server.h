#pragma once

#include <string>

#include <lsp/logger.h>
#include <lsp/message_queue.h>
#include <lsp/thread_pool.h>

namespace LCompilers::LanguageServer {
  namespace lsl = LCompilers::LanguageServer::Logging;

  class LanguageServer {
  public:
    LanguageServer(
      MessageQueue &incomingMessages,
      MessageQueue &outgoingMessages,
      lsl::Logger &logger
    );
    virtual bool isTerminated() const = 0;
    virtual void join() = 0;
  protected:
    MessageQueue &incomingMessages;
    MessageQueue &outgoingMessages;
    lsl::Logger &logger;
    std::string buffer;

    auto send(const std::string &request) -> void;

    virtual void prepare(
      std::string &buffer,
      const std::string &message
    ) const = 0;
  };

} // namespace LCompilers::LanguageServer
