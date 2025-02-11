#pragma once

namespace LCompilers::LanguageServer {

  class MessageStream {
  public:
    virtual std::string next() = 0;
  };

} // namespace LCompilers::LanguageServer
