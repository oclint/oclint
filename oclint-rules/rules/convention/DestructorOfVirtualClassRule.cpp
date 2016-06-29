#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace clang;

static bool hasVirtualDestructor(const CXXRecordDecl& cxxRecordDecl)
{
    const CXXDestructorDecl* cxxDestructorDecl = cxxRecordDecl.getDestructor();

    return cxxDestructorDecl != nullptr && cxxDestructorDecl->isVirtual();
}

static std::string getMessageViolation(const CXXRecordDecl& cxxRecordDecl)
{
    const std::string& className = cxxRecordDecl.getNameAsString();
    return "class " + className + " should have a virtual destructor ~" + className + "()";
}

class DestructorOfVirtualClassRule :
    public oclint::AbstractASTVisitorRule<DestructorOfVirtualClassRule>
{
public:
    virtual const std::string name() const override
    {
        return "destructor of virtual class";
    }

    virtual int priority() const override
    {
        return 2;
    }

    virtual const std::string category() const override
    {
        return "convention";
    }

    unsigned int supportedLanguages() const override
    {
        return oclint::LANG_CXX;
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.8";
    }

    virtual const std::string description() const override
    {
        return "This rule enforces the destructor of a virtual class must be virtual.";
    }

    virtual const std::string example() const override
    {
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

    bool VisitCXXRecordDecl(const CXXRecordDecl* cxxRecordDecl)
    {
        if (cxxRecordDecl != nullptr
            && cxxRecordDecl->hasDefinition()
            && cxxRecordDecl->isPolymorphic()
            && !hasVirtualDestructor(*cxxRecordDecl))
        {
            addViolation(cxxRecordDecl, this, getMessageViolation(*cxxRecordDecl));
        }
        return true;
    }
};

static oclint::RuleSet rules(new DestructorOfVirtualClassRule());
