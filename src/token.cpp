#include "token.h"

std::ostream& operator<<(std::ostream& out, Token const& token)
{
    return out << token.lexeme;
}
