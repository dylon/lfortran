#ifndef LCOMPILERS_LSP_SPECIFICATION_H
#define LCOMPILERS_LSP_SPECIFICATION_H

#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

/**
 * Interface definitions from the LSP 3.17 specification.
 * See: https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification
 */
namespace LCompilers::LanguageServiceProvider {

  /**
   * Defines an integer number in the range of -2^31 to 2^31 - 1.
   *
   * export type integer = number;
   */
  typedef int integer;

  /**
   * Defines an unsigned integer number in the range of 0 to 2^31 - 1.
   *
   * export type uinteger = number;
   */
  typedef unsigned int uinteger;

  /**
   * Defines a decimal number. Since decimal numbers are very rare in the
   * language server specification we denote the exact range with every decimal
   * using the mathematics interval notation (e.g. [0, 1] denotes all decimals d
   * with 0 <= d <= 1.
   *
   * export type decimal = number;
   */
  typedef double decimal;

  typedef bool boolean;

  typedef std::nullptr_t null;

  typedef std::string string;

  struct LSPAny;  // Forward declaration

  /**
   * LSP object definition.
   *
   * export type LSPObject = { [key: string]: LSPAny };
   *
   * @since 3.17.0
   */
  typedef std::map<std::string, std::unique_ptr<LSPAny>> LSPObject;

  /**
   * LSP arrays.
   *
   * export type LSPArray = LSPAny[];
   *
   * @since 3.17.0
   */
  typedef std::vector<std::unique_ptr<LSPAny>> LSPArray;

  enum class LSPAnyType {
    LSP_OBJECT,
    LSP_ARRAY,
    LSP_STRING,
    LSP_INTEGER,
    LSP_UINTEGER,
    LSP_DECIMAL,
    LSP_BOOLEAN,
    LSP_NULL,
  };

  /**
   * The LSP any type.
   *
   * export type LSPAny = LSPObject
   *   | LSPArray
   *   | string
   *   | integer
   *   | uinteger
   *   | decimal
   *   | boolean
   *   | null;
   *
   * @since 3.17.0
   */
  struct LSPAny {
    LSPAnyType type;
    union {
      std::unique_ptr<LSPObject> objectValue;
      std::unique_ptr<LSPArray> arrayValue;
      string stringValue;
      integer integerValue;
      uinteger uintegerValue;
      decimal decimalValue;
      null nullValue;
    };
  };

  /**
   * A general message as defined by JSON-RPC. The language server protocol
   * always uses “2.0” as the jsonrpc version.
   *
   * interface Message {
   *   jsonrpc: string;
   * }
   */
  struct Message {
    string jsonrpc;
  };

  enum class RequestIdType {
    LSP_INTEGER,
    LSP_STRING,
  };

  struct RequestId {
    RequestIdType type;
    union {
      integer integervalue;
      string stringvalue;
    };
  };

  enum class RequestParamsType {
    LSP_ARRAY,
    LSP_OBJECT,
  };

  struct RequestParams {
    RequestParamsType type;
    union {
      std::unique_ptr<std::vector<std::unique_ptr<void>>> arrayValue;
      std::unique_ptr<void> objectValue;
    };
  };

  /**
   * A request message to describe a request between the client and the server.
   * Every processed request must send a response back to the sender of the
   * request.
   *
   * interface RequestMessage extends Message {
   *   id: integer | string;
   *   method: string;
   *   params?: array | object;
   * }
   */
  struct RequestMessage : public Message {

    /**
     * The request id.
     *
     * id: integer | string;
     */
    std::unique_ptr<RequestId> id;

    /**
    * The method to be invoked.
     *
     * method: string;
    */
    string method;

    /**
     * The method's params.
     *
     * params?: array | object;
     */
    std::optional<std::unique_ptr<RequestParams>> params;
  };

  /**
   * export namespace ErrorCodes {
   *   export const ParseError: integer = -32700;
   *   export const InvalidRequest: integer = -32600;
   *   export const MethodNotFound: integer = -32601;
   *   export const InvalidParams: integer = -32602;
   *   export const InternalError: integer = -32603;
   *   export const jsonrpcReservedErrorRangeStart: integer = -32099;
   *   export const serverErrorStart: integer = jsonrpcReservedErrorRangeStart;
   *   export const ServerNotInitialized: integer = -32002;
   *   export const UnknownErrorCode: integer = -32001;
   *   export const jsonrpcReservedErrorRangeEnd = -32000;
   *   export const serverErrorEnd: integer = jsonrpcReservedErrorRangeEnd;
   *   export const lspReservedErrorRangeStart: integer = -32899;
   *   export const RequestFailed: integer = -32803;
   *   export const ServerCancelled: integer = -32802;
   *   export const ContentModified: integer = -32801;
   *   export const RequestCancelled: integer = -32800;
   *   export const lspReservedErrorRangeEnd: integer = -32800;
   * }
   */
  enum class ErrorCodes {

    /**
     * Defined by JSON-RPC.
     *
     * export const ParseError: integer = -32700;
     */
    ParseError = -32700,

    /**
     * Defined by JSON-RPC.
     *
     * export const InvalidRequest: integer = -32600;
     */
    InvalidRequest = -32600,

    /**
     * Defined by JSON-RPC.
     *
     * export const MethodNotFound: integer = -32601;
     */
    MethodNotFound = -32601,

    /**
     * Defined by JSON-RPC.
     *
     * export const InvalidParams: integer = -32602;
     */
    InvalidParams = -32602,

    /**
     * Defined by JSON-RPC.
     *
     * export const InternalError: integer = -32603;
     */
    InternalError = -32603,

    /**
     * This is the start range of JSON-RPC reserved error codes.
     * It doesn't denote a real error code. No LSP error codes should
     * be defined between the start and end range. For backwards
     * compatibility the `ServerNotInitialized` and the `UnknownErrorCode`
     * are left in the range.
     *
     * export const jsonrpcReservedErrorRangeStart: integer = -32099;
     *
     * @since 3.16.0
     */
    jsonrpcReservedErrorRangeStart = -32099,

    /**
     * @deprecated use jsonrpcReservedErrorRangeStart
     *
     * export const serverErrorStart: integer = jsonrpcReservedErrorRangeStart;
     */
    serverErrorStart = jsonrpcReservedErrorRangeStart,

    /**
     * Error code indicating that a server received a notification or
     * request before the server has received the `initialize` request.
     *
     * export const ServerNotInitialized: integer = -32002;
     */
    ServerNotInitialized = -32002,

    /**
     * export const UnknownErrorCode: integer = -32001;
     */
    UnknownErrorCode = -32001,

    /**
     * This is the end range of JSON-RPC reserved error codes.
     * It doesn't denote a real error code.
     *
     * export const jsonrpcReservedErrorRangeEnd = -32000;
     *
     * @since 3.16.0
     */
    jsonrpcReservedErrorRangeEnd = -32000,

    /**
     * @deprecated use jsonrpcReservedErrorRangeEnd
     *
     * export const serverErrorEnd: integer = jsonrpcReservedErrorRangeEnd;
     */
    serverErrorEnd = jsonrpcReservedErrorRangeEnd,

