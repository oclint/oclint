#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;


// Traversal for ivar accesses
class ContainsIvarFetch : public RecursiveASTVisitor<ContainsIvarFetch>
{
public:
    // Location to save found ivar accesses
    vector<ObjCIvarRefExpr*> _instances;

    bool VisitObjCIvarRefExpr(ObjCIvarRefExpr* ivarRef) {
        _instances.push_back(ivarRef);
        return true;
    }

    vector<ObjCIvarRefExpr*>& getInstances() {
        return _instances;
    }

};


// Look for assignment operators. Then collect ivar accesses inside
class ContainsBinaryOperatorWithIvarAssignment :
    public RecursiveASTVisitor<ContainsBinaryOperatorWithIvarAssignment>
{
private:
    // Location to save found ivar accesses
    vector<ObjCIvarRefExpr*> _instances;

public:
    bool VisitBinaryOperator(BinaryOperator *binaryOperator)
    {
        // Found an operator
        Expr *leftExpr = binaryOperator->getLHS();
        if (binaryOperator->isAssignmentOp())
        {
            // So check for ivar references in the assignment part
            ContainsIvarFetch checker;
            checker.TraverseStmt(leftExpr);
            _instances.insert(
                _instances.end(),
                checker.getInstances().begin(),
                checker.getInstances().end()
            );
        }

        return true;
    }

    vector<ObjCIvarRefExpr*>& getInstances() {
        return _instances;
    }
};



// Main rule implementation.
// Looks for methods then finds accesses in each method and sees if they're okay
class ObjCAssignIvarOutsideAccessorsRule:
    public AbstractASTVisitorRule<ObjCAssignIvarOutsideAccessorsRule>
{
public:
    virtual const string name() const override
    {
        return "ivar assignment outside accessors or init";
    }

    virtual const string identifier() const override
    {
        return "AssignIvarOutsideAccessors";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const string category() const override
    {
        return "convention";
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.8";
    }

    virtual const std::string description() const override
    {
        return "This rule prevents assigning an ivar outside of "
            "getters, setters, and ``init`` method.";
    }

    virtual const std::string fileName() const override
    {
        return "ObjCAssignIvarOutsideAccessorsRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: objective-c

    @interface Foo : NSObject
    {
        int _bar;
    }
    @property (assign, nonatomic) int bar;
    @end
    @implementation Foo
    @synthesize bar = _bar;
    - (void)doSomething {
        _bar = 3; // access _bar outside its getter, setter or init
    }
    @end
        )rst";
    }
#endif

    bool VisitObjCMethodDecl(ObjCMethodDecl* decl) {
        // Save the method name
        string selectorName = decl->getSelector().getAsString();

        // Then traverse down with a separate checker
        // to find the actual accesses
        ContainsBinaryOperatorWithIvarAssignment checker;
        checker.TraverseDecl(decl);

        // Now go through all the ivar accesses and see
        // if they match the method name as a getter, setter, or init method
        for (auto& access : checker.getInstances())
        {
            // ivarName is _foo or foo or foo_
            string ivarName = access->getDecl()->getNameAsString();
            // getterName is foo. Note this won't work for
            // properties that reassign getter= or setter=
            string getterName = removeUnderscores(ivarName);
            // setterName is setFoo
            string setterName = "set" + capitalizeFirstLetter(getterName) + ":";
            if((selectorName != getterName
              && selectorName != setterName)
              && selectorName.substr(0, 4) != "init") {
                addViolation(access, this);
            }
        }
        return true;
    }

};

// Instantiate the rule
static RuleSet rules(new ObjCAssignIvarOutsideAccessorsRule());

