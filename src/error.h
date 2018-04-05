#pragma once

#include <bits/unique_ptr.h>
#include <string>
#include <vector>

typedef struct {
public:
    unsigned int const line;
    unsigned int const column;
} ErrorPosition;

class Error
{
public:
    ErrorPosition const position;
    std::string const message;
    std::string const context = "";
    std::string const where = "";

    Error(ErrorPosition position, std::string message, std::string context);
};

typedef std::vector<std::unique_ptr<Error>> ErrorList;