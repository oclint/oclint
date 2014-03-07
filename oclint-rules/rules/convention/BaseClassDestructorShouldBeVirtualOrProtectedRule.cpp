#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"

using namespace clang;

static bool hasVirtualOrProtectedDestructor(const CXXRecordDecl& cxxRecordDecl)
{
    const CXXDestructorDecl* cxxDestructorDecl = cxxRecordDecl.getDestructor();

    if (cxxDestructorDecl == nullptr)
    {
        return false;
    }
    return cxxDestructorDecl->isVirtual()
        || cxxDestructorDecl->getAccess() == AS_protected;
}

static std::string getMessageViolation(const CXXRecordDecl& base, const CXXRecordDecl& child)
{
    return "~" + base.getNameAsString() + "() should be protected or virtual"
        " according to class " + child.getNameAsString();
}

/*
 * Base class destructor should be protected or virtual
 * to avoid memory leak by calling base class destructor
 * without calling child class destructor.
 *
 * If base class destructor is virtual, so the correct destructor is called.
 * If base class destructor is protected, it cannot be called from outside.
 *
 * To avoid false positive with 'type traits' class as std::true_type
 * only check parents of polymorphic classes.
 */
class BaseClassDestructorShouldBeVirtualOrProtectedRule :
    public oclint::AbstractASTVisitorRule<BaseClassDestructorShouldBeVirtualOrProtectedRule>
{
public:
    virtual const std::string name() const
    {
        return "Base class destructor should be virtual or protected";
    }

    virtual int priority() const
    {
        return 2;
    }

    unsigned int supportedLanguages() const
    {
        return oclint::LANG_CXX;
    }

    bool VisitCXXRecordDecl(const CXXRecordDecl* cxxRecordDecl)
    {
        if (cxxRecordDecl != nullptr && cxxRecordDecl->isPolymorphic())
        {
            check(*cxxRecordDecl, cxxRecordDecl->bases_begin(), cxxRecordDecl->bases_end());
        }
        return true;
    }

private:
    typedef CXXRecordDecl::base_class_const_iterator base_class_const_iterator;

    void check(const CXXRecordDecl& childClass,
               base_class_const_iterator begin,
               base_class_const_iterator end)
    {
        for (auto it = begin; it != end; ++it)
        {
            if (it->getAccessSpecifier() != AS_public)
            {
                continue;
            }
            const CXXRecordDecl* baseClass = it->getType()->getAsCXXRecordDecl();

            if (baseClass == nullptr)
            {
                continue;
            }
            if (!hasVirtualOrProtectedDestructor(*baseClass))
            {
                addViolation(baseClass, this, getMessageViolation(*baseClass, childClass));
            }
            // Avoid to have duplicated violation from linked parents
            if (!baseClass->isPolymorphic())
            {
                check(childClass, baseClass->bases_begin(), baseClass->bases_end());
            }
        }
    }
};

static oclint::RuleSet rules(new BaseClassDestructorShouldBeVirtualOrProtectedRule());
