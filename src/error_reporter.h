#pragma once

#include <string>

#include "error.h"

class ErrorReporter
{
public:
    ErrorReporter() = default;
    explicit ErrorReporter(std::string filename);

    void add(ErrorPosition position, std::string message);
    void add(ErrorPosition position, std::string message, std::string context);
    void report(std::ostream& out) const;
    unsigned long count() const;

private:
    ErrorList errors{};
    std::string const filename = "";
};
