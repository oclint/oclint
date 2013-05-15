#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

class ShortVariableNameRule : public AbstractASTVisitorRule<ShortVariableNameRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "short variable name";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        int nameLength = varDecl->getNameAsString().size();
        int threshold = RuleConfiguration::intForKey("SHORT_VARIABLE_NAME", 3);
        if (nameLength > 0 && nameLength < threshold)
        {
            addViolation(varDecl, this);
        }
        return true;
    }
};

RuleSet ShortVariableNameRule::rules(new ShortVariableNameRule());
