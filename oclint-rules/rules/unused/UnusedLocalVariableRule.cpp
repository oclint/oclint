#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class UnusedLocalVariableRule : public AbstractASTVisitorRule<UnusedLocalVariableRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "unused local variable";
    }

    virtual const int priority() const
    {
        return 2;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        if (varDecl && !varDecl->isUsed() &&
            varDecl->isLocalVarDecl() && !varDecl->isStaticDataMember())
        {
            _carrier->addViolation(varDecl, this);
        }
        return true;
    }
};

RuleSet UnusedLocalVariableRule::rules(new UnusedLocalVariableRule());
