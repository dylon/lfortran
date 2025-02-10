#pragma once

#include <lsp/specification.h>

namespace LCompilers::LanguageServerProtocol {

  class LspJsonSerializer {
  public:
    std::string serialize(const LSPAny &any) const;
  private:
    void serializeArray(std::stringstream &ss, const LSPArray &array) const;
    void serializeObject(std::stringstream &ss, const LSPObject &object) const;
    void serializeValue(std::stringstream &ss, const LSPAny &value) const;
    void serializeString(std::stringstream &ss, const LSPAny &value) const;
    void serializeNumber(std::stringstream &ss, const LSPAny &value) const;
    void serializeBoolean(std::stringstream &ss, const LSPAny &value) const;
    void serializeNull(std::stringstream &ss, const LSPAny &value) const;
  };

} // namespace LCompilers::LanguageServerProtocol
