#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

class LongVariableNameRule : public AbstractASTVisitorRule<LongVariableNameRule>
{
private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "long variable name";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        int nameLength = varDecl->getNameAsString().size();
        int threshold = RuleConfiguration::intForKey("LONG_VARIABLE_NAME", 20);
        if (nameLength > threshold)
        {
            addViolation(varDecl, this);
        }

        return true;
    }
};

RuleSet LongVariableNameRule::rules(new LongVariableNameRule());
