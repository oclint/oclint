#include "oclint/GenericException.h"

using namespace oclint;

GenericException::GenericException(const std::string& desc) : description(desc)
{
}

const char *GenericException::what() const throw()
{
    return description.c_str();
}
