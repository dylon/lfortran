#ifndef LCOMPILERS_LS_SERIALIZER_H
#define LCOMPILERS_LS_SERIALIZER_H

#include <string>

#include <rapidjson/document.h>

namespace LCompilers::LanguageServer {

  template <typename T>
  class Serializer {
  public:
    virtual std::string serialize(const T &response) const = 0;
  };

  template <typename T>
  class Deserializer {
  public:
    virtual T deserialize(const std::string &message) const = 0;
  };

  class JsonSerializer : public Serializer<rapidjson::Document> {
  public:
    std::string serialize(const rapidjson::Document &document) const override;
  };

  class JsonDeserializer : public Deserializer<rapidjson::Document> {
  public:
    rapidjson::Document deserialize(const std::string &message) const override;
  };

} // namespace LCompilers::LanguageServer

#endif // LCOMPILERS_LS_SERIALIZER_H