    /**
     * This is the start range of LSP reserved error codes.
     * It doesn't denote a real error code.
     *
     * @since 3.16.0
     *
     * export const lspReservedErrorRangeStart: integer = -32899;
     */
    lspReservedErrorRangeStart = -32899,

    /**
     * A request failed but it was syntactically correct, e.g the
     * method name was known and the parameters were valid. The error
     * message should contain human readable information about why
     * the request failed.
     *
     * export const RequestFailed: integer = -32803;
     *
     * @since 3.17.0
     */
    RequestFailed = -32803,

    /**
     * The server cancelled the request. This error code should
     * only be used for requests that explicitly support being
     * server cancellable.
     *
     * export const ServerCancelled: integer = -32802;
     *
     * @since 3.17.0
     */
    ServerCancelled = -32802,

    /**
     * The server detected that the content of a document got
     * modified outside normal conditions. A server should
     * NOT send this error code if it detects a content change
     * in it unprocessed messages. The result even computed
     * on an older state might still be useful for the client.
     *
     * If a client decides that a result is not of any use anymore
     * the client should cancel the request.
     *
     * export const ContentModified: integer = -32801;
     */
    ContentModified = -32801,

    /**
     * The client has canceled a request and a server has detected
     * the cancel.
     *
     * export const RequestCancelled: integer = -32800;
     */
    RequestCancelled = -32800,

    /**
     * This is the end range of LSP reserved error codes.
     * It doesn't denote a real error code.
     *
     * export const lspReservedErrorRangeEnd: integer = -32800;
     *
     * @since 3.16.0
     */
    lspReservedErrorRangeEnd = -32800,
  };

  /**
   * interface ResponseError {
   *   code: integer;
   *   message: string;
   *   data?: LSPAny;
   * }
   */
  struct ResponseError {

    /**
     * A number indicating the error type that occurred.
     *
     * code: integer;
     */
    integer code;

    /**
     * A string providing a short description of the error.
     *
     * message: string;
     */
    string message;

    /**
     * A primitive or structured value that contains additional
     * information about the error. Can be omitted.
     *
     * data?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  enum class ResponseIdType {
    LSP_INTEGER,
    LSP_STRING,
    LSP_NULL,
  };

  struct ResponseId {
    ResponseIdType type;
    union {
      integer integerValue;
      string stringValue;
      null nullValue;
    };
  };

  /**
   * A Response Message sent as a result of a request. If a request doesn’t
   * provide a result value the receiver of a request still needs to return a
   * response message to conform to the JSON-RPC specification. The result
   * property of the ResponseMessage should be set to null in this case to
   * signal a successful request.
   */
  struct ResponseMessage : public Message {

    /**
     * The request id.
     *
     * id: integer | string | null;
     */
    std::unique_ptr<ResponseId> id;

    /**
     * The result of a request. This member is REQUIRED on success. This member
     * MUST NOT exist if there was an error invoking the method.
     *
     * result?: LSPAny;
     */
    std::optional<std::unique_ptr<LSPAny>> result;

    /**
     * The error object in case a request fails.
     *
     * error?: ResponseError;
     */
    std::optional<std::unique_ptr<ResponseError>> error;
  };

  enum class NotificationParamsType {
    LSP_ARRAY,
    LSP_OBJECT,
  };

  struct NotificationParams {
    NotificationParamsType type;
    union {
      std::unique_ptr<std::vector<std::unique_ptr<void>>> arrayValue;
      std::unique_ptr<void> objectValue;
    };
  };

  /**
   * A notification message. A processed notification message must not send a
   * response back. They work like events.
   *
   * interface NotificationMessage extends Message {
   *   method: string;
   *   params?: array | object;
   * }
   */
  struct NotificationMessage : public Message {

    /**
     * The method to be invoked.
     *
     * method: string;
     */
    std::string method;

    /**
     * The notification's params.
     *
     * params?: array | object;
     */
    std::optional<std::unique_ptr<NotificationParams>> params;
  };

  // ---------------------------------------------------------------------------
  // $ Notifications and Requests
  // ---------------------------------------------------------------------------
  // Notification and requests whose methods start with ‘$/’ are messages which
  // are protocol implementation dependent and might not be implementable in all
  // clients or servers. For example if the server implementation uses a single
  // threaded synchronous programming language then there is little a server can
  // do to react to a $/cancelRequest notification. If a server or client
  // receives notifications starting with ‘$/’ it is free to ignore the
  // notification. If a server or client receives a request starting with ‘$/’
  // it must error the request with error code MethodNotFound (e.g. -32601).
  // ---------------------------------------------------------------------------

  /**
   * The base protocol offers support for request cancellation. To cancel a request, a notification message with the following properties is sent:
   *
   *  Notification:
   *    method: ‘$/cancelRequest’
   *    params: CancelParams defined as follows:
   *
   * A request that got canceled still needs to return from the server and send
   * a response back. It can not be left open / hanging. This is in line with
   * the JSON-RPC protocol that requires that every request sends a response
   * back. In addition it allows for returning partial results on cancel. If the
   * request returns an error response on cancellation it is advised to set the
   * error code to ErrorCodes.RequestCancelled.
   *
   *  interface CancelParams {
   *    id: integer | string;
   *  }
   */
  struct CancelParams {

    /**
     * The request id to cancel.
     *
     * id: integer | string;
     */
    std::unique_ptr<RequestId> id;
  };

  enum class ProgressTokenType {
    LSP_INTEGER,
    LSP_STRING,
  };

  struct ProgressToken {
    ProgressTokenType type;
    union {
      integer integerValue;
      string stringValue;
    };
  };

  /**
   * The base protocol offers also support to report progress in a generic
   * fashion. This mechanism can be used to report any kind of progress
   * including work done progress (usually used to report progress in the user
   * interface using a progress bar) and partial result progress to support
   * streaming of results.
   *
   * A progress notification has the following properties:
   *
   * Notification:
   *   method: ‘$/progress’
   *   params: ProgressParams defined as follows:
   *
   * Progress is reported against a token. The token is different than the
   * request ID which allows to report progress out of band and also for
   * notification.
   *
   * interface ProgressParams<T> {
   *   token: ProgressToken;
   *   value: T;
   * }
   */
  struct ProgressParams {

    /**
     * The progress token provided by the client or server.
     *
     * token: ProgressToken;
     */
    std::unique_ptr<ProgressToken> token;

    /**
     * The progress data.
     *
     * value: T;
     */
    std::unique_ptr<void> value;
  };

  /**
   * Position in a text document expressed as zero-based line and zero-based
   * character offset. A position is between two characters like an ‘insert’
   * cursor in an editor. Special values like for example -1 to denote the end
   * of a line are not supported.
   *
   * interface Position {
   *   line: uinteger;
   *   character: uinteger;
   * }
   */
  struct Position {

    /**
     * Line position in a document (zero-based).
     *
     * line: uinteger;
     */
    uinteger line;

    /**
     * Character offset on a line in a document (zero-based). The meaning of this
     * offset is determined by the negotiated `PositionEncodingKind`.
     *
     * If the character value is greater than the line length it defaults back
     * to the line length.
     *
     * character: uinteger;
     */
    uinteger character;
  };

