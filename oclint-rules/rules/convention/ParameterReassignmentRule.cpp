#include "oclint/AbstractAstVisitorRule.h"
#include "oclint/RuleSet.h"

class ParameterReassignmentRule : public AbstractAstVisitorRule<ParameterReassignmentRule>
{
    class ParametersNameFinder : public RecursiveASTVisitor<ParametersNameFinder>
    {
    private:
        vector<string> _names;

    public:
        vector<string> find(Decl *decl)
        {
            _names.clear();
            TraverseDecl(decl);
            return _names;
        }

        bool VisitParmVarDecl(ParmVarDecl *parmVarDecl)
        {
            if (parmVarDecl->getNameAsString() != "")
            {
                _names.push_back(parmVarDecl->getNameAsString());
            }
            return true;
        }
    };

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
            TraverseDecl(decl);
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

    void applyDecl(Decl *decl)
    {
        ParametersNameFinder parametersNameFinder;
        vector<string> names = parametersNameFinder.find(decl);
        if (names.size() > 0)
        {
            BinaryOperatorAnalyzer binaryOperatorAnalyzer;
            vector<BinaryOperator*> binaryOperators = binaryOperatorAnalyzer.analyze(decl, names);
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

    virtual const int priority() const
    {
        return 3;
    }

    bool VisitFunctionDecl(FunctionDecl *decl)
    {
        applyDecl(decl);
        return true;
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl *decl)
    {
        applyDecl(decl);
        return true;
    }
};

RuleSet ParameterReassignmentRule::rules(new ParameterReassignmentRule());
