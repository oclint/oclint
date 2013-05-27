#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;

#include <vector>
#include <utility>
#include <string>

using namespace std;

#include <clang/AST/AST.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/Tooling.h>

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

class TestFrontendAction : public clang::ASTFrontendAction
{
private:
    MatchFinder* _finder;

public:
    TestFrontendAction(MatchFinder &finder)
    {
        _finder = &finder;
    }

    clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef)
    {
        return _finder->newASTConsumer();
    }
};

void testMatcherOnCode(const Twine &fileName, MatchFinder &finder, const string &code)
{
    FrontendAction *action = new TestFrontendAction(finder);
    Twine twine(code);
    if (!runToolOnCode(action, twine, fileName))
    {
        FAIL();
    }
}

void testMatcherOnCode(MatchFinder &finder, const string &code)
{
    testMatcherOnCode("input.c", finder, code);
}

void testMatcherOnCXXCode(MatchFinder &finder, const string &code)
{
    testMatcherOnCode("input.cpp", finder, code);
}

void testMatcherOnObjCCode(MatchFinder &finder, const string &code)
{
    testMatcherOnCode("input.m", finder, code);
}