  /**
   * A type indicating how positions are encoded, specifically what column
   * offsets mean.
   *
   * export type PositionEncodingKind = string;
   *
   * @since 3.17.0
   */
  enum class PositionEncodingKind {

    /**
     * Character offsets count UTF-8 code units (e.g bytes).
     *
     * export const UTF8: PositionEncodingKind = 'utf-8';
     */
    UTF8,

    /**
     * Character offsets count UTF-16 code units.
     *
     * This is the default and must always be supported
     * by servers
     *
     * export const UTF16: PositionEncodingKind = 'utf-16';
     */
    UTF16,

    /**
     * Character offsets count UTF-32 code units.
     *
     * Implementation note: these are the same as Unicode code points,
     * so this `PositionEncodingKind` may also be used for an
     * encoding-agnostic representation of character offsets.
     *
     * export const UTF32: PositionEncodingKind = 'utf-32';
     */
    UTF32,
  };

  extern std::map<PositionEncodingKind, std::string> PositionEncodingKindNames;

  auto positionEncodingKind(const std::string &name) -> PositionEncodingKind;

  /**
   * A range in a text document expressed as (zero-based) start and end
   * positions. A range is comparable to a selection in an editor. Therefore,
   * the end position is exclusive. If you want to specify a range that contains
   * a line including the line ending character(s) then use an end position
   * denoting the start of the next line.
   *
   * interface Range {
   *   start: Position;
   *   end: Position;
   * }
   */
  struct Range {

    /**
     * The range's start position.
     *
     * start: Position;
     */
    std::unique_ptr<Position> start;

    /**
     * The range's end position.
     *
     * end: Position;
     */
    std::unique_ptr<Position> end;
  };

  /**
   * A URI specifying the path to a document. This should begin with "file://"
   * but might begin with "file:" or the protocol (e.g. "file://") may be
   * excluded. By specification, it should begin with the "file://" protocol.
   *
   * type DocumentUri = string;
   */
  typedef string DocumentUri;

  /**
   * A tagging interface for normal non document URIs.
   *
   * type URI = string;
   */
  typedef string URI;

  /**
   * An item to transfer a text document from the client to the server.
   *
   * Text documents have a language identifier to identify a document on the
   * server side when it handles more than one language to avoid re-interpreting
   * the file extension. If a document refers to one of the programming
   * languages listed below it is recommended that clients use those ids.
   *
   * --------------------+-----------------------------------
   * Language            | Identifier
   * --------------------+-----------------------------------
   * ABAP                | abap
   * Windows Bat         | bat
   * BibTeX              | bibtex
   * Clojure             | clojure
   * Coffeescript        | coffeescript
   * C                   | c
   * C++                 | cpp
   * C#                  | csharp
   * CSS                 | css
   * Diff                | diff
   * Dart                | dart
   * Dockerfile          | dockerfile
   * Elixir              | elixir
   * Erlang              | erlang
   * F#                  | fsharp
   * Git                 | git-commit
   * Git                 | git-rebase
   * Go                  | go
   * Groovy              | groovy
   * Handlebars          | handlebars
   * HTML                | html
   * Ini                 | ini
   * Java                | java
   * JavaScript          | javascript
   * JavaScript React    | javascriptreact
   * JSON                | json
   * LaTeX               | latex
   * Less                | less
   * Lua                 | lua
   * Makefile            | makefile
   * Markdown            | markdown
   * Objective-C         | objective-c
   * Objective-C++       | objective-cpp
   * Perl                | perl
   * Perl 6              | perl6
   * PHP                 | php
   * Powershell          | powershell
   * Pug                 | jade
   * Python              | python
   * R                   | r
   * Razor (cshtml)      | razor
   * Ruby                | ruby
   * Rust                | rust
   * SCSS                | scss (syntax using curly brackets)
   * SCSS                | sass (indented syntax)
   * Scala               | scala
   * ShaderLab           | shaderlab
   * Shell Script (Bash) | shellscript
   * SQL                 | sql
   * Swift               | swift
   * TypeScript          | typescript
   * TypeScript React    | typescriptreact
   * TeX                 | tex
   * Visual Basic        | vb
   * XML                 | xml
   * XSL                 | xsl
   * YAML                | yaml
   * --------------------+-----------------------------------
   *
   * interface TextDocumentItem {
   *   uri: DocumentUri;
   *   languageId: string;
   *   version: integer;
   *   text: string;
   * }
   */
  struct TextDocumentItem {

    /**
     * The text document's URI.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * The text document's language identifier.
     *
     * languageId: string;
     */
    string languageId;

    /**
     * The version number of this document (it will increase after each change,
     * including undo/redo).
     *
     * version: integer;
     */
    integer version;

    /**
     * The content of the opened text document.
     *
     * text: string;
     */
    string text;
  };

  /**
   * Text documents are identified using a URI. On the protocol level, URIs are
   * passed as strings.
   *
   * interface TextDocumentIdentifier {
   *   uri: DocumentUri;
   * }
   */
  struct TextDocumentIdentifier {

    /**
     * The text document's URI.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;
  };

  /**
   * An identifier to denote a specific version of a text document. This
   * information usually flows from the client to the server.
   *
   * interface VersionedTextDocumentIdentifier extends TextDocumentIdentifier {
   *   version: integer;
   * }
   */
  struct VersionedTextDocumentIdentifier : public TextDocumentIdentifier {

    /**
     * The version number of this document.
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     *
     * version: integer;
     */
    integer version;
  };

  /**
   * An identifier which optionally denotes a specific version of a text
   * document. This information usually flows from the server to the client.
   *
   * interface OptionalVersionedTextDocumentIdentifier extends TextDocumentIdentifier {
   *   version: integer | null;
   * }
   */
  struct OptionalVersionedTextDocumentIdentifier : public TextDocumentIdentifier {

    /**
     * The version number of this document. If an optional versioned text document
     * identifier is sent from the server to the client and the file is not
     * open in the editor (the server has not received an open notification
     * before) the server can send `null` to indicate that the version is
     * known and the content on disk is the master (as specified with document
     * content ownership).
     *
     * The version number of a document will increase after each change,
     * including undo/redo. The number doesn't need to be consecutive.
     *
     * version: integer | null;
     */
    std::optional<integer> version;
  };

  /**
   * Was TextDocumentPosition in 1.0 with inlined parameters.
   *
   * A parameter literal used in requests to pass a text document and a position
   * inside that document. It is up to the client to decide how a selection is
   * converted into a position when issuing a request for a text document. The
   * client can for example honor or ignore the selection direction to make LSP
   * request consistent with features implemented internally.
   *
   * interface TextDocumentPositionParams {
   *   textDocument: TextDocumentIdentifier;
   *   position: Position;
   * }
   */
  struct TextDocumentPositionParams {

    /**
     * The text document.
     *
     * textDocument: TextDocumentIdentifier;
     */
    std::unique_ptr<TextDocumentIdentifier> textDocument;

    /**
     * The position inside the text document.
     *
     * position: Position;
     */
    std::unique_ptr<Position> position;
  };

