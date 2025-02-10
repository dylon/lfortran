#include <cctype>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>

#include <lsp/communication_protocol.h>
#include <lsp/request_parser.h>

namespace LCompilers::LanguageServer {
  using namespace std::literals::chrono_literals;

  CommunicationProtocol::CommunicationProtocol(
    LanguageServer &languageServer,
    RequestParserFactory &parserFactory,
    MessageQueue &incomingMessages,
    MessageQueue &outgoingMessages,
    lsl::Logger &logger)
    : languageServer(languageServer)
    , parserFactory(parserFactory)
    , incomingMessages(incomingMessages)
    , outgoingMessages(outgoingMessages)
    , logger(logger)
  {
    // empty
  }

  StdIOCommunicationProtocol::StdIOCommunicationProtocol(
    LanguageServer &languageServer,
    RequestParserFactory &parserFactory,
    MessageQueue &incomingMessages,
    MessageQueue &outgoingMessages,
    lsl::Logger &logger)
    : CommunicationProtocol(
      languageServer,
      parserFactory,
      incomingMessages,
      outgoingMessages,
      logger
    )
    , listener([this]() {
      listen();
    })
  {
    // empty
  }

  auto StdIOCommunicationProtocol::listen() -> void {
    try {
      do {
        const std::string message = incomingMessages.dequeue();
        std::cout << message << std::flush;
        {
          auto loggerLock = logger.lock();
          logger << message << std::endl;
        }
      } while (running);
    } catch (std::runtime_error &e) {
      auto loggerLock = logger.lock();
      logger
        << "Interrupted while dequeuing messages: "
        << e.what()
        << std::endl;
    }
  }

  void StdIOCommunicationProtocol::serve() {
    {
      auto loggerLock = logger.lock();
      logger
        << "Serving stdio requests."
        << std::endl;
    }
    std::unique_ptr<RequestParser> parser = parserFactory.build();
    try {
      bool pending = true;
      while (!languageServer.isTerminated()) {
        // TODO: Buffer the input stream from a separate thread or find a more
        // efficient way to buffer the reads. I tried `fread`, but it blocks
        // until the requested number of characters have been read. I also tried
        // `std::cin.readsome` but it does not block at all (i.e. it spin
        // locks); to use `readsome` we would need to introduce sleep cycles
        // which is less than ideal.
        char c;
        std::cin.get(c);
        if (!languageServer.isTerminated()) {
          pending = !parser->parse(static_cast<unsigned char>(c));
          if (!pending) {
            if (parser->state() == RequestParserState::COMPLETE) {
              const std::string &message = parser->body();
              {
                auto loggerLock = logger.lock();
                const std::string &startLine = parser->startLine();
                if (startLine.length() > 0) {
                  logger
                    << "request.startLine = \"" << startLine << "\""
                    << std::endl;
                }
                for (auto const &[header, value] : parser->headers()) {
                  logger
                    << "request.headers[\"" << header << "\"] = \"" << value << "\""
                    << std::endl;
                }
                logger << "request.body = '" << message << "'" << std::endl;
              }
              if (message.length() > 0) {
                outgoingMessages.enqueue(message);
              } else {
                auto loggerLock = logger.lock();
                logger << "Cannot parse an empty request body." << std::endl;
              }
            } else if (parser->state() == RequestParserState::ERROR) {
              const std::string &error = parser->error();
              {
                auto loggerLock = logger.lock();
                logger << "Failed to parse request: " << error << std::endl;
              }
            } else {
              auto loggerLock = logger.lock();
              logger
                << "Parser in unexpected state: "
                << static_cast<int>(parser->state())
                << std::endl;
            }
            parser->reset();
            pending = true;
          }
        }
      }
      if (pending) {
        parser->finish();
        const std::string &message = parser->body();
        {
          auto loggerLock = logger.lock();
          const std::string &startLine = parser->startLine();
          if (startLine.length() > 0) {
            logger
              << "request.startLine = \"" << startLine << "\""
              << std::endl;
          }
          for (auto const &[header, value] : parser->headers()) {
            logger
              << "request.headers[\"" << header << "\"] = \"" << value << "\""
              << std::endl;
          }
          logger << "request.body = " << message << std::endl;
        }
        if (message.length() > 0) {
          outgoingMessages.enqueue(message);
        } else {
          auto loggerLock = logger.lock();
          logger << "Cannot parse an empty request body." << std::endl;
        }
      }
    } catch (std::exception &e) {
      auto loggerLock = logger.lock();
      logger
        << "Caught unhandled exception while serving requests: " << e.what()
        << std::endl;
    }
    running = false;
    incomingMessages.stop();
    listener.join();
  }

} // namespace LCompilers::LanguageServer
