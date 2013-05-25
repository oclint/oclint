#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

class UnusedLocalVariableRule : public AbstractASTVisitorRule<UnusedLocalVariableRule>
{
private:
    static RuleSet rules;

    bool isInNonTemplateFunction(Decl *varDecl)
    {
        FunctionDecl *decl = dyn_cast<FunctionDecl>(varDecl->getLexicalDeclContext());
        if (decl)
        {
            return decl->getTemplatedKind() == FunctionDecl::TK_NonTemplate;
        }
        return true;
    }

public:
    virtual const string name() const
    {
        return "unused local variable";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        if (varDecl && !varDecl->isUsed() &&
            varDecl->isLocalVarDecl() &&
            !varDecl->isStaticDataMember() &&
            isInNonTemplateFunction(varDecl))
        {
            addViolation(varDecl, this);
        }
        return true;
    }
};

RuleSet UnusedLocalVariableRule::rules(new UnusedLocalVariableRule());
