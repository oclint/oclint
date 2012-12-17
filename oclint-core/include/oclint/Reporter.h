#ifndef OCLINT_REPORTER_H
#define OCLINT_REPORTER_H

#include "oclint/Results.h"

using namespace clang;

class Reporter
{
public:
    virtual void report(Results *results, ostream &out) = 0;
};

#endif
