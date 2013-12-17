#include <sstream>
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class UnusedLocalVariableRule : public AbstractASTVisitorRule<UnusedLocalVariableRule>
{
private:
    static RuleSet rules;

    bool isInNonTemplateFunction(Decl *varDecl)
    {
        FunctionDecl *decl = dyn_cast_or_null<FunctionDecl>(varDecl->getLexicalDeclContext());
        if (decl)
        {
            return decl->getTemplatedKind() == FunctionDecl::TK_NonTemplate;
        }
        return true;
    }

    bool isLocalVariable(VarDecl *varDecl)
    {
        return varDecl->isLocalVarDecl();
    }

    bool isUnused(VarDecl *varDecl)
    {
        return !varDecl->isUsed() && !varDecl.isReferenced();
    }

    bool hasName(VarDecl *varDecl)
    {
        return varDecl->getNameAsString() != "";
    }

    bool isNonStaticVariable(VarDecl *varDecl)
    {
        return !varDecl->isStaticDataMember();
    }

    bool isUnusedLocalVariable(VarDecl *varDecl)
    {
        return isUnused(varDecl) &&
            isLocalVariable(varDecl) &&
            hasName(varDecl) &&
            isNonStaticVariable(varDecl) &&
            isInNonTemplateFunction(varDecl);
    }

    string description(const string& unusedVariableName)
    {
        ostringstream stream;
        stream << "The local variable '" << unusedVariableName << "' is unused.";
        return stream.str();
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
        if (isUnusedLocalVariable(varDecl))
        {
            addViolation(varDecl, this, description(varDecl->getNameAsString()));
        }
        return true;
    }
};

RuleSet UnusedLocalVariableRule::rules(new UnusedLocalVariableRule());
