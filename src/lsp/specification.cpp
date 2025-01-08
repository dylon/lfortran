#include <stdexcept>

#include <lsp/specification.h>
#include <stdexcept>

namespace LCompilers::LanguageServiceProvider {

  std::map<PositionEncodingKind, std::string> PositionEncodingKindNames = {
    {PositionEncodingKind::UTF8, "utf-8"},
    {PositionEncodingKind::UTF16, "utf-16"},
    {PositionEncodingKind::UTF32, "utf-32"},
  };

  auto positionEncodingKind(const std::string &name) -> PositionEncodingKind {
    for (const auto &[key, value] : PositionEncodingKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid PositionEncodingKind name: " + name);
  }

  std::map<DiagnosticSeverity, std::string> DiagnosticSeverityNames = {
    {DiagnosticSeverity::Error, "Error"},
    {DiagnosticSeverity::Warning, "Warning"},
    {DiagnosticSeverity::Information, "Information"},
    {DiagnosticSeverity::Hint, "Hint"},
  };

  auto diagnosticSeverity(const std::string &name) -> DiagnosticSeverity {
    for (const auto &[key, value] : DiagnosticSeverityNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid DiagnosticSeverity name: " + name);
  }

  std::map<DiagnosticTag, std::string> DiagnosticTagNames = {
    {DiagnosticTag::Unnecessary, "Unnecessary"},
    {DiagnosticTag::Deprecated, "Deprecated"},
  };

  auto diagnosticTag(const std::string &name) -> DiagnosticTag {
    for (const auto &[key, value] : DiagnosticTagNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid DiagnosticTag name: " + name);
  }

  std::map<MarkupKind, std::string> MarkupKindNames = {
    {MarkupKind::PlainText, "plaintext"},
    {MarkupKind::Markdown, "markdown"},
  };

  auto markupKind(const std::string &name) -> MarkupKind {
    for (const auto &[key, value] : MarkupKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid MarkupKind name: " + name);
  }

  std::map<ResourceOperationKind, std::string> ResourceOperationKindNames = {
    {ResourceOperationKind::Create, "create"},
    {ResourceOperationKind::Rename, "rename"},
    {ResourceOperationKind::Delete, "delete"},
  };

  auto resourceOperationKind(const std::string &name) -> ResourceOperationKind {
    for (const auto &[key, value] : ResourceOperationKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid ResourceOperationKind name: " + name);
  }

  std::map<FailureHandlingKind, std::string> FailureHandlingKindNames = {
    {FailureHandlingKind::Abort, "abort"},
    {FailureHandlingKind::Transactional, "transactional"},
    {FailureHandlingKind::TextOnlyTransactional, "textOnlyTransactional"},
    {FailureHandlingKind::Undo, "undo"},
  };

  auto failureHandlingKind(const std::string &name) -> FailureHandlingKind {
    for (const auto &[key, value] : FailureHandlingKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid FailureHandlingKind name: " + name);
  }

  std::map<TraceValue, std::string> TraceValueNames = {
    {TraceValue::OFF, "off"},
    {TraceValue::MESSAGES, "messages"},
    {TraceValue::VERBOSE, "verbose"},
  };

  auto traceValue(const std::string &name) -> TraceValue {
    for (const auto &[key, value] : TraceValueNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid TraceValue name: " + name);
  }

} // namespace LCompilers::LanguageServiceProvider
