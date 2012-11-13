#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"

class UnusedLocalVariableRule : public Rule<UnusedLocalVariableRule>
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
            addViolation(varDecl, this);
        }
        return true;
    }
};

RuleSet UnusedLocalVariableRule::rules(new UnusedLocalVariableRule());
