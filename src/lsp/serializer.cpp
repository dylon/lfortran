#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include <lsp/serializer.h>

namespace LCompilers::LanguageServer {

  auto JsonSerializer::serialize(
    const rapidjson::Document &document
  ) const -> std::string {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
  }

  auto JsonDeserializer::deserialize(
    const std::string &message
  ) const -> rapidjson::Document {
    rapidjson::Document document;
    document.Parse(message.c_str());
    return document;
  }

} // namespace LCompilers::LanguageServer
