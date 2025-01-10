#include <stdexcept>

#include <lsp/specification.h>

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

  std::map<DocumentHighlightKind, std::string> DocumentHighlightKindNames = {
    {DocumentHighlightKind::Text, "Text"},
    {DocumentHighlightKind::Read, "Read"},
    {DocumentHighlightKind::Write, "Write"},
  };

  auto documentHighlightKind(const std::string &name) -> DocumentHighlightKind {
    for (const auto &[key, value] : DocumentHighlightKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid DocumentHighlightKind name: " + name);
  }

  std::map<FoldingRangeKind, std::string> FoldingRangeKindNames = {
    {FoldingRangeKind::Comment, "comment"},
    {FoldingRangeKind::Imports, "imports"},
    {FoldingRangeKind::Region, "region"},
  };

  auto foldingRangeKind(const std::string &name) -> FoldingRangeKind {
    for (const auto &[key, value] : FoldingRangeKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid FoldingRangeKind name: " + name);
  }

  std::map<SymbolKind, std::string> SymbolKindNames = {
    {SymbolKind::File, "File"},
    {SymbolKind::Module, "Module"},
    {SymbolKind::Namespace, "Namespace"},
    {SymbolKind::Package, "Package"},
    {SymbolKind::Class, "Class"},
    {SymbolKind::Method, "Method"},
    {SymbolKind::Property, "Property"},
    {SymbolKind::Field, "Field"},
    {SymbolKind::Constructor, "Constructor"},
    {SymbolKind::Enum, "Enum"},
    {SymbolKind::Interface, "Interface"},
    {SymbolKind::Function, "Function"},
    {SymbolKind::Variable, "Variable"},
    {SymbolKind::Constant, "Constant"},
    {SymbolKind::String, "String"},
    {SymbolKind::Number, "Number"},
    {SymbolKind::Boolean, "Boolean"},
    {SymbolKind::Array, "Array"},
    {SymbolKind::Object, "Object"},
    {SymbolKind::Key, "Key"},
    {SymbolKind::Null, "Null"},
    {SymbolKind::EnumMember, "EnumMember"},
    {SymbolKind::Struct, "Struct"},
    {SymbolKind::Event, "Event"},
    {SymbolKind::Operator, "Operator"},
    {SymbolKind::TypeParameter, "TypeParameter"},
  };

  auto symbolKind(const std::string &name) -> SymbolKind {
    for (const auto &[key, value] : SymbolKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid SymbolKind name: " + name);
  }

  std::map<SymbolTag, std::string> SymbolTagNames = {
    {SymbolTag::Deprecated, "Deprecated"},
  };

  auto symbolTag(const std::string &name) -> SymbolTag {
    for (const auto &[key, value] : SymbolTagNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid SymbolTag name: " + name);
  }

  std::map<SemanticTokenTypes, std::string> SemanticTokenTypeNames = {
    {SemanticTokenTypes::Namespace, "namespace"},
    {SemanticTokenTypes::Type, "type"},
    {SemanticTokenTypes::Class, "class"},
    {SemanticTokenTypes::Enum, "enum"},
    {SemanticTokenTypes::Interface, "interface"},
    {SemanticTokenTypes::Struct, "struct"},
    {SemanticTokenTypes::TypeParameter, "typeParameter"},
    {SemanticTokenTypes::Parameter, "parameter"},
    {SemanticTokenTypes::Variable, "variable"},
    {SemanticTokenTypes::Property, "property"},
    {SemanticTokenTypes::EnumMember, "enumMember"},
    {SemanticTokenTypes::Event, "event"},
    {SemanticTokenTypes::Function, "function"},
    {SemanticTokenTypes::Method, "method"},
    {SemanticTokenTypes::Macro, "macro"},
    {SemanticTokenTypes::Keyword, "keyword"},
    {SemanticTokenTypes::Modifier, "modifier"},
    {SemanticTokenTypes::Comment, "comment"},
    {SemanticTokenTypes::String, "string"},
    {SemanticTokenTypes::Number, "number"},
    {SemanticTokenTypes::Regexp, "regexp"},
    {SemanticTokenTypes::Operator, "operator"},
    {SemanticTokenTypes::Decorator, "decorator"},
  };

  auto SemanticTokenTypes(const std::string &name) -> SemanticTokenTypes {
    for (const auto &[key, value] : SemanticTokenTypeNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid SemanticTokenTypes name: " + name);
  }

  std::map<SemanticTokenModifiers, std::string> SemanticTokenModifierNames = {
    {SemanticTokenModifiers::Declaration, "declaration"},
    {SemanticTokenModifiers::Definition, "definition"},
    {SemanticTokenModifiers::Readonly, "readonly"},
    {SemanticTokenModifiers::Static, "static"},
    {SemanticTokenModifiers::Deprecated, "deprecated"},
    {SemanticTokenModifiers::Abstract, "abstract"},
    {SemanticTokenModifiers::Async, "async"},
    {SemanticTokenModifiers::Modification, "modification"},
    {SemanticTokenModifiers::Documentation, "documentation"},
    {SemanticTokenModifiers::DefaultLibrary, "defaultLibrary"},
  };

  auto semanticTokenModifiers(const std::string &name) -> SemanticTokenModifiers {
    for (const auto &[key, value] : SemanticTokenModifierNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid SemanticTokenModifiers name: " + name);
  }

  std::map<TokenFormat, std::string> TokenFormatNames = {
    {TokenFormat::Relative, "relative"},
  };

  auto tokenFormat(const std::string &name) -> TokenFormat {
    for (const auto &[key, value] : TokenFormatNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid TokenFormats name: " + name);
  }

  std::map<InlayHintKind, std::string> InlayHintKindNames = {
    {InlayHintKind::Type, "Type"},
    {InlayHintKind::Parameter, "Parameter"},
  };

  auto inlayHintKind(const std::string &name) -> InlayHintKind {
    for (const auto &[key, value] : InlayHintKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid InlayHintKind name: " + name);
  }

  std::map<UniquenessLevel, std::string> UniquenessLevelNames = {
    {UniquenessLevel::Document, "document"},
    {UniquenessLevel::Project, "project"},
    {UniquenessLevel::Group, "group"},
    {UniquenessLevel::Scheme, "scheme"},
    {UniquenessLevel::Global, "global"},
  };

  auto uniquenessLevel(const std::string &name) -> UniquenessLevel {
    for (const auto &[key, value] : UniquenessLevelNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid UniquenessLevels name: " + name);
  }

  std::map<MonikerKind, std::string> MonikerKindNames = {
    {MonikerKind::Import, "import"},
    {MonikerKind::Export, "export"},
    {MonikerKind::Local, "local"},
  };

  auto monikerKind(const std::string &name) -> MonikerKind {
    for (const auto &[key, value] : MonikerKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid MonikerKind name: " + name);
  }

  std::map<CompletionTriggerKind, std::string> CompletionTriggerKindNames = {
    {CompletionTriggerKind::Invoked, "Invoked"},
    {CompletionTriggerKind::TriggerCharacter, "TriggerCharacter"},
    {CompletionTriggerKind::TriggerForIncompleteCompletions, "TriggerForIncompleteCompletions"},
  };

  auto completionTriggerKind(const std::string &name) -> CompletionTriggerKind {
    for (const auto &[key, value] : CompletionTriggerKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid CompletionTriggerKind name: " + name);
  }

  std::map<InsertTextFormat, std::string> InsertTextFormatNames = {
    {InsertTextFormat::PlainText, "PlainText"},
    {InsertTextFormat::Snippet, "Snippet"},
  };

  auto insertTextFormat(const std::string &name) -> InsertTextFormat {
    for (const auto &[key, value] : InsertTextFormatNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid InsertTextFormat name: " + name);
  }

  std::map<CompletionItemTag, std::string> CompletionItemTagNames = {
    {CompletionItemTag::Deprecated, "Deprecated"},
  };

  auto completionItemTag(const std::string &name) -> CompletionItemTag {
    for (const auto &[key, value] : CompletionItemTagNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid CompletionItemTag name: " + name);
  }

  std::map<InsertTextMode, std::string> InsertTextModeNames = {
    {InsertTextMode::asIs, "asIs"},
    {InsertTextMode::adjustIndentation, "adjustIndentation"},
  };

  auto insertTextMode(const std::string &name) -> InsertTextMode {
    for (const auto &[key, value] : InsertTextModeNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid InsertTextMode name: " + name);
  }

  std::map<CompletionItemKind, std::string> CompletionItemKindNames = {
    {CompletionItemKind::Text, "Text"},
    {CompletionItemKind::Method, "Method"},
    {CompletionItemKind::Function, "Function"},
    {CompletionItemKind::Constructor, "Constructor"},
    {CompletionItemKind::Field, "Field"},
    {CompletionItemKind::Variable, "Variable"},
    {CompletionItemKind::Class, "Class"},
    {CompletionItemKind::Interface, "Interface"},
    {CompletionItemKind::Module, "Module"},
    {CompletionItemKind::Property, "Property"},
    {CompletionItemKind::Unit, "Unit"},
    {CompletionItemKind::Value, "Value"},
    {CompletionItemKind::Enum, "Enum"},
    {CompletionItemKind::Keyword, "Keyword"},
    {CompletionItemKind::Snippet, "Snippet"},
    {CompletionItemKind::Color, "Color"},
    {CompletionItemKind::File, "File"},
    {CompletionItemKind::Reference, "Reference"},
    {CompletionItemKind::Folder, "Folder"},
    {CompletionItemKind::EnumMember, "EnumMember"},
    {CompletionItemKind::Constant, "Constant"},
    {CompletionItemKind::Struct, "Struct"},
    {CompletionItemKind::Event, "Event"},
    {CompletionItemKind::Operator, "Operator"},
    {CompletionItemKind::TypeParameter, "TypeParameter"},
  };

  auto completionItemKind(const std::string &name) -> CompletionItemKind {
    for (const auto &[key, value] : CompletionItemKindNames) {
      if (name == value) {
        return key;
      }
    }
    throw std::invalid_argument("Invalid CompletionItemKind name: " + name);
  }

} // namespace LCompilers::LanguageServiceProvider
