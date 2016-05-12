#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace oclint;

namespace clang {
namespace ast_matchers {
AST_MATCHER(ParmVarDecl, hasDefaultArg)
{
    return Node.hasDefaultArg();
}
} // namespace ast_matchers
} // namespace clang

class AvoidDefaultArgumentsOnVirtualMethodsRule : public AbstractASTMatcherRule
{
public:
    virtual const string name() const override
    {
        return "avoid default arguments on virtual methods";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "design";
    }

    virtual unsigned int supportedLanguages() const override
    {
        return LANG_CXX;
    }

    virtual void callback(const MatchFinder::MatchResult &result) override
    {
        const CXXMethodDecl *method = result.Nodes.getNodeAs<CXXMethodDecl>("cxxMethod");
        addViolation(method, this);
    }

    virtual void setUpMatcher() override
    {
        addMatcher(
            cxxMethodDecl(isVirtual(), hasAnyParameter(hasDefaultArg()))
            .bind("cxxMethod"));
    }
};

static RuleSet rules(new AvoidDefaultArgumentsOnVirtualMethodsRule());
