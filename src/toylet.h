#pragma once

#include <string>
#include "error_reporter.h"

class Toylet
{
public:
    void file(std::string filename) const;
    void prompt() const;

private:
    void run(std::string const& code, ErrorReporter& errors) const;
};
