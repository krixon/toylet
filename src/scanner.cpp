#include "error_reporter.h"
#include "scanner.h"
#include <algorithm>

std::unordered_map<std::string, TokenType> const Scanner::keywords { // NOLINT
    {"and",    TokenType::AND},
    {"class",  TokenType::CLASS},
    {"else",   TokenType::ELSE},
    {"false",  TokenType::FALSE},
    {"for",    TokenType::FOR},
    {"fn",     TokenType::FN},
    {"if",     TokenType::IF},
    {"null",   TokenType::NIL},
    {"or",     TokenType::OR},
    {"print",  TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super",  TokenType::SUPER},
    {"this",   TokenType::THIS},
    {"true",   TokenType::TRUE},
    {"var",    TokenType::VAR},
    {"while",  TokenType::WHILE}
};

Scanner::Scanner(std::string code, ErrorReporter& errors) : code(std::move(code)), errors(errors)
{}

TokenList const& Scanner::scan()
{
    while (!isAtEnd()) {
        start = current;
        next();
    }

    start = current;
    add(TokenType::END_OF_FILE);

    return tokens;
}

void Scanner::next()
{
    char c = advance();

    switch (c) {
        case ' ':
        case '\r':
        case '\t':
        case '\n':
            // Ignore whitespace.
            break;

        case '(': add(TokenType::LEFT_PAREN);  break;
        case ')': add(TokenType::RIGHT_PAREN); break;
        case '{': add(TokenType::LEFT_BRACE);  break;
        case '}': add(TokenType::RIGHT_BRACE); break;
        case ',': add(TokenType::COMMA);       break;
        case '.': add(TokenType::DOT);         break;
        case '-': add(TokenType::MINUS);       break;
        case '+': add(TokenType::PLUS);        break;
        case ';': add(TokenType::SEMICOLON);   break;
        case '*': add(TokenType::STAR);        break;

        case '!': add(match('=') ? TokenType::BANG_EQUAL    : TokenType::BANG);    break;
        case '=': add(match('=') ? TokenType::EQUAL_EQUAL   : TokenType::EQUAL);   break;
        case '<': add(match('=') ? TokenType::LESS_EQUAL    : TokenType::LESS);    break;
        case '>': add(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        case '/':
            if (match('/')) {
                // Line comment. Consume everything until the next line break.
                while (peek() != '\n' && !isAtEnd()) advance();
            } else {
                add(TokenType::SLASH);
            }
            break;

        case '"': string(); break;

        default:
            if (isdigit(c)) {
                number();
            } else if (isalpha(c)) {
                identifier();
            } else {
                error("Unexpected character.");
            }
            break;
    }
}

void Scanner::string()
{
    // TODO: Support escape sequences in strings.

    while (peek() != '"' && !isAtEnd()) advance();

    if (isAtEnd()) {
        error("Unterminated string.");
        return;
    }

    std::string value = code.substr(start + 1, current - (start + 1));

    // Move past the closing ".
    advance();

    add(TokenType::STRING, value);
}

void Scanner::number()
{
    while (isdigit(peek())) advance();

    // Handle a fractional component.
    if (peek() == '.' && isdigit(peek(1))) {
        advance();
        while (isdigit(peek())) advance();
    }

    double value = strtod(lexeme().c_str(), nullptr);

    add(TokenType::NUMBER, value);
}

void Scanner::identifier()
{
    while (isalnum(peek())) advance();

    // Check if the identifier is a keyword.
    std::string value = code.substr(start, current);
    auto iter         = keywords.find(value);
    TokenType type    = iter == keywords.end() ? TokenType::IDENTIFIER : iter->second;

    add(type);
}

bool Scanner::isAtEnd() const
{
    return current >= code.length();
}

char Scanner::peek() const
{
    return peek(0);
}

char Scanner::peek(int offset) const
{
    if (current + offset >= code.length()) return '\0';
    return code.at(current + offset);
}

char Scanner::advance()
{
    return code.at(current++);
}

bool Scanner::match(char c)
{
    if (isAtEnd()) return false;
    if (peek() != c) return false;

    current++;
    return true;
}

std::string Scanner::lexeme() const
{
    return code.substr(start, current - start);
}

void Scanner::add(TokenType type)
{
    auto token = std::make_unique<Token>(type, lexeme(), start);

    tokens.push_back(std::move(token));
}

void Scanner::add(TokenType type, std::string literal)
{
    auto token = std::make_unique<LiteralToken<std::string>>(type, lexeme(), start, literal);

    tokens.push_back(std::move(token));
}

void Scanner::add(TokenType type, double literal)
{
    auto token = std::make_unique<LiteralToken<double>>(type, lexeme(), literal, start);

    tokens.push_back(std::move(token));
}

void Scanner::error(std::string message)
{
    // Work out the line number and the column on that line for the start of the current token.
    // Also keep hold of the line of code on which the error occurred so we can display it as part of the error.

    // Find the position of the start of the current line.
    size_t line_start_pos = 1 + code.find_last_of('\n', start);
    if (line_start_pos == std::string::npos) {
        line_start_pos = 0;
    }

    // Find the position of the end of the current line.
    size_t line_end_pos = code.find_first_of('\n', start);
    if (line_end_pos == std::string::npos) {
        line_end_pos = code.length();
    }

    // Find the number of the line containing the start of the bad token.
    auto line = static_cast<unsigned int>(std::count(code.begin(), code.begin() + start, '\n'));
    line++;

    // Find the column offset within the line for the start of the bad token.
    size_t column = start - line_start_pos;
    column++;

    // Grab the content of the line containing the start of the bad token.
    // Note that the bad token might span multiple lines (e.g. unterminated multi-line string).
    // For providing context to the error, only the line containing the start of the token is used.
    std::string context = code.substr(line_start_pos, line_end_pos - line_start_pos);

    auto pos = ErrorPosition{(line), static_cast<unsigned int>(column)};

    errors.add(pos, std::move(message), context);
}
