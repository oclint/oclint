#include <sstream>
#include <set>
#include <string>
#include <list>
#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/RuleConfiguration.h"

using namespace std;
using namespace clang;
using namespace oclint;

class UnusedLocalVariableRule : public AbstractASTVisitorRule<UnusedLocalVariableRule>
{
private:
    set<string> skippedTypes;

    void splitClasses(const string & input) {
        int startPos = 0;
        // Take the string end into account, to avoid special string end handling
        for (int i = 0; i < input.length() + 1; ++i)
        {
            for (auto const & curSep : { ',', ';', ' ', '\t', '\0' })
            {
                if (input[i] == curSep)
                {
                    if (startPos != i)
                    {
                        const auto curClassName = input.substr(startPos, i-startPos);
                        skippedTypes.insert(curClassName);
                    }
                    startPos = i + 1;
                }
            }
        }
    }

public:
    UnusedLocalVariableRule()
    {
        string defKeys = RuleConfiguration::stringForKey("RAII_DEFAULT_CLASSES",
            "std::lock_guard, std::unique_lock");
        string cusKeys = RuleConfiguration::stringForKey("RAII_CUSTOM_CLASSES", "");

        resetSkippedTypes({ defKeys, cusKeys });
    }
protected:
    /*
     * Replace the list of known splitting classes and assign new values based on the parameter
     *
     * The main purpose of this class is to allow tests with different rule configurations
     * In the regular operation mode, this part should be only triggered within the ctor
     *
     * @param newIgnoreTypes The new types to insert accordingly
     * @throw std::invalid_argument, in case that the given input contains a failure
     */
    void resetSkippedTypes(const std::list<string> & newIgnoreTypes)
    {
        skippedTypes.clear();
        for (auto const & curKeys : newIgnoreTypes)
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

    bool isRAIIClass(VarDecl *varDecl)
    {
        // Getting ride of any template definition which might follow
        auto varTypeName = varDecl->getType().getCanonicalType().getAsString();
        auto const templPos = varTypeName.find('<');
        if (templPos != string::npos)
        {
            varTypeName = varTypeName.substr(0,templPos);
        }

        // Remove of the qualifiers, to get ride of class/struct/... definition parts
        auto const lastSpacePos = varTypeName.rfind(' ');
        if (lastSpacePos != string::npos)
        {
            varTypeName = varTypeName.substr(lastSpacePos + 1);
        }

        return skippedTypes.find(varTypeName) != skippedTypes.end();
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

    virtual const string category() const override
    {
        return "unused";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.4";
    }

    virtual const std::string description() const override
    {
        return "This rule detects local variables that are declared, but not used.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    int example(int a)
    {
        int i;          // variable i is declared, but not used
        return 0;
    }
    )rst";
    }

    virtual bool enableSuppress() const override
    {
        return true;
    }
#endif

    bool VisitVarDecl(VarDecl *varDecl)
    {
        if (isUnusedLocalVariable(varDecl) && !isRAIIClass(varDecl))
        {
            addViolation(varDecl, this, description(varDecl->getNameAsString()));
        }

        return true;
    }
};

static RuleSet rules(new UnusedLocalVariableRule());
