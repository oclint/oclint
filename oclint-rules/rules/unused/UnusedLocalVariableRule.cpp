#include <sstream>
#include <set>
#include <string>
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/RuleConfiguration.h"

using namespace std;
using namespace clang;
using namespace oclint;

static set<string> skippedTypes;

static void handleClassName(const string & className)
{
    //TODO verify the input
    skippedTypes.insert(className);
}

static void splitClasses(const string & input) {
    int startPos = 0;
	// Take the string end into account, to avoid special string end handling
    for (int i=0; i<input.length()+1; ++i)
    {
        for (auto const & curSep : { ',', ';', ' ', '\t', '\0' })
        {
            if (input[i] == curSep)
            {
                if (startPos != i)
                {
                    handleClassName(input.substr(startPos, i-startPos));
                }
                startPos = i+1;
            }
        }
    }
}

class UnusedLocalVariableRule : public AbstractASTVisitorRule<UnusedLocalVariableRule>
{
public:
    UnusedLocalVariableRule()
    {
        string defKeys = RuleConfiguration::stringForKey("RAII_DEFAULT_CLASSES",
            "std::lock_guard, std::unique_lock");
        string cusKeys = RuleConfiguration::stringForKey("RAII_CUSTOM_CLASSES", "");

        for (auto const & curKeys : { defKeys, cusKeys })
        {
            if (!curKeys.empty())
            {
                splitClasses(curKeys);
            }
        }
    }
private:
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
        return !varDecl->isUsed() && !varDecl->isReferenced();
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
    virtual const string name() const override
    {
        return "unused local variable";
    }

    virtual int priority() const override
    {
        return 3;
    }

    bool VisitVarDecl(VarDecl *varDecl)
    {
        if (!isUnusedLocalVariable(varDecl))
        {
			return true;
		}

        // Getting ride of any template definition which might follow
        auto varTypeName = varDecl->getType().getCanonicalType().getAsString();
        auto const templPos = varTypeName.find('<');
        if (templPos != string::npos) {
            varTypeName = varTypeName.substr(0,templPos);
        }
        // Remove of the qualifiers, to get ride of class/struct/... definition parts
        {
            auto const lastSpacePos = varTypeName.rfind(' ');
            if (lastSpacePos != string::npos) {
                varTypeName = varTypeName.substr(lastSpacePos + 1);
            }
        }
        if (skippedTypes.find(varTypeName) == skippedTypes.end())
        {
            addViolation(varDecl, this, description(varDecl->getNameAsString()));
        }
        return true;
    }
};

static RuleSet rules(new UnusedLocalVariableRule());
