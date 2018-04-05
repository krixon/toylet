#include <utility>
#include <iomanip>
#include "error_reporter.h"
#include "error.h"

ErrorReporter::ErrorReporter(std::string filename) : filename(std::move(filename))
{}

void ErrorReporter::add(ErrorPosition position, std::string message)
{
    add(position, std::move(message), "");
}

void ErrorReporter::add(ErrorPosition position, std::string message, std::string context)
{
    errors.push_back(std::move(std::make_unique<Error>(position, message, context)));
}

void ErrorReporter::report(std::ostream& out) const
{
    for (auto const& error : errors) {
        if (!filename.empty()) {
            out << filename << ":";
        }

        out
            << error->position.line << ":" << error->position.column
            << " error" << error->where << ": " << error->message
            << std::endl;

        if (!error->context.empty()) {
            out << "    " << error->context << std::endl;
            out << "    " << std::setw(error->position.column) << "^" << std::endl;
        }
    }
}

unsigned long ErrorReporter::count() const
{
    return errors.size();
}



