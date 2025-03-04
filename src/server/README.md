# LLanguage Server

Language server framework from the LCompilers suite of language tools.
Presently, only Microsoft's Language Server Protocol (LSP) is supported but
other protocols may be added in the future.

## Language Server

A language server provides the glue between an editor/IDE and language tools
like compilers, linters, and formatters. It provides a common API for handling
such notifications (events) as editing documents, validating documents
(syntactic and semantic analyses), code completion, navigating among symbols,
and refactoring source code.

A language server may be synchronous or asynchronous. It may either maintain a
persistent state or execute batch processes to handle notifications and serve
requests. Asynchronous language servers are preferred since they will not block
the editor while it awaits a reply.

## Language Server Protocol (LSP)

Microsoft has implemented the [Language Server Protocol
(LSP)](https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/)
that describes most operations required by modern editors. It has become very
popular and been adopted as a common standard for integrating language tools
with said editors.

The protocol is based on [JSON-RPC 2.0](https://www.jsonrpc.org/specification),
which invokes methods and responds over a network with
[JSON](https://www.json.org/json-en.html) as its content type.

Messages take a form extremely similar to [HTTP
messages](https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages) but lack
the start line and typically contain just the `Content-Type` and
`Content-Length` headers. Only the latter is required since the `Content-Type`
must be JSON. Other headers may be included but will almost certainly be ignored
by the server. The encoding is assumed to be UTF-8. Headers are separated with
the Windows CR-LF (`\r\n`) regardless the platform and the JSON message body is
separated from the headers with an extra CR-LF (i.e. `\r\n\r\n`). No newline
follows the message body, which is why the `Content-Length` header is required;
exactly `Content-Length` characters (bytes) will be read from the message body.
The following demonstrates the proper form of an LSP message (newlines are
represented by `\r\n` and lines are broken, following CR-LF, for clarity):

```text
Content-Type: application/vscode-jsonrpc; charset=utf-8\r\n
Content-Length: 52\r\n
\r\n
{"jsonrpc":"2.0","method":"initialized","params":{}}
```

### Lifecycle

When the server is spun up, the client will sent it an
[initialize](https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#initialize)
request. This provides the server with a set of parameters describing the client
and its capabilities. The server will respond with a list of its abilities,
scoped according to what the client supports. Until the server has been
initialized, no other request or notification is permitted except `exit` (see
below).

Once the client receives the response and becomes ready, it sends the server an
[initialized](https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#initialized)
notification. This lets the server know that initialization is complete and
gives it the opportunity to register additional hooks that are not made
available by the response API of the `initialize` request.

When the client is ready to terminate, it sends the server a
[shutdown](https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#shutdown)
request. This gives the server time to free up any resources it owns and to
prepare to terminate. Once the server receives a request to shutdown, it is not
permitted to accept any more requests or notifications except the `exit`
notification.

When the client receives the acknowledgement of its `shutdown` request, it will
send the server an
[exit](https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#exit)
notification. This instructs the server to terminate. By this time, all its
resources should have been cleaned up and termination should be done promptly.

## Inter-Process Communication (IPC)

Various mediums of transportation may facilitate communication between the
language server and client (e.g. editor extension), with the most common being
standard input and output communication (stdio). Alternatives include TCP and
websocket communication, but we will focus on stdio since it is the most common,
simplest, and supported by nearly every editor.

### Standard Input/Output Communication (stdio)

With stdio communication, the editor initializes the language server as a
subprocess and communicates with it over its input and output streams. The
client sends requests, notifications, and responses to the server by writing
them as plain text to its input stream. In the same spirit, it receives
responses, requests, and notifications from the server as plain text messages
from its output stream. Conversely, the server receives requests, notifications
and responses as plain text from its input stream and sends responses, requests,
and notifications by writing them as plain text to its output stream.

Messages written to the server's output stream are not printed to a terminal but
consumed and handled by the client. The server's error stream (standard error
stream or stderr) should not be assumed part of the stdio communication
protocol, but some editors will log messages written to the server's error
stream to a debugging terminal (e.g. VSCode).

## Overview

LLanguage Server is fully asynchronous. Messages are read from its input stream
and appended to a message queue. A pool of request threads consume the incoming
queue to fulfill requests. By default, 5 threads serve requests.

This frees the thread that consumes the input stream to continuously read from
it without blocking for a response. Multiple messages may processed in parallel
if they are received faster than they can be fulfilled. Processing them in
parallel prevents the editor from blocking while the user edits the document.
Responses are served in the order of receipt even if one is completed before
another.

Once a response is ready, it is written to an outgoing message queue. A separate
thread consumes the outgoing queue and writes messages to the server's output
stream. Exactly one thread reads from the input stream and exactly one thread
writes to the output stream. Those threads are dedicated to their purpose and
are not used for anything else. This makes reading from the input stream and
writing to the output stream thread-safe and efficient.

The request pool shares a worker thread pool for asynchronous background tasks
like analyzing documents for issues (document validation). By default, the size
of this pool is fixed at the number of available CPU cores.

The message queues are thread-safe, fixed-size, and blocking. Although they are
fixed in size, they are given a generous enough capacity that they should not
fill up. It is important to use fixed-size message queues to avoid edge cases
that can cause the queue to become so large the host runs out of memory.

### Logging

There are two types of logging in LLanguage Server:
1. Application logging
2. Trace logging

Application logging consists of information about server at various levels of
verbosity and is written to the server's error stream (stderr). Application logs
must not be written to the output stream (stdout) because it is reserved for
sending messages to the client.

Trace logging consists of such information as a specific method being called,
how long it took to call it, and any parameters or return values passed-to or
returned-from it. Trace logs are sent to the client from the server by way of
the
[`$/logTrace`](https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#logTrace)
API. Within the context of LLanguage Server, trace logging is performed by
calling `LspLanguageServer::sendLogTrace`.

### Code Generation

The LSP data types and related files are generated from [Microsoft's latest LSP
spec](https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#metaModel)
(version 3.17.0 as of this writing). Below is a list of the generated files and
a brief synopsis about each:
- `src/server/lsp_specification.h`
  - Defines the data types from the LSP spec.
- `src/server/lsp_specification.cpp`
  - Implements the methods associated with the various LSP types.
- `src/server/lsp_transformer.h`
  - Describes `LspTransformer` which transforms between specific LSP data types
    and the generic `LSPAny` type.
- `src/server/lsp_transformer.cpp`
  - Defines the operations of `LspTransformer`.
- `src/server/lsp_language_server.h`
  - Describes the abstract base class for LSP language servers (`LspLanguageServer`)
- `src/server/lsp_language_server.cpp`
  - Defines the operations of `LspLanguageServer`

The generated types follow these conventions:
- Enumerations are generated as `enum class` instances of the given `${enum_name}`.
  - Each enumeration receives a `std::map<${enum_name}, std::string>` that maps
    each enumerator to its name:
    - `${enum_name}Names`
  - Each enumeration receives a function that maps an enumerator's string name
    to itself:
    - `auto ${enum_name}ByName(const std::string &name) -> ${enum_name}`
  - Each string-valued enumeration receives a
    `std::map<${enum_name}, std::string>` that maps each enumerator to its
    value:
    - `${enum_name}Values`
  - Each enumeration receives a function that maps an enumerator's string- or
    (u)integer-valued value to itself:
    - `auto ${enum_name}ByValue(${value_type} value) -> ${enum_name}`
      - `${value_type}` is `const std::string &` for string-valued enumerations.
      - `${value_type}` is either `int` or `unsigned int` for (u)integer-valued
        enumerations.
  - Each string-valued enumeration receives a function that returns whether a
    string is one of its values:
    - `auto is${enum_name}(const std::string &value) -> bool`
- Structures are modeled as `struct` instances with public fields.
- Nested structures are modeled as `struct` instances with public fields.
- Union types and fields are modeled as tagged unions within `class` instances.
  - Each union type gets its own `${union_name}Type` enumeration and
    corresponding `${union_name}Type type` field specifying which of its union
    fields is active.
  - Each union type has its fields modeled within an anonymous `union`.
    - Primitive fields are modeled directly.
    - Non-primitive fields (e.g. `std::string`, structures, other union types,
      and instances of `std::map`, `std::vector`, or `std::tuple`) are of type
      `std::unique_ptr<{field_type}>`, where `${field_type}` is the data type of
      the field.
    - The only way to access the union members is through setters and getters.
      - The setters are modeled as
        `auto ${union_name}::operator=(${field_type} ${field_name}) -> void`
        methods, where `${field_name}` is the name of the union member.
      - In debug mode (cmake run with `-DCMAKE_BUILD_TYPE=Debug`), the getters
        will ensure the active type is associated with the union member value
        being retrieved and will throw an exception if it is not. They are
        modeled as `auto ${union_name}::${field_name}() -> ${field_type}`
        methods.
- Cyclic structure and union dependencies are forward-declared.
  - Cyclic references to these forward-declared types are of type
    `std::unique_ptr<{cyclic_type}>`, where `${cyclic_type}` is the name of the
    cyclic data type.
