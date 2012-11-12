#ifndef OCLINT_REPORTER_H
#define OCLINT_REPORTER_H

#include <clang/AST/ASTContext.h>

#include "oclint/ViolationSet.h"

using namespace clang;

class Reporter
{
public:
    virtual void report(ASTContext &astContext, ViolationSet *violationSet) = 0;
};

#endif
