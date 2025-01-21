#ifndef LCOMPILERS_LS_LANGUAGE_SERVER_H
#define LCOMPILERS_LS_LANGUAGE_SERVER_H

#include <ostream>
#include <string>
#include <sstream>

#include <lsp/message_queue.h>

namespace LCompilers::LanguageServer {

  class LanguageServer {
  public:
    LanguageServer(MessageQueue &outgoingMessages);
    virtual std::string serve(const std::string &request) = 0;
    virtual bool isTerminated() const = 0;
    virtual void prepare(
      std::ostream &os,
      const std::string &response
    ) const = 0;
    virtual void prepare(
      std::stringstream &ss,
      const std::string &response
    ) const = 0;
  protected:
    MessageQueue &outgoingMessages;
  };

} // namespace LCompilers::LanguageServer

#endif // LCOMPILERS_LS_LANGUAGE_SERVER_H
