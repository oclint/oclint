#ifndef OCLINT_PROCESSOR_H
#define OCLINT_PROCESSOR_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

class Reporter;

using namespace clang;

class Processor : public ASTConsumer
{
private:
    Reporter *_reporter;
    Processor();

public:
    Processor(Reporter *reporter);
    virtual void HandleTranslationUnit(ASTContext &astContext);
};

#endif