  /**
   * A document filter denotes a document through properties like language,
   * scheme or pattern. An example is a filter that applies to TypeScript files
   * on disk. Another example is a filter that applies to JSON files with name
   * package.json.
   *
   * Please note that for a document filter to be valid at least one of the
   * properties for language, scheme, or pattern must be set. To keep the type
   * definition simple all properties are marked as optional.
   *
   * export interface DocumentFilter {
   *   language?: string;
   *   scheme?: string;
   *   pattern?: string;
   * }
   */
  struct DocumentFilter {

    /**
     * A language id, like `typescript`.
     *
     * language?: string;
     */
    std::optional<string> language;

    /**
     * A Uri scheme, like `file` or `untitled`.
     *
     * scheme?: string;
     */
    std::optional<string> scheme;

    /**
     * A glob pattern, like `*.{ts,js}`.
     *
     * Glob patterns can have the following syntax:
     * - `*` to match one or more characters in a path segment.
     * - `?` to match on one character in a path segment.
     * - `**` to match any number of path segments, including none.
     * - `{}` to group sub patterns into an OR expression.
     * - `[]` to declare a range of characters to match in a path segment.
     * - `[!...]` to negate a range of characters to match in a path segment.
     *
     * pattern?: string;
     */
    std::optional<string> pattern;
  };

  /**
   * A document selector is the combination of one or more document filters.
   *
   * export type DocumentSelector = DocumentFilter[];
   */
  typedef std::vector<std::unique_ptr<DocumentFilter>> DocumentSelector;

  /**
   * A textual edit applicable to a text document.
   *
   * interface TextEdit {
   *   range: Range;
   *   newText: string;
   * }
   */
  struct TextEdit {

    /**
     * The range of the text document to be manipulated. To insert
     * text into a document create a range where start === end.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The string to be inserted. For delete operations use an
     * empty string.
     *
     * newText: string;
     */
    string newText;
  };

  /**
   * Since 3.16.0 there is also the concept of an annotated text edit which
   * supports to add an annotation to a text edit. The annotation can add
   * information describing the change to the text edit.
   *
   * Usually clients provide options to group the changes along the annotations
   * they are associated with. To support this in the protocol an edit or
   * resource operation refers to a change annotation using an identifier and
   * not the change annotation literal directly. This allows servers to use the
   * identical annotation across multiple edits or resource operations which
   * then allows clients to group the operations under that change annotation.
   * The actual change annotations together with their identifiers are managed
   * by the workspace edit via the new property changeAnnotations.
   *
   * export interface ChangeAnnotation {
   *   label: string;
   *   needsConfirmation?: boolean;
   *   description?: string;
   * }
   */
  struct ChangeAnnotation {

    /**
     * A human-readable string describing the actual change. The string
     * is rendered prominent in the user interface.
     *
     * label: string;
     */
    string label;

    /**
     * A flag which indicates that user confirmation is needed
     * before applying the change.
     *
     * needsConfirmation?: boolean;
     */
    std::optional<boolean> needsConfirmation;

    /**
     * A human-readable string which is rendered less prominent in
     * the user interface.
     *
     * description?: string;
     */
    std::optional<string> description;
  };

  /**
   * An identifier referring to a change annotation managed by a workspace
   * edit.
   *
   * export type ChangeAnnotationIdentifier = string;
   *
   * @since 3.16.0.
   */
  typedef string ChangeAnnotationIdentifier;

  /**
   * A special text edit with an additional change annotation.
   *
   * export interface AnnotatedTextEdit extends TextEdit {
   *   annotationId: ChangeAnnotationIdentifier;
   * }
   *
   * @since 3.16.0.
   */
  struct AnnotatedTextEdit : public TextEdit {

    /**
     * The actual annotation identifier.
     *
     * annotationId: ChangeAnnotationIdentifier;
     */
    ChangeAnnotationIdentifier annotationId;
  };

  enum class OptionalAnnotatedTextEditType {
    NOT_ANNOTATED,
    ANNOTATED,
  };

  struct OptionalAnnotatedTextEdit {
    OptionalAnnotatedTextEditType type;
    union {
      std::unique_ptr<TextEdit> textEdit;
      std::unique_ptr<AnnotatedTextEdit> annotatedTextEdit;
    };
  };

  /**
   * New in version 3.16: support for AnnotatedTextEdit. The support is guarded
   * by the client capability workspace.workspaceEdit.changeAnnotationSupport.
   * If a client doesn’t signal the capability, servers shouldn’t send
   * AnnotatedTextEdit literals back to the client.
   *
   * Describes textual changes on a single text document. The text document is
   * referred to as a OptionalVersionedTextDocumentIdentifier to allow clients
   * to check the text document version before an edit is applied. A
   * TextDocumentEdit describes all changes on a version Si and after they are
   * applied move the document to version Si+1. So the creator of a
   * TextDocumentEdit doesn’t need to sort the array of edits or do any kind of
   * ordering. However the edits must be non overlapping.
   *
   * export interface TextDocumentEdit {
   *   textDocument: OptionalVersionedTextDocumentIdentifier;
   *   edits: (TextEdit | AnnotatedTextEdit)[];
   * }
   */
  struct TextDocumentEdit {

    /**
     * The text document to change.
     *
     * textDocument: OptionalVersionedTextDocumentIdentifier;
     */
    std::unique_ptr<OptionalVersionedTextDocumentIdentifier> textDocument;

    /**
     * The edits to be applied.
     *
     * @since 3.16.0 - support for AnnotatedTextEdit. This is guarded by the
     * client capability `workspace.workspaceEdit.changeAnnotationSupport`
     *
     * edits: (TextEdit | AnnotatedTextEdit)[];
     */
    std::unique_ptr<std::vector<std::unique_ptr<OptionalAnnotatedTextEdit>>> edits;
  };

  /**
   * Represents a location inside a resource, such as a line inside a text file.
   *
   * interface Location {
   *   uri: DocumentUri;
   *   range: Range;
   * }
   */
  struct Location {

    DocumentUri uri;

    Range range;
  };

  /**
   * Represents a link between a source and a target location.
   *
   * interface LocationLink {
   *   originSelectionRange?: Range;
   *   targetUri: DocumentUri;
   *   targetRange: Range;
   *   targetSelectionRange: Range;
   * }
   */
  struct LocationLink {

    /**
     * Span of the origin of this link.
     *
     * Used as the underlined span for mouse interaction. Defaults to the word
     * range at the mouse position.
     *
     * originSelectionRange?: Range;
     */
    std::optional<std::unique_ptr<Range>> originSelectionRange;

    /**
     * The target resource identifier of this link.
     *
     * targetUri: DocumentUri;
     */
    DocumentUri targetUri;

    /**
     * The full target range of this link. If the target for example is a symbol
     * then target range is the range enclosing this symbol not including
     * leading/trailing whitespace but everything else like comments. This
     * information is typically used to highlight the range in the editor.
     *
     * targetRange: Range;
     */
    std::unique_ptr<Range> targetRange;

