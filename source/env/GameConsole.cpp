#include <radix/env/GameConsole.hpp>

#include <string>
#include <iostream>
#include <linenoise.hpp>

namespace radix {

void GameConsole::run(BaseGame &game) {
    const auto path = "history.txt";

    // Enable the multi-line mode
    linenoise::SetMultiLine(true);

    // Set max length of the history
    linenoise::SetHistoryMaxLen(4);

    linenoise::SetCompletionCallback([](const char* editBuffer, std::vector<std::string>& completions) {
        if (editBuffer[0] == 'h') {
          completions.push_back("help");
        }

        if (editBuffer[0] == 'r') {
          completions.push_back("run");
        }

        if (editBuffer[0] == 'e') {
          completions.push_back("exit");
          completions.push_back("echo");
        }

        if (editBuffer[0] == 'q') {
          completions.push_back("quit");
        }
    });

    // Load history
    linenoise::LoadHistory(path);

    while (true) {
        std::string line;

        auto quit = linenoise::Readline("command > ", line);

        if (quit) {
          break;
        }

        if (line == "help") {
          std::cout <<  "Valid commands:" << std::endl;
          std::cout <<  "quit, run, exit, help" << std::endl;
        }

        if (line == "quit") {
          std::cout <<  "Closing console" << std::endl;
          break;
        }

        if (line == "run") {
          std::cout <<  "Starting game" << std::endl;
          break;
        }

        if (line == "exit") {
          std::cout <<  "Exiting game" << std::endl;
          exit(0);
        }

        if (line == "echo") {
          std::cout <<  line << std::endl;
        }

        // Add line to history
        linenoise::AddHistory(line.c_str());

        // Save history
        linenoise::SaveHistory(path);
    }
}

} /* namespace radix */
