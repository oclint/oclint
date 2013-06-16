#ifndef OCLINT_GENERICEXCEPTION_H
#define OCLINT_GENERICEXCEPTION_H

#include <exception>
#include <string>

namespace oclint
{

class GenericException : public std::exception
{
public:
    GenericException(const std::string& desc);
    virtual ~GenericException() throw() {}
    virtual const char *what() const throw();

private:
    std::string description;
};

} // end namespace oclint

#endif
