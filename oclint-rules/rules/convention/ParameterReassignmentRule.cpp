#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

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
            if (binaryOperator->getOpcode() == BO_Assign &&
                isa<DeclRefExpr>(binaryOperator->getLHS()))
            {
                DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(binaryOperator->getLHS());
                for (int index = 0; index < _names.size(); index++)
                {
                    if (declRefExpr->getFoundDecl()->getNameAsString() == _names.at(index))
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
    static RuleSet rules;

    void locateParamNames(Decl *decl, vector<string> &names)
    {
        if (names.size() > 0)
        {
            BinaryOperatorAnalyzer biOpAnalyzer;
            vector<BinaryOperator*> binaryOperators = biOpAnalyzer.analyze(decl, names);
            for (int index = 0; index < binaryOperators.size(); index++)
            {
                addViolation(binaryOperators.at(index), this);
            }
        }
    }

public:
    virtual const string name() const
    {
        return "parameter reassignment";
    }

    virtual int priority() const
    {
        return 3;
    }

    bool VisitFunctionDecl(FunctionDecl *decl)
    {
        vector<string> names;
        for (int i = 0; i < decl->getNumParams(); i++)
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

RuleSet ParameterReassignmentRule::rules(new ParameterReassignmentRule());