    /**
     * The range that should be selected and revealed when this link is being
     * followed, e.g the name of a function. Must be contained by the
     * `targetRange`. See also `DocumentSymbol#range`
     *
     * targetSelectionRange: Range;
     */
    std::unique_ptr<Range> targetSelectionRange;
  };

  /**
   * The protocol currently supports the following diagnostic severities and
   * tags.
   *
   * export namespace DiagnosticSeverity {
   *   export const Error: 1 = 1;
   *   export const Warning: 2 = 2;
   *   export const Information: 3 = 3;
   *   export const Hint: 4 = 4;
   * }
   *
   * export type DiagnosticSeverity = 1 | 2 | 3 | 4;
   */
  enum class DiagnosticSeverity {

    /**
     * Reports an error.
     *
     * export const Error: 1 = 1;
     */
    Error = 1,

    /**
     * Reports a warning.
     *
     * export const Warning: 2 = 2;
     */
    Warning = 2,

    /**
     * Reports an information.
     *
     * export const Information: 3 = 3;
     */
    Information = 3,

    /**
     * Reports a hint.
     *
     * export const Hint: 4 = 4;
     */
    Hint = 4,
  };

  extern std::map<DiagnosticSeverity, std::string> DiagnosticSeverityNames;

  auto diagnosticSeverity(const std::string &name) -> DiagnosticSeverity;

  /**
   * The diagnostic tags.
   *
   * export namespace DiagnosticTag {
   *   export const Unnecessary: 1 = 1;
   *   export const Deprecated: 2 = 2;
   * }
   *
   * export type DiagnosticTag = 1 | 2;
   *
   * @since 3.15.0
   */
  enum class DiagnosticTag {

    /**
     * Unused or unnecessary code.
     *
     * Clients are allowed to render diagnostics with this tag faded out
     * instead of having an error squiggle.
     *
     * export const Unnecessary: 1 = 1;
     */
    Unnecessary = 1,

    /**
     * Deprecated or obsolete code.
     *
     * Clients are allowed to rendered diagnostics with this tag strike through.
     *
     * export const Deprecated: 2 = 2;
     */
    Deprecated = 2,
  };

  extern std::map<DiagnosticTag, std::string> DiagnosticTagNames;

  auto diagnosticTag(const std::string &name) -> DiagnosticTag;

  /**
   * Represents a related message and source code location for a diagnostic.
   * This should be used to point to code locations that cause or are related to
   * a diagnostics, e.g when duplicating a symbol in a scope.
   *
   * export interface DiagnosticRelatedInformation {
   *   location: Location;
   *   message: string;
   * }
   */
  struct DiagnosticRelatedInformation {

    /**
     * The location of this related diagnostic information.
     *
     * location: Location;
     */
    std::unique_ptr<Location> location;

    /**
     * The message of this related diagnostic information.
     *
     * message: string;
     */
    string message;
  };

  /**
   * Structure to capture a description for an error code.
   *
   * export interface CodeDescription {
   *   href: URI;
   * }
   *
   * @since 3.16.0
   */
  struct CodeDescription {

    /**
     * An URI to open with more information about the diagnostic error.
     *
     * href: URI;
     */
    URI href;
  };

  enum class DiagnosticCodeType {
    LSP_INTEGER,
    LSP_STRING,
  };

  struct DiagnosticCode {
    DiagnosticCodeType type;
    union {
      integer integerValue;
      string stringValue;
    };
  };

  /**
   * Represents a diagnostic, such as a compiler error or warning. Diagnostic
   * objects are only valid in the scope of a resource.
   *
   * export interface Diagnostic {
   *   range: Range;
   *   severity?: DiagnosticSeverity;
   *   code?: integer | string;
   *   codeDescription?: CodeDescription;
   *   source?: string;
   *   message: string;
   *   tags?: DiagnosticTag[];
   *   relatedInformation?: DiagnosticRelatedInformation[];
   *   data?: LSPAny;
   * }
   */
  struct Diagnostic {

    /**
     * The range at which the message applies.
     *
     * range: Range;
     */
    std::unique_ptr<Range> range;

    /**
     * The diagnostic's severity. To avoid interpretation mismatches when a
     * server is used with different clients it is highly recommended that
     * servers always provide a severity value. If omitted, it’s recommended for
     * the client to interpret it as an Error severity.
     *
     * severity?: DiagnosticSeverity;
     */
    std::optional<DiagnosticSeverity> severity;

    /**
     * The diagnostic's code, which might appear in the user interface.
     *
     * code?: integer | string;
     */
    std::optional<std::unique_ptr<DiagnosticCode>> code;

    /**
     * An optional property to describe the error code.
     *
     * codeDescription?: CodeDescription;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<CodeDescription>> codeDescription;

    /**
     * A human-readable string describing the source of this
     * diagnostic, e.g. 'typescript' or 'super lint'.
     *
     * source?: string;
     */
    std::optional<string> source;

    /**
     * The diagnostic's message.
     *
     * message: string;
     */
    string message;

    /**
     * Additional metadata about the diagnostic.
     *
     * tags?: DiagnosticTag[];
     *
     * @since 3.15.0
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<DiagnosticTag>>>> tags;

    /**
     * An array of related diagnostic information, e.g. when symbol-names within
     * a scope collide all definitions can be marked via this property.
     *
     * relatedInformation?: DiagnosticRelatedInformation[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<DiagnosticRelatedInformation>>>> relatedInformation;

    /**
     * A data entry field that is preserved between a
     * `textDocument/publishDiagnostics` notification and
     * `textDocument/codeAction` request.
     *
     * data?: LSPAny;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<LSPAny>> data;
  };

  /**
   * Represents a reference to a command. Provides a title which will be used to
   * represent a command in the UI. Commands are identified by a string
   * identifier. The recommended way to handle commands is to implement their
   * execution on the server side if the client and server provides the
   * corresponding capabilities. Alternatively the tool extension code could
   * handle the command. The protocol currently doesn’t specify a set of
   * well-known commands.
   *
   * interface Command {
   *   title: string;
   *   command: string;
   *   arguments?: LSPAny[];
   * }
   */
  struct Command {

    /**
     * Title of the command, like `save`.
     *
     * title: string;
     */
    string title;

    /**
     * The identifier of the actual command handler.
     *
     * command: string;
     */
    string command;

    /**
     * Arguments that the command handler should be invoked with.
     *
     * arguments?: LSPAny[];
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<LSPAny>>>> arguments;
  };

  /**
   * Describes the content type that a client supports in various result
   * literals like `Hover`, `ParameterInfo` or `CompletionItem`.
   *
   * Please note that `MarkupKinds` must not start with a `$`. This kinds are
   * reserved for internal usage.
   *
   * export namespace MarkupKind {
   *   export const PlainText: 'plaintext' = 'plaintext';
   *   export const Markdown: 'markdown' = 'markdown';
   * }
   * export type MarkupKind = 'plaintext' | 'markdown';
   */
  enum class MarkupKind {

    /**
     * Plain text is supported as a content format
     *
     * export const PlainText: 'plaintext' = 'plaintext';
     */
    PlainText,

    /**
     * Markdown is supported as a content format
     *
     * export const Markdown: 'markdown' = 'markdown';
     */
    Markdown,
  };

