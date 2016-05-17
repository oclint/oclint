#ifndef OCLINT_COMPILERINSTANCE_H
#define OCLINT_COMPILERINSTANCE_H

#include <vector>

#include <clang/Frontend/CompilerInstance.h>

namespace clang
{
    class FrontendAction;
}

namespace oclint
{

class CompilerInstance : public clang::CompilerInstance
{
public:
    void start();
    void end();

private:
    std::vector<std::unique_ptr<clang::FrontendAction>> _actions;
};

} // end namespace oclint

#endif
