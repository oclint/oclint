#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

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
            string description = "Variable name with " + toString<int>(nameLength) +
                " characters is longer than the threshold of " + toString<int>(threshold);
            addViolation(varDecl, this, description);
        }

        return true;
    }
};

RuleSet LongVariableNameRule::rules(new LongVariableNameRule());
