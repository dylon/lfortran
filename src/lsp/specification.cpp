#include <stdexcept>

#include <lsp/specification.h>

namespace LCompilers::LanguageServiceProtocol {

  std::map<PositionEncodingKind, std::string> PositionEncodingKindNames = {
    {PositionEncodingKind::UTF8, "UTF8"},
    {PositionEncodingKind::UTF16, "UTF16"},
    {PositionEncodingKind::UTF32, "UTF32"},
  };

  auto positionEncodingKindByName(const std::string &name) -> PositionEncodingKind {
    for (const auto &[enum_key, enum_value] : PositionEncodingKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid PositionEncodingKind name: " + name);
  }

  std::map<PositionEncodingKind, std::string> PositionEncodingKindValues = {
    {PositionEncodingKind::UTF8, "utf-8"},
    {PositionEncodingKind::UTF16, "utf-16"},
    {PositionEncodingKind::UTF32, "utf-32"},
  };

  auto positionEncodingKindByValue(const std::string &value) -> PositionEncodingKind {
    for (const auto &[enum_key, enum_value] : PositionEncodingKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid PositionEncodingKind value: " + value);
  }

  std::map<DiagnosticSeverity, std::string> DiagnosticSeverityNames = {
    {DiagnosticSeverity::Error, "Error"},
    {DiagnosticSeverity::Warning, "Warning"},
    {DiagnosticSeverity::Information, "Information"},
    {DiagnosticSeverity::Hint, "Hint"},
  };

  auto diagnosticSeverityByName(const std::string &name) -> DiagnosticSeverity {
    for (const auto &[enum_key, enum_value] : DiagnosticSeverityNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid DiagnosticSeverity name: " + name);
  }

  std::map<DiagnosticTag, std::string> DiagnosticTagNames = {
    {DiagnosticTag::Unnecessary, "Unnecessary"},
    {DiagnosticTag::Deprecated, "Deprecated"},
  };

  auto diagnosticTagByName(const std::string &name) -> DiagnosticTag {
    for (const auto &[enum_key, enum_value] : DiagnosticTagNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid DiagnosticTag name: " + name);
  }

  std::map<MarkupKind, std::string> MarkupKindNames = {
    {MarkupKind::PlainText, "PlainText"},
    {MarkupKind::Markdown, "Markdown"},
  };

  auto markupKindByName(const std::string &name) -> MarkupKind {
    for (const auto &[enum_key, enum_value] : MarkupKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid MarkupKind name: " + name);
  }

  std::map<MarkupKind, std::string> MarkupKindValues = {
    {MarkupKind::PlainText, "plaintext"},
    {MarkupKind::Markdown, "markdown"},
  };

  auto markupKindByValue(const std::string &value) -> MarkupKind {
    for (const auto &[enum_key, enum_value] : MarkupKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid MarkupKind value: " + value);
  }

  std::map<ResourceOperationKind, std::string> ResourceOperationKindNames = {
    {ResourceOperationKind::Create, "Create"},
    {ResourceOperationKind::Rename, "Rename"},
    {ResourceOperationKind::Delete, "Delete"},
  };

  auto resourceOperationKindByName(const std::string &name) -> ResourceOperationKind {
    for (const auto &[enum_key, enum_value] : ResourceOperationKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid ResourceOperationKind name: " + name);
  }

  std::map<ResourceOperationKind, std::string> ResourceOperationKindValues = {
    {ResourceOperationKind::Create, "create"},
    {ResourceOperationKind::Rename, "rename"},
    {ResourceOperationKind::Delete, "delete"},
  };

  auto resourceOperationKindByValue(const std::string &value) -> ResourceOperationKind {
    for (const auto &[enum_key, enum_value] : ResourceOperationKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid ResourceOperationKind value: " + value);
  }

  std::map<FailureHandlingKind, std::string> FailureHandlingKindNames = {
    {FailureHandlingKind::Abort, "Abort"},
    {FailureHandlingKind::Transactional, "Transactional"},
    {FailureHandlingKind::TextOnlyTransactional, "TextOnlyTransactional"},
    {FailureHandlingKind::Undo, "Undo"},
  };

  auto failureHandlingKindByName(const std::string &name) -> FailureHandlingKind {
    for (const auto &[enum_key, enum_value] : FailureHandlingKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid FailureHandlingKind name: " + name);
  }

  std::map<FailureHandlingKind, std::string> FailureHandlingKindValues = {
    {FailureHandlingKind::Abort, "abort"},
    {FailureHandlingKind::Transactional, "transactional"},
    {FailureHandlingKind::TextOnlyTransactional, "textOnlyTransactional"},
    {FailureHandlingKind::Undo, "undo"},
  };

  auto failureHandlingKindByValue(const std::string &value) -> FailureHandlingKind {
    for (const auto &[enum_key, enum_value] : FailureHandlingKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid FailureHandlingKind value: " + value);
  }

  std::map<TraceValue, std::string> TraceValueNames = {
    {TraceValue::OFF, "OFF"},
    {TraceValue::MESSAGES, "MESSAGES"},
    {TraceValue::VERBOSE, "VERBOSE"},
  };

  auto traceValueByName(const std::string &name) -> TraceValue {
    for (const auto &[enum_key, enum_value] : TraceValueNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid TraceValue name: " + name);
  }

  std::map<TraceValue, std::string> TraceValueValues = {
    {TraceValue::OFF, "off"},
    {TraceValue::MESSAGES, "messages"},
    {TraceValue::VERBOSE, "verbose"},
  };

  auto traceValueByValue(const std::string &value) -> TraceValue {
    for (const auto &[enum_key, enum_value] : TraceValueValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid TraceValue value: " + value);
  }

  std::map<InitializeErrorCodes, std::string> InitializeErrorCodesNames {
    {InitializeErrorCodes::unknownProtocolVersion, "unknownProtocolVersion"},
  };

  auto initializeErrorCodesByName(const std::string &name) -> InitializeErrorCodes {
    for (const auto &[enum_key, enum_value] : InitializeErrorCodesNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid InitializeErrorCodes name: " + name);
  }

  std::map<TextDocumentSyncKind, std::string> TextDocumentSyncKindNames = {
    {TextDocumentSyncKind::None, "None"},
    {TextDocumentSyncKind::Full, "Full"},
    {TextDocumentSyncKind::Incremental, "Incremental"},
  };

  auto textDocumentSyncKindByName(const std::string &name) -> TextDocumentSyncKind {
    for (const auto &[enum_key, enum_value] : TextDocumentSyncKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid TextDocumentSyncKind name: " + name);
  }

  std::map<TextDocumentSaveReason, std::string> TextDocumentSaveReasonNames = {
    {TextDocumentSaveReason::Manual, "Manual"},
    {TextDocumentSaveReason::AfterDelay, "AfterDelay"},
    {TextDocumentSaveReason::FocusOut, "FocusOut"},
  };

  auto textDocumentSaveReasonByName(const std::string &name) -> TextDocumentSaveReason {
    for (const auto &[enum_key, enum_value] : TextDocumentSaveReasonNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid TextDocumentSaveReason name: " + name);
  }

  std::map<NotebookCellKind, std::string> NotebookCellKindNames = {
    {NotebookCellKind::Markup, "Markup"},
    {NotebookCellKind::Code, "Code"},
  };

  auto notebookCellKindByName(const std::string &name) -> NotebookCellKind {
    for (const auto &[enum_key, enum_value] : NotebookCellKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid NotebookCellKind name: " + name);
  }

  std::map<DocumentHighlightKind, std::string> DocumentHighlightKindNames = {
    {DocumentHighlightKind::Text, "Text"},
    {DocumentHighlightKind::Read, "Read"},
    {DocumentHighlightKind::Write, "Write"},
  };

  auto documentHighlightKindByName(const std::string &name) -> DocumentHighlightKind {
    for (const auto &[enum_key, enum_value] : DocumentHighlightKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid DocumentHighlightKind name: " + name);
  }

  std::map<FoldingRangeKind, std::string> FoldingRangeKindNames = {
    {FoldingRangeKind::Comment, "Comment"},
    {FoldingRangeKind::Imports, "Imports"},
    {FoldingRangeKind::Region, "Region"},
  };

  auto foldingRangeKindByName(const std::string &name) -> FoldingRangeKind {
    for (const auto &[enum_key, enum_value] : FoldingRangeKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid FoldingRangeKind name: " + name);
  }

  std::map<FoldingRangeKind, std::string> FoldingRangeKindValues = {
    {FoldingRangeKind::Comment, "comment"},
    {FoldingRangeKind::Imports, "imports"},
    {FoldingRangeKind::Region, "region"},
  };

  auto foldingRangeKindByValue(const std::string &value) -> FoldingRangeKind {
    for (const auto &[enum_key, enum_value] : FoldingRangeKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid FoldingRangeKind value: " + value);
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

  auto symbolKindByName(const std::string &name) -> SymbolKind {
    for (const auto &[enum_key, enum_value] : SymbolKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid SymbolKind name: " + name);
  }

  std::map<SymbolTag, std::string> SymbolTagNames = {
    {SymbolTag::Deprecated, "Deprecated"},
  };

  auto symbolTagByName(const std::string &name) -> SymbolTag {
    for (const auto &[enum_key, enum_value] : SymbolTagNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid SymbolTag name: " + name);
  }

  std::map<SemanticTokenTypes, std::string> SemanticTokenTypeNames = {
    {SemanticTokenTypes::Namespace, "Namespace"},
    {SemanticTokenTypes::Type, "Type"},
    {SemanticTokenTypes::Class, "Class"},
    {SemanticTokenTypes::Enum, "Enum"},
    {SemanticTokenTypes::Interface, "Interface"},
    {SemanticTokenTypes::Struct, "Struct"},
    {SemanticTokenTypes::TypeParameter, "TypeParameter"},
    {SemanticTokenTypes::Parameter, "Parameter"},
    {SemanticTokenTypes::Variable, "Variable"},
    {SemanticTokenTypes::Property, "Property"},
    {SemanticTokenTypes::EnumMember, "EnumMember"},
    {SemanticTokenTypes::Event, "Event"},
    {SemanticTokenTypes::Function, "Function"},
    {SemanticTokenTypes::Method, "Method"},
    {SemanticTokenTypes::Macro, "Macro"},
    {SemanticTokenTypes::Keyword, "Keyword"},
    {SemanticTokenTypes::Modifier, "Modifier"},
    {SemanticTokenTypes::Comment, "Comment"},
    {SemanticTokenTypes::String, "String"},
    {SemanticTokenTypes::Number, "Number"},
    {SemanticTokenTypes::Regexp, "Regexp"},
    {SemanticTokenTypes::Operator, "Operator"},
    {SemanticTokenTypes::Decorator, "Decorator"},
  };

  auto semanticTokenTypesByName(const std::string &name) -> SemanticTokenTypes {
    for (const auto &[enum_key, enum_value] : SemanticTokenTypeNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid SemanticTokenTypes name: " + name);
  }

  std::map<SemanticTokenTypes, std::string> SemanticTokenTypeValues = {
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

  auto semanticTokenTypesByValue(const std::string &value) -> SemanticTokenTypes {
    for (const auto &[enum_key, enum_value] : SemanticTokenTypeValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid SemanticTokenTypes value: " + value);
  }

  std::map<SemanticTokenModifiers, std::string> SemanticTokenModifierNames = {
    {SemanticTokenModifiers::Declaration, "Declaration"},
    {SemanticTokenModifiers::Definition, "Definition"},
    {SemanticTokenModifiers::Readonly, "Readonly"},
    {SemanticTokenModifiers::Static, "Static"},
    {SemanticTokenModifiers::Deprecated, "Deprecated"},
    {SemanticTokenModifiers::Abstract, "Abstract"},
    {SemanticTokenModifiers::Async, "Async"},
    {SemanticTokenModifiers::Modification, "Modification"},
    {SemanticTokenModifiers::Documentation, "Documentation"},
    {SemanticTokenModifiers::DefaultLibrary, "DefaultLibrary"},
  };

  auto semanticTokenModifiersByName(const std::string &name) -> SemanticTokenModifiers {
    for (const auto &[enum_key, enum_value] : SemanticTokenModifierNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid SemanticTokenModifiers name: " + name);
  }

  std::map<SemanticTokenModifiers, std::string> SemanticTokenModifierValues = {
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

  auto semanticTokenModifiersByValue(const std::string &value) -> SemanticTokenModifiers {
    for (const auto &[enum_key, enum_value] : SemanticTokenModifierValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid SemanticTokenModifiers value: " + value);
  }

  std::map<TokenFormat, std::string> TokenFormatNames = {
    {TokenFormat::Relative, "Relative"},
  };

  auto tokenFormatByName(const std::string &name) -> TokenFormat {
    for (const auto &[enum_key, enum_value] : TokenFormatNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid TokenFormats name: " + name);
  }

  std::map<TokenFormat, std::string> TokenFormatValues = {
    {TokenFormat::Relative, "relative"},
  };

  auto tokenFormatByValue(const std::string &value) -> TokenFormat {
    for (const auto &[enum_key, enum_value] : TokenFormatValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid TokenFormats value: " + value);
  }

  std::map<InlayHintKind, std::string> InlayHintKindNames = {
    {InlayHintKind::Type, "Type"},
    {InlayHintKind::Parameter, "Parameter"},
  };

  auto inlayHintKindByName(const std::string &name) -> InlayHintKind {
    for (const auto &[enum_key, enum_value] : InlayHintKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid InlayHintKind name: " + name);
  }

  std::map<UniquenessLevel, std::string> UniquenessLevelNames = {
    {UniquenessLevel::Document, "Document"},
    {UniquenessLevel::Project, "Project"},
    {UniquenessLevel::Group, "Group"},
    {UniquenessLevel::Scheme, "Scheme"},
    {UniquenessLevel::Global, "Global"},
  };

  auto uniquenessLevelByName(const std::string &name) -> UniquenessLevel {
    for (const auto &[enum_key, enum_value] : UniquenessLevelNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid UniquenessLevels name: " + name);
  }

  std::map<UniquenessLevel, std::string> UniquenessLevelValues = {
    {UniquenessLevel::Document, "document"},
    {UniquenessLevel::Project, "project"},
    {UniquenessLevel::Group, "group"},
    {UniquenessLevel::Scheme, "scheme"},
    {UniquenessLevel::Global, "global"},
  };

  auto uniquenessLevelByValue(const std::string &value) -> UniquenessLevel {
    for (const auto &[enum_key, enum_value] : UniquenessLevelValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid UniquenessLevels value: " + value);
  }

  std::map<MonikerKind, std::string> MonikerKindNames = {
    {MonikerKind::Import, "Import"},
    {MonikerKind::Export, "Export"},
    {MonikerKind::Local, "Local"},
  };

  auto monikerKindByName(const std::string &name) -> MonikerKind {
    for (const auto &[enum_key, enum_value] : MonikerKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid MonikerKind name: " + name);
  }

  std::map<MonikerKind, std::string> MonikerKindValues = {
    {MonikerKind::Import, "import"},
    {MonikerKind::Export, "export"},
    {MonikerKind::Local, "local"},
  };

  auto monikerKindByValue(const std::string &value) -> MonikerKind {
    for (const auto &[enum_key, enum_value] : MonikerKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid MonikerKind value: " + value);
  }

  std::map<CompletionTriggerKind, std::string> CompletionTriggerKindNames = {
    {CompletionTriggerKind::Invoked, "Invoked"},
    {CompletionTriggerKind::TriggerCharacter, "TriggerCharacter"},
    {CompletionTriggerKind::TriggerForIncompleteCompletions, "TriggerForIncompleteCompletions"},
  };

  auto completionTriggerKindByName(const std::string &name) -> CompletionTriggerKind {
    for (const auto &[enum_key, enum_value] : CompletionTriggerKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid CompletionTriggerKind name: " + name);
  }

  std::map<InsertTextFormat, std::string> InsertTextFormatNames = {
    {InsertTextFormat::PlainText, "PlainText"},
    {InsertTextFormat::Snippet, "Snippet"},
  };

  auto insertTextFormatByName(const std::string &name) -> InsertTextFormat {
    for (const auto &[enum_key, enum_value] : InsertTextFormatNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid InsertTextFormat name: " + name);
  }

  std::map<CompletionItemTag, std::string> CompletionItemTagNames = {
    {CompletionItemTag::Deprecated, "Deprecated"},
  };

  auto completionItemTagByName(const std::string &name) -> CompletionItemTag {
    for (const auto &[enum_key, enum_value] : CompletionItemTagNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid CompletionItemTag name: " + name);
  }

  std::map<InsertTextMode, std::string> InsertTextModeNames = {
    {InsertTextMode::asIs, "asIs"},
    {InsertTextMode::adjustIndentation, "adjustIndentation"},
  };

  auto insertTextModeByName(const std::string &name) -> InsertTextMode {
    for (const auto &[enum_key, enum_value] : InsertTextModeNames) {
      if (name == enum_value) {
        return enum_key;
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

  auto completionItemKindByName(const std::string &name) -> CompletionItemKind {
    for (const auto &[enum_key, enum_value] : CompletionItemKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid CompletionItemKind name: " + name);
  }

  std::map<DocumentDiagnosticReportKind, std::string> DocumentDiagnosticReportKindNames = {
    {DocumentDiagnosticReportKind::Full, "Full"},
    {DocumentDiagnosticReportKind::Unchanged, "Unchanged"},
  };

  auto documentDiagnosticReportKindByName(const std::string &name) -> DocumentDiagnosticReportKind {
    for (const auto &[enum_key, enum_value] : DocumentDiagnosticReportKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid DocumentDiagnosticReportKind name: " + name);
  }

  std::map<DocumentDiagnosticReportKind, std::string> DocumentDiagnosticReportKindValues = {
    {DocumentDiagnosticReportKind::Full, "full"},
    {DocumentDiagnosticReportKind::Unchanged, "unchanged"},
  };

  auto documentDiagnosticReportKindByValue(const std::string &value) -> DocumentDiagnosticReportKind {
    for (const auto &[enum_key, enum_value] : DocumentDiagnosticReportKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid DocumentDiagnosticReportKind value: " + value);
  }

  std::map<SignatureHelpTriggerKind, std::string> SignatureHelpTriggerKindNames = {
    {SignatureHelpTriggerKind::Invoked, "Invoked"},
    {SignatureHelpTriggerKind::TriggerCharacter, "TriggerCharacter"},
    {SignatureHelpTriggerKind::ContentChange, "ContentChange"},
  };

  auto signatureHelpTriggerKindByName(const std::string &name) -> SignatureHelpTriggerKind {
    for (const auto &[enum_key, enum_value] : SignatureHelpTriggerKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid SignatureHelpTriggerKind name: " + name);
  }

  std::map<CodeActionKind, std::string> CodeActionKindNames = {
    {CodeActionKind::Empty, ""},
    {CodeActionKind::QuickFix, "QuickFix"},
    {CodeActionKind::Refactor, "Refactor"},
    {CodeActionKind::RefactorExtract, "RefactorExtract"},
    {CodeActionKind::RefactorInline, "RefactorInline"},
    {CodeActionKind::RefactorRewrite, "RefactorRewrite"},
    {CodeActionKind::Source, "Source"},
    {CodeActionKind::SourceOrganizeImports, "SourceOrganizeImports"},
    {CodeActionKind::SourceFixAll, "SourceFixAll"},
  };

  auto codeActionKindByName(const std::string &name) -> CodeActionKind {
    for (const auto &[enum_key, enum_value] : CodeActionKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid CodeActionKind name: " + name);
  }

  std::map<CodeActionKind, std::string> CodeActionKindValues = {
    {CodeActionKind::Empty, ""},
    {CodeActionKind::QuickFix, "quickfix"},
    {CodeActionKind::Refactor, "refactor"},
    {CodeActionKind::RefactorExtract, "refactor.extract"},
    {CodeActionKind::RefactorInline, "refactor.inline"},
    {CodeActionKind::RefactorRewrite, "refactor.rewrite"},
    {CodeActionKind::Source, "source"},
    {CodeActionKind::SourceOrganizeImports, "source.organizeImports"},
    {CodeActionKind::SourceFixAll, "source.fixAll"},
  };

  auto codeActionKindByValue(const std::string &value) -> CodeActionKind {
    for (const auto &[enum_key, enum_value] : CodeActionKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid CodeActionKind value: " + value);
  }

  std::map<CodeActionTriggerKind, std::string> CodeActionTriggerKindNames = {
    {CodeActionTriggerKind::Invoked, "Invoked"},
    {CodeActionTriggerKind::Automatic, "Automatic"},
  };

  auto codeActionTriggerKindByName(const std::string &name) -> CodeActionTriggerKind {
    for (const auto &[enum_key, enum_value] : CodeActionTriggerKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid CodeActionTriggerKind name: " + name);
  }

  std::map<PrepareSupportDefaultBehavior, std::string> PrepareSupportDefaultBehaviorNames = {
    {PrepareSupportDefaultBehavior::Identifier, "Identifier"},
  };

  auto prepareSupportDefaultBehaviorByName(const std::string &name) -> PrepareSupportDefaultBehavior {
    for (const auto &[enum_key, enum_value] : PrepareSupportDefaultBehaviorNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid PrepareSupportDefaultBehavior name: " + name);
  }

  std::map<FileOperationPatternKind, std::string> FileOperationPatternKindNames = {
    {FileOperationPatternKind::File, "File"},
    {FileOperationPatternKind::Folder, "Folder"},
  };

  auto fileOperationPatternKindByName(const std::string &name) -> FileOperationPatternKind {
    for (const auto &[enum_key, enum_value] : FileOperationPatternKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid FileOperationPatternKind name: " + name);
  }

  std::map<FileOperationPatternKind, std::string> FileOperationPatternKindValues = {
    {FileOperationPatternKind::File, "file"},
    {FileOperationPatternKind::Folder, "folder"},
  };

  auto fileOperationPatternKindByValue(const std::string &value) -> FileOperationPatternKind {
    for (const auto &[enum_key, enum_value] : FileOperationPatternKindValues) {
      if (value == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid FileOperationPatternKind value: " + value);
  }

  std::map<WatchKind, std::string> WatchKindNames = {
    {WatchKind::Create, "Create"},
    {WatchKind::Change, "Change"},
    {WatchKind::Delete, "Delete"},
  };

  auto watchKindByName(const std::string &name) -> WatchKind {
    for (const auto &[enum_key, enum_value] : WatchKindNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid WatchKind name: " + name);
  }

  std::map<FileChangeType, std::string> FileChangeTypeNames = {
    {FileChangeType::Created, "Created"},
    {FileChangeType::Changed, "Changed"},
    {FileChangeType::Deleted, "Deleted"},
  };

  auto fileChangeTypeByName(const std::string &name) -> FileChangeType {
    for (const auto &[enum_key, enum_value] : FileChangeTypeNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid FileChangeType name: " + name);
  }

  std::map<MessageType, std::string> MessageTypeNames = {
    {MessageType::Error, "Error"},
    {MessageType::Warning, "Warning"},
    {MessageType::Info, "Info"},
    {MessageType::Log, "Log"},
    {MessageType::Debug, "Debug"},
  };

  auto messageTypeByName(const std::string &name) -> MessageType {
    for (const auto &[enum_key, enum_value] : MessageTypeNames) {
      if (name == enum_value) {
        return enum_key;
      }
    }
    throw std::invalid_argument("Invalid MessageType name: " + name);
  }

} // namespace LCompilers::LanguageServiceProtocol
