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

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.10.1";
    }

    virtual const std::string description() const override
    {
        return "Giving virtual functions default argument initializers tends to "
            "defeat polymorphism and introduce unnecessary complexity into a class hierarchy.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    class Foo
    {
    public:
        virtual ~Foo();
        virtual void a(int b = 3);
        // ...
    };

    class Bar : public Foo
    {
    public:
        void a(int b);
        // ...
    };

    Bar *bar = new Bar;
    Foo *foo = bar;
    foo->a();   // default of 3
    bar->a();   // compile time error!
        )rst";
    }
#endif

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
