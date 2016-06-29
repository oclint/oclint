#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace oclint;

namespace clang {
namespace ast_matchers {
AST_MATCHER(VarDecl, isStaticDataMember)
{
    return Node.isStaticDataMember();
}

AST_MATCHER(CXXMethodDecl, isStatic)
{
    return Node.isStatic();
}
} // namespace ast_matchers
} // namespace clang

class AvoidPrivateStaticMembersRule : public AbstractASTMatcherRule
{
public:
    virtual const string name() const override
    {
        return "avoid private static members";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "design";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.10.1";
    }

    virtual const std::string description() const override
    {
        return "Having static members is easier to harm encapsulation.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    class Foo
    {
        static int a;       // static field
    };
    class Bar
    {
        static int b();     // static method
    }
        )rst";
    }
#endif

    virtual void callback(const MatchFinder::MatchResult &result) override
    {
        if (auto field = result.Nodes.getNodeAs<VarDecl>("field"))
        {
            addViolation(field, this);
        }
        if (auto method = result.Nodes.getNodeAs<CXXMethodDecl>("cxxMethod"))
        {
            addViolation(method, this);
        }
    }

    virtual void setUpMatcher() override
    {
        addMatcher(varDecl(isPrivate(), isStaticDataMember()).bind("field"));
        addMatcher(cxxMethodDecl(isPrivate(), isStatic()).bind("cxxMethod"));
    }
};

static RuleSet rules(new AvoidPrivateStaticMembersRule());
