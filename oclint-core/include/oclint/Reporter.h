#ifndef OCLINT_REPORTER_H
#define OCLINT_REPORTER_H

#include <ostream>

#include "oclint/Results.h"

using namespace std;

class Reporter
{
public:
    virtual void report(Results *results, ostream &out) = 0;
    virtual const string name() const = 0;
};

#endif