  extern std::map<MarkupKind, std::string> MarkupKindNames;

  auto markupKind(const std::string &name) -> MarkupKind;

  /**
   * A `MarkupContent` literal represents a string value which content is
   * interpreted base on its kind flag. Currently the protocol supports
   * `plaintext` and `markdown` as markup kinds.
   *
   * If the kind is `markdown` then the value can contain fenced code blocks
   * like in GitHub issues.
   *
   * *Please Note* that clients might sanitize the return markdown. A client
   * could decide to remove HTML from the markdown to avoid script execution.
   *
   * export interface MarkupContent {
   *   kind: MarkupKind;
   *   value: string;
   * }
   */
  struct MarkupContent {

    /**
     * The type of the Markup
     *
     * kind: MarkupKind;
     */
    MarkupKind kind;

    /**
     * The content itself
     *
     * value: string;
     */
    string value;
  };

  /**
   * Client capabilities specific to the used markdown parser.
   *
   * export interface MarkdownClientCapabilities {
   *   parser: string;
   *   version?: string;
   *   allowedTags?: string[];
   * }
   *
   * @since 3.16.0
   */
  struct MarkdownClientCapabilities {

    /**
     * The name of the parser.
     *
     * parser: string;
     */
    string parser;

    /**
     * The version of the parser.
     *
     * version?: string;
     */
    std::optional<string> version;

    /**
     * A list of HTML tags that the client allows / supports in
     * Markdown.
     *
     * allowedTags?: string[];
     *
     * @since 3.17.0
     */
    std::optional<std::vector<std::string>> allowedTags;
  };

  // --------------------------------------------------------------------------
  // New in version 3.13. Since version 3.16 file resource changes can carry an
  // additional property changeAnnotation to describe the actual change in more
  // detail. Whether a client has support for change annotations is guarded by
  // the client capability workspace.workspaceEdit.changeAnnotationSupport.
  //
  // File resource changes allow servers to create, rename and delete files and
  // folders via the client. Note that the names talk about files but the
  // operations are supposed to work on files and folders. This is in line with
  // other naming in the Language Server Protocol (see file watchers which can
  // watch files and folders).
  // --------------------------------------------------------------------------

  /**
   * Options to create a file.
   *
   * export interface CreateFileOptions {
   *   overwrite?: boolean;
   *   ignoreIfExists?: boolean;
   * }
   */
  struct CreateFileOptions {

    /**
     * Overwrite existing file. Overwrite wins over `ignoreIfExists`
     *
     * overwrite?: boolean;
     */
    std::optional<boolean> overwrite;

    /**
     * Ignore if exists.
     *
     * ignoreIfExists?: boolean;
     */
    std::optional<boolean> ignoreIfExists;
  };

  /**
   * Create file operation.
   *
   * export interface CreateFile {
   *   kind: 'create';
   *   uri: DocumentUri;
   *   options?: CreateFileOptions;
   *   annotationId?: ChangeAnnotationIdentifier;
   * }
   */
  struct CreateFile {

    /**
     * A create
     *
     * kind: 'create';
     */
    const std::string kind = "create";

    /**
     * The resource to create.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * Additional options
     *
     * options?: CreateFileOptions;
     */
    std::optional<std::unique_ptr<CreateFileOptions>> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ChangeAnnotationIdentifier>> annotationId;
  };

  /**
   * Rename file options.
   *
   * export interface RenameFileOptions {
   *   overwrite?: boolean;
   *   ignoreIfExists?: boolean;
   * }
   */
  struct RenameFileOptions {

    /**
     * Overwrite target if existing. Overwrite wins over `ignoreIfExists`
     *
     * overwrite?: boolean;
     */
    std::optional<boolean> overwrite;

    /**
     * Ignores if target exists.
     *
     * ignoreIfExists?: boolean;
     */
    std::optional<boolean> ignoreIfExists;
  };

  /**
   * Rename file operation.
   *
   * export interface RenameFile {
   *   kind: 'rename';
   *   oldUri: DocumentUri;
   *   newUri: DocumentUri;
   *   options?: RenameFileOptions;
   *   annotationId?: ChangeAnnotationIdentifier;
   * }
   */
  struct RenameFile {

    /**
     * A rename.
     *
     * kind: 'rename';
     */
    const std::string kind = "rename";

    /**
     * The old (existing) location.
     *
     * oldUri: DocumentUri;
     */
    DocumentUri oldUri;

    /**
     * The new location.
     *
     * newUri: DocumentUri;
     */
    DocumentUri newUri;

    /**
     * Rename options.
     *
     * options?: RenameFileOptions;
     */
    std::optional<std::unique_ptr<RenameFileOptions>> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ChangeAnnotationIdentifier>> annotationId;
  };

  /**
   * Delete file options.
   *
   * export interface DeleteFileOptions {
   *   recursive?: boolean;
   *   ignoreIfNotExists?: boolean;
   * }
   */
  struct DeleteFileOptions {

    /**
     * Delete the content recursively if a folder is denoted.
     *
     * recursive?: boolean;
     */
    std::optional<boolean> recursive;

    /**
     * Ignore the operation if the file doesn't exist.
     *
     * ignoreIfNotExists?: boolean;
     */
    std::optional<boolean> ignoreIfNotExists;
  };

  /**
   * Delete file operation.
   *
   * export interface DeleteFile {
   *   kind: 'delete';
   *   uri: DocumentUri;
   *   options?: DeleteFileOptions;
   *   annotationId?: ChangeAnnotationIdentifier;
   * }
   */
  struct DeleteFile {

    /**
     * A delete.
     *
     * kind: 'delete';
     */
    const std::string kind = "delete";

    /**
     * The file to delete.
     *
     * uri: DocumentUri;
     */
    DocumentUri uri;

    /**
     * Delete options.
     *
     * options?: DeleteFileOptions;
     */
    std::optional<std::unique_ptr<DeleteFileOptions>> options;

    /**
     * An optional annotation identifier describing the operation.
     *
     * annotationId?: ChangeAnnotationIdentifier;
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ChangeAnnotationIdentifier>> annotationId;
  };

  enum class DocumentChangeType {
    TEXT_DOCUMENT_EDIT,
    CREATE_FILE,
    RENAME_FILE,
    DELETE_FILE,
  };

  struct DocumentChange {
    DocumentChangeType type;
    union {
      std::unique_ptr<TextDocumentEdit> textDocumentEdit;
      std::unique_ptr<CreateFile> createFile;
      std::unique_ptr<RenameFile> renameFile;
      std::unique_ptr<DeleteFile> deleteFile;
    };
  };

  /**
   * A workspace edit represents changes to many resources managed in the
   * workspace. The edit should either provide changes or documentChanges. If
   * the client can handle versioned document edits and if documentChanges are
   * present, the latter are preferred over changes.
   *
   * Since version 3.13.0 a workspace edit can contain resource operations
   * (create, delete or rename files and folders) as well. If resource
   * operations are present clients need to execute the operations in the order
   * in which they are provided. So a workspace edit for example can consist of
   * the following two changes: (1) create file a.txt and (2) a text document
   * edit which insert text into file a.txt. An invalid sequence (e.g. (1)
   * delete file a.txt and (2) insert text into file a.txt) will cause failure
   * of the operation. How the client recovers from the failure is described by
   * the client capability: workspace.workspaceEdit.failureHandling
   *
   * export interface WorkspaceEdit {
   *   changes?: { [uri: DocumentUri]: TextEdit[]; };
   *   documentChanges?: (
   *     TextDocumentEdit[] |
   *     (TextDocumentEdit | CreateFile | RenameFile | DeleteFile)[]
   *   );
   *   changeAnnotations?: {
   *     [id: string]: ChangeAnnotation;
   *   };
   * }
   */
  struct WorkspaceEdit {

