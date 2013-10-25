// Finds assignments to ivars outside of their associated getter or setter methods

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

#include <algorithm>
using namespace std;
using namespace clang;
using namespace oclint;


// Helper functions
static bool IsUnderscore(char c) {
    return c == '_';
}

static string RemoveUnderscores(string s) {
    s.erase(remove_if(s.begin(), s.end(), &::IsUnderscore), s.end());
    return s;
}

static string Capitalize(string s) {
    transform(s.begin(), s.begin() + 1, s.begin(), ::toupper);
    return s;
}

// Traversal for ivar accesses
class ContainsIvarFetch : public RecursiveASTVisitor<ContainsIvarFetch>
{
public: 
    // Location to save found ivar accesses
    vector<ObjCIvarRefExpr*> instances;

    bool VisitObjCIvarRefExpr(ObjCIvarRefExpr* ivarRef) {
        this->instances.push_back(ivarRef);
        return true;
    }

};


// Look for assignment operators. Then collect ivar accesses inside
class ContainsBinaryOperatorWithIvarAssignment : public RecursiveASTVisitor<ContainsBinaryOperatorWithIvarAssignment>
{
private:
    bool IsAssignOperator(BinaryOperatorKind kind) {
        switch(kind) {
            case BO_Assign:
            case BO_MulAssign:
            case BO_DivAssign:
            case BO_RemAssign:
            case BO_AddAssign:
            case BO_SubAssign:
            case BO_ShlAssign:
            case BO_ShrAssign:
            case BO_AndAssign:
            case BO_XorAssign:
            case BO_OrAssign:
                return true;
            default:
                return false;
        }
    }

public:
    // Location to save found ivar accesses
    vector<ObjCIvarRefExpr*> instances;

    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        // Found an operator
        Expr *leftExpr = binaryOperator->getLHS();
        if (IsAssignOperator(binaryOperator->getOpcode()))
        {
            // So check for ivar references in the assignment part
            ContainsIvarFetch checker;
            checker.TraverseStmt(leftExpr);
            this->instances.insert(this->instances.end(), checker.instances.begin(), checker.instances.end());
        }

        return true;
    }
};
    


// Main rule implementation.
// Looks for methods then finds accesses in each method and sees if they're okay
class ObjCAssignIvarOutsideAccessorsRule:
    public AbstractASTVisitorRule<ObjCAssignIvarOutsideAccessorsRule>
{

private:
    static RuleSet rules;

public:
    virtual const string name() const
    {
        return "ivar assignment outside accessors or init";
    }

    virtual int priority() const
    {
        return 2;
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl* decl) {
        // Save the method name
        string selectorName = decl->getSelector().getAsString();

        // Then traverse down with a separate checker
        // to find the actual accesses
        ContainsBinaryOperatorWithIvarAssignment checker;
        checker.TraverseDecl(decl);

        // Now go through all the ivar accesses and see if they match the method name as a getter or setter
        for (vector<ObjCIvarRefExpr*>::iterator it = checker.instances.begin(); it != checker.instances.end(); ++it) {
            ObjCIvarRefExpr* access = *it;
            // ivarName is _foo or foo or foo_
            string ivarName = access->getDecl()->getNameAsString();
            // getterName is foo. Note this won't work for properties that reassign getter= or setter=
            string getterName = RemoveUnderscores(ivarName);
            // setterName is setFoo
            string setterName = "set" + Capitalize(getterName) + ":";
            if((selectorName != getterName && selectorName != setterName) && selectorName.substr(0, 4) != "init") {
                this->addViolation(*it, this);
            }
        }
        return true;
    }

};

// Instantiate the rule
RuleSet ObjCAssignIvarOutsideAccessorsRule::rules(new ObjCAssignIvarOutsideAccessorsRule());

