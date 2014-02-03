#ifndef OCLINT_REPORTER_H
#define OCLINT_REPORTER_H

#include <ostream>

namespace oclint
{

class Results;

class Reporter
{
public:
    virtual ~Reporter() {}
    virtual void report(Results *results, std::ostream &out) = 0;
    virtual const std::string name() const = 0;
};

} // end namespace oclint

#endif
