#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "token.h"
#include "error_reporter.h"

class Scanner
{
public:
    Scanner(std::string code, ErrorReporter& errors);

    TokenList const& scan();

private:
    std::string const code;
    ErrorReporter& errors;
    TokenList tokens{};
    unsigned int start = 0;
    unsigned int current = 0;
    static std::unordered_map<std::string, TokenType> const keywords;

    /**
     * Scans the next token.
     */
    void next();

    /**
     * Produces a string literal token.
     */
    void string();

    /**
     * Produces a number literal token.
     */
    void number();

    /**
     * Produces an identifier token.
     */
    void identifier();

    /**
     * Determines if the end of the source code has been reached.
     */
    bool isAtEnd() const;

    /**
     * Returns the current character without advancing.
     */
    char peek() const;

    /**
     * Returns the character at the specified offset from the current character, without advancing.
     */
    char peek(int offset) const;

    /**
     * Consumes and returns the current character then advances to the next character.
     */
    char advance();

    /**
     * Attempts to match the current character with the one specified.
     *
     * If successful, consumes the current character and advances to the next character.
     *
     * This is like a conditional advance().
     */
    bool match(char c);

    std::string lexeme() const;

    /**
     * Adds a new token to the list.
     */
    void add(TokenType type);

    /**
     * Adds a new token with a literal string value to the list.
     */
    void add(TokenType type, std::string literal);

    /**
     * Adds a new token with a literal number value to the list.
     */
    void add(TokenType type, double literal);

    void error(std::string message);
};
