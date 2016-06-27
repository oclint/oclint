#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;
using namespace oclint;

class BitwiseOperatorInConditionalRule : public AbstractASTMatcherRule
{
public:
    virtual const string name() const override
    {
        return "bitwise operator in conditional";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "basic";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "Checks for bitwise operations in conditionals. Although being written on purpose "
            "in some rare cases, bitwise operations are considered to be too \"smart\". "
            "Smart code is not easy to understand.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a, int b)
    {
        if (a | b)
        {
        }
        if (a & b)
        {
        }
    }
    )rst";
    }
#endif

    virtual void callback(const MatchFinder::MatchResult& result) override
    {
        const IfStmt *ifStmt = result.Nodes.getNodeAs<IfStmt>("ifStmt");
        if (ifStmt)
        {
            addViolation(ifStmt->getCond(), this);
        }

        const WhileStmt *whileStmt = result.Nodes.getNodeAs<WhileStmt>("whileStmt");
        if (whileStmt)
        {
            addViolation(whileStmt->getCond(), this);
        }

        const DoStmt *doStmt = result.Nodes.getNodeAs<DoStmt>("doStmt");
        if (doStmt)
        {
            addViolation(doStmt->getCond(), this);
        }

        const ConditionalOperator *conditionalOperator =
            result.Nodes.getNodeAs<ConditionalOperator>("conditionalOperator");
        if (conditionalOperator)
        {
            addViolation(conditionalOperator->getCond(), this);
        }
    }

    virtual void setUpMatcher() override
    {
        StatementMatcher biOpMatcher = binaryOperator(
            anyOf(hasOperatorName("&"), hasOperatorName("|"), hasOperatorName("^")));

        addMatcher(
            ifStmt(anyOf(
                hasCondition(biOpMatcher),
                hasCondition(expr(hasDescendant(biOpMatcher)))))
            .bind("ifStmt"));
        addMatcher(
            whileStmt(anyOf(
                hasCondition(biOpMatcher),
                hasCondition(expr(hasDescendant(biOpMatcher)))))
            .bind("whileStmt"));
        addMatcher(
            doStmt(anyOf(
                hasCondition(biOpMatcher),
                hasCondition(expr(hasDescendant(biOpMatcher)))))
            .bind("doStmt"));
        addMatcher(
            conditionalOperator(anyOf(
                hasCondition(biOpMatcher),
                hasCondition(expr(hasDescendant(biOpMatcher)))))
            .bind("conditionalOperator"));
    }

};

static RuleSet rules(new BitwiseOperatorInConditionalRule());
