#include <iostream>
#include <fstream>
#include <sstream>

#include "toylet.h"
#include "scanner.h"

void Toylet::file(std::string filename) const
{
    auto errors = std::make_unique<ErrorReporter>(filename);

    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();

    // TODO.md: Process a chunk at a time rather than reading the whole file into memory.
    run(buffer.str(), *errors);

    if (errors->count()) {
        errors->report(std::cout);
        exit(65);
    }
}

void Toylet::prompt() const
{
    auto errors = std::make_unique<ErrorReporter>();
    std::string input;

    for (;;) {
        std::cout << "> ";
        getline(std::cin, input);
        std::cout << input << std::endl;
        run(input, *errors);
    }
}

void Toylet::run(std::string const& code, ErrorReporter& errors) const
{
    Scanner scanner(code, errors);
    const TokenList& tokens = scanner.scan();

    for (auto const& token : tokens) {
        std::cout << "Token: " << *token << std::endl;
    }
}
