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

  std::map<InitializeErrorCodes, std::string> InitializeErrorCodesNames {
    {InitializeErrorCodes::unknownProtocolVersion, "unknownProtocolVersion"},
  }

  auto initializeErrorCodes(const std::string &name) -> InitializeErrorCodes {
    for (const auto &[key, value] : InitializeErrorCodesNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid InitializeErrorCodes name: " + name);
  }

  std::map<TextDocumentSyncKind, std::string> TextDocumentSyncKindNames = {
    {TextDocumentSyncKind::None, "None"},
    {TextDocumentSyncKind::Full, "Full"},
    {TextDocumentSyncKind::Incremental, "Incremental"},
  };

  auto textDocumentSyncKind(const std::string &name) -> TextDocumentSyncKind {
    for (const auto &[key, value] : TextDocumentSyncKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid TextDocumentSyncKind name: " + name);
  }

  std::map<TextDocumentSaveReason, std::string> TextDocumentSaveReasonNames = {
    {TextDocumentSaveReason::Manual, "Manual"},
    {TextDocumentSaveReason::AfterDelay, "AfterDelay"},
    {TextDocumentSaveReason::FocusOut, "FocusOut"},
  };

  auto textDocumentSaveReason(const std::string &name) -> TextDocumentSaveReason {
    for (const auto &[key, value] : TextDocumentSaveReasonNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid TextDocumentSaveReason name: " + name);
  }

  std::map<NotebookCellKind, std::string> NotebookCellKindNames = {
    {NotebookCellKind::Markup, "Markup"},
    {NotebookCellKind::Code, "Code"},
  };

  auto notebookCellKind(const std::string &name) -> NotebookCellKind {
    for (const auto &[key, value] : NotebookCellKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid NotebookCellKind name: " + name);
  }

} // namespace LCompilers::LanguageServiceProvider
