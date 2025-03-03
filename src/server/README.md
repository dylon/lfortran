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

## Architecture

LLanguage Server is fully asynchronous. Messages are read from its input stream
and appended to a message queue that is consumed by a pool of request threads.
This allows multiple messages to be handled in parallel and prevents the editor
from blocking while the user continues editing his document. Responses are
served in the order of receipt even if one is completed before another.
