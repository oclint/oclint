#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

class ParameterReassignmentRule : public AbstractASTVisitorRule<ParameterReassignmentRule>
{
    class BinaryOperatorAnalyzer : public RecursiveASTVisitor<BinaryOperatorAnalyzer>
    {
    private:
        vector<string> _names;
        vector<BinaryOperator*> _binaryOperators;

    public:
        vector<BinaryOperator*> analyze(Decl *decl, vector<string> &names)
        {
            _names = names;
            _binaryOperators.clear();
            (void) /* explicitly ignore the return of this function */ TraverseDecl(decl);
            return _binaryOperators;
        }

        bool VisitBinaryOperator(BinaryOperator *binaryOperator)
        {
            Expr *leftExpr = binaryOperator->getLHS();
            if (binaryOperator->getOpcode() == BO_Assign && leftExpr && isa<DeclRefExpr>(leftExpr))
            {
                DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(leftExpr);
                for (const auto& name : _names)
                {
                    if (declRefExpr->getFoundDecl()->getNameAsString() == name)
                    {
                        _binaryOperators.push_back(binaryOperator);
                        break;
                    }
                }
            }
            return true;
        }
    };

private:
    void locateParamNames(Decl *decl, vector<string> &names)
    {
        if (!names.empty())
        {
            BinaryOperatorAnalyzer biOpAnalyzer;
            vector<BinaryOperator*> binaryOperators = biOpAnalyzer.analyze(decl, names);
            for (const auto& binaryOperator : binaryOperators)
            {
                addViolation(binaryOperator, this);
            }
        }
    }

public:
    virtual const string name() const override
    {
        return "parameter reassignment";
    }

    virtual int priority() const override
    {
        return 3;
    }

    virtual const string category() const override
    {
        return "convention";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.6";
    }

    virtual const std::string description() const override
    {
        return "Reassigning values to parameters is very problematic in most cases.";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    void example(int a)
    {
        if (a < 0)
        {
            a = 0; // reassign parameter a to 0
        }
    }
        )rst";
    }
#endif

    bool VisitFunctionDecl(FunctionDecl *decl)
    {
        vector<string> names;
        for (size_t i = 0; i != decl->getNumParams(); ++i)
        {
            ParmVarDecl *parmVarDecl = decl->getParamDecl(i);
            if (parmVarDecl->getNameAsString() != "")
            {
                names.push_back(parmVarDecl->getNameAsString());
            }
        }
        locateParamNames(decl, names);

        return true;
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl *decl)
    {
        vector<string> names;
        for (ObjCMethodDecl::param_iterator param = decl->param_begin(),
            paramEnd = decl->param_end(); param != paramEnd; param++)
        {
            ParmVarDecl *parmVarDecl = *param;
            if (parmVarDecl->getNameAsString() != "")
            {
                names.push_back(parmVarDecl->getNameAsString());
            }
        }
        locateParamNames(decl, names);

        return true;
    }
};

static RuleSet rules(new ParameterReassignmentRule());