    /**
     * Holds changes to existing resources.
     *
     * changes?: { [uri: DocumentUri]: TextEdit[]; };
     */
    std::optional<std::unique_ptr<std::map<DocumentUri, std::unique_ptr<std::vector<TextEdit>>>>> changes;

    /**
     * Depending on the client capability
     * `workspace.workspaceEdit.resourceOperations` document changes are either
     * an array of `TextDocumentEdit`s to express changes to n different text
     * documents where each text document edit addresses a specific version of a
     * text document. Or it can contain above `TextDocumentEdit`s mixed with
     * create, rename and delete file / folder operations.
     *
     * Whether a client supports versioned document edits is expressed via
     * `workspace.workspaceEdit.documentChanges` client capability.
     *
     * If a client neither supports `documentChanges` nor
     * `workspace.workspaceEdit.resourceOperations` then only plain `TextEdit`s
     * using the `changes` property are supported.
     *
     * documentChanges?: (
     *   TextDocumentEdit[] |
     *   (TextDocumentEdit | CreateFile | RenameFile | DeleteFile)[]
     * );
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<DocumentChange>>>> documentChanges;

    /**
     * A map of change annotations that can be referenced in
     * `AnnotatedTextEdit`s or create, rename and delete file / folder
     * operations.
     *
     * Whether clients honor this property depends on the client capability
     * `workspace.changeAnnotationSupport`.
     *
     * changeAnnotations?: {
     *   [id: string]: ChangeAnnotation;
     * };
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<std::map<string, std::unique_ptr<ChangeAnnotation>>>> changeAnnotations;
  };

  /**
   * The kind of resource operations supported by the client.
   *
   * export type ResourceOperationKind = 'create' | 'rename' | 'delete';
   * export namespace ResourceOperationKind {
   *   export const Create: ResourceOperationKind = 'create';
   *   export const Rename: ResourceOperationKind = 'rename';
   *   export const Delete: ResourceOperationKind = 'delete';
   * }
   */
  enum class ResourceOperationKind {

    /**
     * Supports creating new files and folders.
     *
     * export const Create: ResourceOperationKind = 'create';
     */
    Create,

    /**
     * Supports renaming existing files and folders.
     *
     * export const Rename: ResourceOperationKind = 'rename';
     */
    Rename,

    /**
     * Supports deleting existing files and folders.
     *
     * export const Delete: ResourceOperationKind = 'delete';
     */
    Delete,
  };

  extern std::map<ResourceOperationKind, std::string> ResourceOperationKindNames;

  auto resourceOperationKind(const std::string &name) -> ResourceOperationKind;

  /**
   * export type FailureHandlingKind = 'abort' | 'transactional' | 'undo'
   *   | 'textOnlyTransactional';
   * export namespace FailureHandlingKind {
   *   export const Abort: FailureHandlingKind = 'abort';
   *   export const Transactional: FailureHandlingKind = 'transactional';
   *   export const TextOnlyTransactional: FailureHandlingKind
   *     = 'textOnlyTransactional';
   *   export const Undo: FailureHandlingKind = 'undo';
   * }
   */
  enum class FailureHandlingKind {

    /**
     * Applying the workspace change is simply aborted if one of the changes
     * provided fails. All operations executed before the failing operation stay
     * executed.
     *
     * export const Abort: FailureHandlingKind = 'abort';
     */
    Abort,

    /**
     * All operations are executed transactional. That means they either all
     * succeed or no changes at all are applied to the workspace.
     *
     * export const Transactional: FailureHandlingKind = 'transactional';
     */
    Transactional,

    /**
     * If the workspace edit contains only textual file changes they are
     * executed transactional. If resource changes (create, rename or delete
     * file) are part of the change the failure handling strategy is abort.
     *
     * export const TextOnlyTransactional: FailureHandlingKind
     *   = 'textOnlyTransactional';
     */
    TextOnlyTransactional,

    /**
     * The client tries to undo the operations already executed. But there is no
     * guarantee that this is succeeding.
     *
     * export const Undo: FailureHandlingKind = 'undo';
     */
    Undo,
  };

  extern std::map<FailureHandlingKind, std::string> FailureHandlingKindNames;

  auto failureHandlingKind(const std::string &name) -> FailureHandlingKind;

  struct ChangeAnnotationSupport {

    /**
     * Whether the client groups edits with equal labels into tree nodes,
     * for instance all edits labelled with "Changes in Strings" would
     * be a tree node.
     *
     * groupsOnLabel?: boolean;
     */
    std::optional<boolean> groupsOnLabel;
  };

  /**
   * New in version 3.13: ResourceOperationKind and FailureHandlingKind and the
   * client capability workspace.workspaceEdit.resourceOperations as well as
   * workspace.workspaceEdit.failureHandling.
   *
   * The capabilities of a workspace edit has evolved over the time. Clients can
   * describe their support using the following client capability:
   *
   * Client Capability:
   *   property path (optional): workspace.workspaceEdit
   *   property type: WorkspaceEditClientCapabilities
   *
   * export interface WorkspaceEditClientCapabilities {
   *   documentChanges?: boolean;
   *   resourceOperations?: ResourceOperationKind[];
   *   failureHandling?: FailureHandlingKind;
   *   normalizesLineEndings?: boolean;
   *   changeAnnotationSupport?: {
   *     groupsOnLabel?: boolean;
   *   };
   * }
   */
  struct WorkspaceEditClientCapabilities {

    /**
     * The client supports versioned document changes in `WorkspaceEdit`s
     *
     * documentChanges?: boolean;
     */
    std::optional<boolean> documentChanges;

    /**
     * The resource operations the client supports. Clients should at least
     * support 'create', 'rename' and 'delete' files and folders.
     *
     * resourceOperations?: ResourceOperationKind[];
     *
     * @since 3.13.0
     */
    std::optional<std::unique_ptr<std::vector<std::unique_ptr<ResourceOperationKind>>>> resourceOperations;

    /**
     * The failure handling strategy of a client if applying the workspace edit
     * fails.
     *
     * failureHandling?: FailureHandlingKind;
     *
     * @since 3.13.0
     */
    std::optional<std::unique_ptr<FailureHandlingKind>> failureHandling;

    /**
     * Whether the client normalizes line endings to the client specific
     * setting. If set to `true` the client will normalize line ending
     * characters in a workspace edit to the client specific new line
     * character(s).
     *
     * normalizesLineEndings?: boolean;
     *
     * @since 3.16.0
     */
    std::optional<boolean> normalizesLineEndings;

