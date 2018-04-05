#include "error.h"
#include <utility>

Error::Error(ErrorPosition position, std::string message, std::string context)
    : position(position), message(std::move(message)), context(std::move(context))
{}
