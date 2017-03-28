#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

static bool hasVirtualDestructor(const CXXRecordDecl &cxxRecordDecl) {
    const CXXDestructorDecl *cxxDestructorDecl = cxxRecordDecl.getDestructor();

    return cxxDestructorDecl != nullptr && cxxDestructorDecl->isVirtual();
}

static string getMessageViolation(const CXXRecordDecl &cxxRecordDecl) {
    const string &className = cxxRecordDecl.getNameAsString();
    return "class " + className + " should have a virtual destructor ~" + className + "()";
}

class DestructorOfVirtualClassRule :
    public AbstractASTVisitorRule<DestructorOfVirtualClassRule> {
    public:
        virtual const string name() const override {
            return "destructor of virtual class";
        }

        virtual int priority() const override {
            return 2;
        }

        virtual const string category() const override {
            return "convention";
        }

        unsigned int supportedLanguages() const override {
            return LANG_CXX;
        }

        #ifdef DOCGEN
        virtual const string since() const override {
            return "0.8";
        }

        virtual const string description() const override {
            return "This rule enforces the destructor of a virtual class must be virtual.";
        }

        virtual const string example() const override {
            return R"rst(
.. code-block:: cpp

    class Base { // class Base should have a virtual destructor ~Base()
        public: virtual void f();
    };
    class Child : public Base {
        public: ~Child();  // destructor ~Child() should be virtual
    };
        )rst";
        }
        #endif

        bool VisitCXXRecordDecl(const CXXRecordDecl *cxxRecordDecl) {
            if (cxxRecordDecl != nullptr
                    && cxxRecordDecl->hasDefinition()
                    && cxxRecordDecl->isPolymorphic()
                    && !hasVirtualDestructor(*cxxRecordDecl)) {
                addViolation(cxxRecordDecl, this, getMessageViolation(*cxxRecordDecl));
            }
            return true;
        }
};

static RuleSet rules(new DestructorOfVirtualClassRule());
