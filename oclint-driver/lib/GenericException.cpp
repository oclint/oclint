#include "oclint/GenericException.h"

#include <utility>

using namespace oclint;

GenericException::GenericException(std::string desc)
    : description(std::move(desc))
{
}

const char *GenericException::what() const throw()
{
    return description.c_str();
}
