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
        if (const IfStmt *constIfStmt = result.Nodes.getNodeAs<IfStmt>("ifStmt"))
        {
            IfStmt *ifStmt = (IfStmt *)constIfStmt;
            addViolation(ifStmt->getCond(), this);
        }
        if (const WhileStmt *constWhileStmt = result.Nodes.getNodeAs<WhileStmt>("whileStmt"))
        {
            WhileStmt *whileStmt = (WhileStmt *)constWhileStmt;
            addViolation(whileStmt->getCond(), this);
        }
        if (const DoStmt *constDoStmt = result.Nodes.getNodeAs<DoStmt>("doStmt"))
        {
            DoStmt *doStmt = (DoStmt *)constDoStmt;
            addViolation(doStmt->getCond(), this);
        }
        if (const ConditionalOperator *constCondOper =
            result.Nodes.getNodeAs<ConditionalOperator>("conditionalOperator"))
        {
            ConditionalOperator *conditionalOperator = (ConditionalOperator *)constCondOper;
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
