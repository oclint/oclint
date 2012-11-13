#ifndef OCLINT_PROCESSOR_H
#define OCLINT_PROCESSOR_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>

using namespace clang;

class Processor : public ASTConsumer
{
public:
    virtual void HandleTranslationUnit(ASTContext &astContext);
};

#endif
