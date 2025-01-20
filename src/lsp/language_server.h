#ifndef LCOMPILERS_LS_LANGUAGE_SERVER_H
#define LCOMPILERS_LS_LANGUAGE_SERVER_H

#include <string>

namespace LCompilers::LanguageServer {

  class LanguageServer {
  public:
    virtual std::string serve(const std::string &request) = 0;
    virtual bool isTerminated() const = 0;
  };

} // namespace LCompilers::LanguageServer

#endif // LCOMPILERS_LS_LANGUAGE_SERVER_H
