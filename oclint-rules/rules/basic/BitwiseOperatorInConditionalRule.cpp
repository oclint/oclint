#include "oclint/AbstractASTMatcherRule.h"
#include "oclint/RuleSet.h"

class BitwiseOperatorInConditionalRule : public AbstractASTMatcherRule
{

private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "bitwise operator in conditional";
    }

    virtual const int priority() const
    {
        return 2;
    }

    virtual void callback(const MatchFinder::MatchResult &result)
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

    virtual void setUpMatcher()
    {
        StatementMatcher binaryOperatorMatcher = binaryOperator(
            anyOf(hasOperatorName("&"), hasOperatorName("|"), hasOperatorName("^")));

        addMatcher(
            ifStmt(anyOf(
                hasCondition(binaryOperatorMatcher),
                hasCondition(expr(hasDescendant(binaryOperatorMatcher)))))
            .bind("ifStmt"));
        addMatcher(
            whileStmt(anyOf(
                hasCondition(binaryOperatorMatcher),
                hasCondition(expr(hasDescendant(binaryOperatorMatcher)))))
            .bind("whileStmt"));
        addMatcher(
            doStmt(anyOf(
                hasCondition(binaryOperatorMatcher),
                hasCondition(expr(hasDescendant(binaryOperatorMatcher)))))
            .bind("doStmt"));
        addMatcher(
            conditionalOperator(anyOf(
                hasCondition(binaryOperatorMatcher),
                hasCondition(expr(hasDescendant(binaryOperatorMatcher)))))
            .bind("conditionalOperator"));
    }

};

RuleSet BitwiseOperatorInConditionalRule::rules(new BitwiseOperatorInConditionalRule());
