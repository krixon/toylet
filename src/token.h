#pragma once

#include <iosfwd>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <bits/unique_ptr.h>

enum class TokenType
{
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    IDENTIFIER,
    STRING,
    NUMBER,
    AND,
    CLASS,
    ELSE,
    FALSE,
    FN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,
    END_OF_FILE
};

class Token
{
public:
    TokenType const type;
    std::string const lexeme;
    unsigned int const offset = 0;

    Token(TokenType type, std::string lexeme, unsigned int offset)
        : type(type), lexeme(std::move(lexeme)), offset(offset)
    {}
};

typedef std::vector<std::unique_ptr<Token>> TokenList;

std::ostream& operator<<(std::ostream& out, Token const& token);

template <class T>
class LiteralToken : public Token
{
public:
    T const literal;

    LiteralToken(TokenType type, std::string lexeme, unsigned int offset, T literal)
        : Token(type, lexeme, offset), literal(std::move(literal))
    {}
};
