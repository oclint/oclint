#ifndef OCLINT_ANALYZER_H
#define OCLINT_ANALYZER_H

#include <vector>

namespace clang
{
    class ASTContext;
}

namespace oclint
{

class Analyzer
{
public:
    virtual void preprocess(clang::ASTContext *context) {}
    virtual void analyze(clang::ASTContext *context) = 0;
    virtual void postprocess(clang::ASTContext *context) {}
};

} // end namespace oclint

#endif
