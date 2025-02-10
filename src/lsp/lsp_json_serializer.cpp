#include <sstream>
#include <utility>

#include <lsp/specification.h>
#include <lsp/lsp_exception.h>
#include <lsp/lsp_json_serializer.h>

namespace LCompilers::LanguageServerProtocol {

  std::string LspJsonSerializer::serialize(const LSPAny &any) const {
    std::stringstream ss;
    LSPAnyType anyType = static_cast<LSPAnyType>(any.index());
    switch (anyType) {
    case LSPAnyType::OBJECT_TYPE: {
      serializeObject(ss, std::get<LSPObject>(any));
      break;
    }
    case LSPAnyType::ARRAY_TYPE: {
      serializeArray(ss, std::get<LSPArray>(any));
      break;
    }
    default: {
      throw LSP_EXCEPTION(
        ErrorCodes::PARSE_ERROR,
        ("JSON root must be of type LSPAnyType::OBJECT_TYPE or "
         "LSPAnyType::ARRAY_TYPE, not LSPAnyType::" +
         LSPAnyTypeNames.at(anyType))
      );
    }
    }
    return ss.str();
  }

  void LspJsonSerializer::serializeArray(
    std::stringstream &ss,
    const LSPArray &array
  ) const {
    ss << "[";
    LSPArray::const_iterator iter = array.begin();
    if (iter != array.end()) {
      serializeValue(ss, **iter++);
      while (iter != array.end()) {
        ss << ",";
        serializeValue(ss, **iter++);
      }
    }
    ss << "]";
  }

  void LspJsonSerializer::serializeObject(
    std::stringstream &ss,
    const LSPObject &object
  ) const {
    ss << "{";
    LSPObject::const_iterator iter = object.begin();
    if (iter != object.end()) {
      serializeString(ss, iter->first);
      ss << ":";
      serializeValue(ss, *iter->second);
      while ((++iter) != object.end()) {
        ss << ",";
        serializeString(ss, iter->first);
        ss << ":";
        serializeValue(ss, *iter->second);
      }
    }
    ss << "}";
  }

  void LspJsonSerializer::serializeValue(
    std::stringstream &ss,
    const LSPAny &value
  ) const {
    switch (static_cast<LSPAnyType>(value.index())) {
    case LSPAnyType::OBJECT_TYPE: {
      serializeObject(ss, std::get<LSPObject>(value));
      break;
    }
    case LSPAnyType::ARRAY_TYPE: {
      serializeArray(ss, std::get<LSPArray>(value));
      break;
    }
    case LSPAnyType::STRING_TYPE: {
      serializeString(ss, value);
      break;
    }
    case LSPAnyType::INTEGER_TYPE: // fallthrough
    case LSPAnyType::UINTEGER_TYPE: // fallthrough
    case LSPAnyType::DECIMAL_TYPE: {
      serializeNumber(ss, value);
      break;
    }
    case LSPAnyType::BOOLEAN_TYPE: {
      serializeBoolean(ss, value);
      break;
    }
    case LSPAnyType::NULL_TYPE: {
      serializeNull(ss, value);
      break;
    }
    }
  }

  void LspJsonSerializer::serializeString(
    std::stringstream &ss,
    const LSPAny &value
  ) const {
    LSPAnyType valueType = static_cast<LSPAnyType>(value.index());
    switch (valueType) {
    case LSPAnyType::STRING_TYPE: {
      const string_t &string = std::get<string_t>(value);
      ss << '"';
      for (std::size_t i = 0, k = string.length(); i < k; ++i) {
        unsigned char c = string[i];
        switch (c) {
        case '"': {
          ss << "\\\"";
          break;
        }
        case '\\': {
          ss << "\\\\";
          break;
        }
        case '\n': {
          ss << "\\n";
          break;
        }
        case '\t': {
          ss << "\\t";
          break;
        }
        case '\b': {
          ss << "\\b";
          break;
        }
        case '\r': {
          ss << "\\r";
          break;
        }
        case '\f': {
          ss << "\\f";
          break;
        }
        default: {
          ss << c;
        }
        }
      }
      ss << '"';
      break;
    }
    default: {
      throw LSP_EXCEPTION(
        ErrorCodes::INVALID_PARAMS,
        ("Cannot serialize JSON string of type " +
         LSPAnyTypeNames.at(valueType))
      );
    }
    }
  }

  void LspJsonSerializer::serializeNumber(
    std::stringstream &ss,
    const LSPAny &value
  ) const {
    LSPAnyType valueType = static_cast<LSPAnyType>(value.index());
    switch (valueType) {
    case LSPAnyType::INTEGER_TYPE: {
      ss << std::get<integer_t>(value);
      break;
    }
    case LSPAnyType::UINTEGER_TYPE: {
      ss << std::get<uinteger_t>(value);
      break;
    }
    case LSPAnyType::DECIMAL_TYPE: {
      ss << std::get<decimal_t>(value);
      break;
    }
    default: {
      throw LSP_EXCEPTION(
        ErrorCodes::INVALID_PARAMS,
        ("Cannot serialize JSON number of type " +
         LSPAnyTypeNames.at(valueType))
      );
    }
    }
  }

  void LspJsonSerializer::serializeBoolean(
    std::stringstream &ss,
    const LSPAny &value
  ) const {
    LSPAnyType valueType = static_cast<LSPAnyType>(value.index());
    switch (valueType) {
    case LSPAnyType::BOOLEAN_TYPE: {
      ss << std::get<boolean_t>(value);
      break;
    }
    default: {
      throw LSP_EXCEPTION(
        ErrorCodes::INVALID_PARAMS,
        ("Cannot serialize JSON boolean of type " +
         LSPAnyTypeNames.at(valueType))
      );
    }
    }
  }

  void LspJsonSerializer::serializeNull(
    std::stringstream &ss,
    const LSPAny &value
  ) const {
    LSPAnyType valueType = static_cast<LSPAnyType>(value.index());
    switch (valueType) {
    case LSPAnyType::NULL_TYPE: {
      ss << std::get<null_t>(value);
      break;
    }
    default: {
      throw LSP_EXCEPTION(
        ErrorCodes::INVALID_PARAMS,
        ("Cannot serialize JSON null of type " +
         LSPAnyTypeNames.at(valueType))
      );
    }
    }
  }

} // namespace LCompilers::LanguageServerProtocol
