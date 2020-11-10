#pragma once
#include <stdexcept>
class InvalidFileException : public std::runtime_error
{
public:
    explicit InvalidFileException(const std::string &message = "")
        : std::runtime_error("Invalid file " + message + " either does not exist or could not be opened.")
    {
        // Empty due to using MIL
    }
};