    /**
     * Whether the client in general supports change annotations on text edits,
     * create file, rename file and delete file changes.
     *
     * changeAnnotationSupport?: {
     *   groupsOnLabel?: boolean;
     * };
     *
     * @since 3.16.0
     */
    std::optional<std::unique_ptr<ChangeAnnotationSupport>> changeAnnotationSupport;
  };

  // Work done progress is reported using the generic $/progress notification.
  // The value payload of a work done progress notification can be of three
  // different forms.

  /**
   * To start progress reporting a $/progress notification with the following
   * payload must be sent:
   *
   * export interface WorkDoneProgressBegin {
   *   kind: 'begin';
   *   title: string;
   *   cancellable?: boolean;
   *   message?: string;
   *   percentage?: uinteger;
   * }
   */
  struct WorkDoneProgressBegin {

    /**
     * kind: 'begin';
     */
    const std::string kind = "begin";

    /**
     * Mandatory title of the progress operation. Used to briefly inform about
     * the kind of operation being performed.
     *
     * Examples: "Indexing" or "Linking dependencies".
     *
     * title: string;
     */
    string title;

    /**
     * Controls if a cancel button should show to allow the user to cancel the
     * long running operation. Clients that don't support cancellation are
     * allowed to ignore the setting.
     *
     * cancellable?: boolean;
     */
    std::optional<boolean> cancellable;

    /**
     * Optional, more detailed associated progress message. Contains
     * complementary information to the `title`.
     *
     * Examples: "3/25 files", "project/src/module2", "node_modules/some_dep".
     * If unset, the previous progress message (if any) is still valid.
     *
     * message?: string;
     */
    std::optional<string> message;

    /**
     * Optional progress percentage to display (value 100 is considered 100%).
     * If not provided infinite progress is assumed and clients are allowed to
     * ignore the `percentage` value in subsequent report notifications.
     *
     * The value should be steadily rising. Clients are free to ignore values
     * that are not following this rule. The value range is [0, 100].
     *
     * percentage?: uinteger;
     */
    std::optional<uinteger> percentage;
  };

  /**
   * Reporting progress is done using the following payload.
   *
   * export interface WorkDoneProgressReport {
   *   kind: 'report';
   *   cancellable?: boolean;
   *   message?: string;
   *   percentage?: uinteger;
   * }
   */
  struct WorkDoneProgressReport {

    /**
     * kind: 'report';
     */
    const std::string kind = "report";

    /**
     * Controls enablement state of a cancel button. This property is only valid
     * if a cancel button got requested in the `WorkDoneProgressBegin` payload.
     *
     * Clients that don't support cancellation or don't support control the
     * button's enablement state are allowed to ignore the setting.
     *
     * cancellable?: boolean;
     */
    std::optional<boolean> cancellable;

    /**
     * Optional, more detailed associated progress message. Contains
     * complementary information to the `title`.
     *
     * Examples: "3/25 files", "project/src/module2", "node_modules/some_dep".
     * If unset, the previous progress message (if any) is still valid.
     *
     * message?: string;
     */
    std::optional<string> message;

    /**
     * Optional progress percentage to display (value 100 is considered 100%).
     * If not provided infinite progress is assumed and clients are allowed
     * to ignore the `percentage` value in subsequent report notifications.
     *
     * The value should be steadily rising. Clients are free to ignore values
     * that are not following this rule. The value range is [0, 100].
     *
     * percentage?: uinteger;
     */
    std::optional<uinteger> percentage;
  };

  /**
   * Signaling the end of a progress reporting is done using the following payload.
   *
   * export interface WorkDoneProgressEnd {
   *   kind: 'end';
   *   message?: string;
   * }
   */
  struct WorkDoneProgressEnd {

    /**
     * kind: 'end';
     */
    const std::string kind = "end";

    /**
     * Optional, a final message indicating to for example indicate the outcome
     * of the operation.
     *
     * message?: string;
     */
    std::optional<string> message;
  };

  // ---------------------------------------------------------------------------
  // Work Done progress can be initiated in two different ways:
  // 1. By the sender of a request (mostly clients) using the predefined
  // workDoneToken property in the requests parameter literal. The document will
  // refer to this kind of progress as client initiated progress.
  // 2. By a server using the request window/workDoneProgress/create. The
  // document will refer to this kind of progress as server initiated progress.
  // ---------------------------------------------------------------------------

  /**
   * Consider a client sending a textDocument/reference request to a server and
   * the client accepts work done progress reporting on that request. To signal
   * this to the server the client would add a workDoneToken property to the
   * reference request parameters.
   *
   * The corresponding type definition for the parameter property looks like this:
   *
   * export interface WorkDoneProgressParams {
   *   workDoneToken?: ProgressToken;
   * }
   */
  struct WorkDoneProgressParams {

    /**
     * An optional token that a server can use to report work done progress.
     *
     * workDoneToken?: ProgressToken;
     */
    std::optional<std::unique_ptr<ProgressToken>> workDoneToken;
  };

  /**
   * A server uses the workDoneToken to report progress for the specific
   * textDocument/reference.
   *
   * The token received via the workDoneToken property in a request’s param
   * literal is only valid as long as the request has not send a response back.
   * Canceling work done progress is done by simply canceling the corresponding
   * request.
   *
   * There is no specific client capability signaling whether a client will send
   * a progress token per request. The reason for this is that this is in many
   * clients not a static aspect and might even change for every request
   * instance for the same request type. So the capability is signal on every
   * request instance by the presence of a workDoneToken property.
   *
   * To avoid that clients set up a progress monitor user interface before
   * sending a request but the server doesn’t actually report any progress a
   * server needs to signal general work done progress reporting support in the
   * corresponding server capability. For the above find references example a
   * server would signal such a support by setting the referencesProvider
   * property in the server capabilities
   *
   * The corresponding type definition for the server capability looks like
   * this:
   *
   * export interface WorkDoneProgressOptions {
   *   workDoneProgress?: boolean;
   * }
   */
  struct WorkDoneProgressOptions {
    std::optional<boolean> workDoneProgress;
  };

  /**
   * A parameter literal used to pass a partial result token.
   *
   * export interface PartialResultParams {
   *   partialResultToken?: ProgressToken;
   * }
   */
  struct PartialResultParams {

    /**
     * An optional token that a server can use to report partial results (e.g.
     * streaming) to the client.
     *
     * partialResultToken?: ProgressToken;
     */
    std::optional<std::unique_ptr<ProgressToken>> partialResultToken;
  };

  /**
   * A TraceValue represents the level of verbosity with which the server
   * systematically reports its execution trace using $/logTrace notifications.
   * The initial trace value is set by the client at initialization and can be
   * modified later using the $/setTrace notification.
   */
  enum class TraceValue {
    OFF,
    MESSAGES,
    VERBOSE,
  };

  extern std::map<TraceValue, std::string> TraceValueNames;

  auto traceValue(const std::string &name) -> TraceValue;

} // namespace LCompilers::LanguageServiceProvider

#endif // LCOMPILERS_LSP_SPECIFICATION_H
