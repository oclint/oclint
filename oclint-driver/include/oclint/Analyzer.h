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
    virtual void preprocess(std::vector<clang::ASTContext *> &contexts) {}
    virtual void analyze(std::vector<clang::ASTContext *> &contexts) = 0;
    virtual void postprocess(std::vector<clang::ASTContext *> &contexts) {}
};

} // end namespace oclint

#endif
