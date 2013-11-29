#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace clang;

static bool hasVirtualDestructor(const CXXRecordDecl& cxxRecordDecl)
{
    const CXXDestructorDecl* cxxDestructorDecl = cxxRecordDecl.getDestructor();

    return cxxDestructorDecl != nullptr && cxxDestructorDecl->isVirtual();
}

class DestructorOfVirtualClassRule : public oclint::AbstractASTVisitorRule<DestructorOfVirtualClassRule>
{
public:
    virtual const std::string name() const
    {
        return "destructor of virtual class";
    }

    virtual int priority() const
    {
        return 2;
    }

    unsigned int supportedLanguages() const
    {
        return LANG_CXX;
    }

    bool VisitCXXRecordDecl(const CXXRecordDecl* cxxRecordDecl)
    {
        if (cxxRecordDecl != nullptr
            && cxxRecordDecl->isPolymorphic()
            && !hasVirtualDestructor(*cxxRecordDecl))
        {
            addViolation(cxxRecordDecl, this, getMessageViolation(*cxxRecordDecl));
        }
        return true;
    }

private:
    static std::string getMessageViolation(const CXXRecordDecl& cxxRecordDecl)
    {
        return "~" + cxxRecordDecl.getNameAsString() + "() should be virtual";
    }

private:
    static oclint::RuleSet rules;
};

oclint::RuleSet DestructorOfVirtualClassRule::rules(new DestructorOfVirtualClassRule());
