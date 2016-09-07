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
    virtual const std::string name() const override
    {
        return "base class destructor should be virtual or protected";
    }

    virtual const std::string identifier() const override
    {
        return "ProblematicBaseClassDestructor";
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
        return "0.10.2";
    }

    virtual const std::string description() const override
    {
        return "Make base class destructor public and virtual, or protected and nonvirtual";
    }

    virtual const std::string fileName() const override
    {
        return "BaseClassDestructorShouldBeVirtualOrProtectedRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cpp

    class Base
    {
    public:
        ~Base(); // this should be either protected or virtual
    }
    class C : public Base
    {
        virtual ~C();
    }
        )rst";
    }

    virtual const std::string additionalDocument() const override
    {
        return R"rst(
**References:**

Sutter & Alexandrescu (November 2004).
`"C++ Coding Standards: 101 Rules, Guidelines, and Best Practices"
<http://gotw.ca/publications/c++cs.htm>`_. *Addison-Wesley Professional*
        )rst";
    }
#endif

    bool VisitCXXRecordDecl(const CXXRecordDecl* cxxRecordDecl)
    {
        if (cxxRecordDecl != nullptr
            && cxxRecordDecl->hasDefinition()
            && cxxRecordDecl->isPolymorphic())
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

            if (baseClass == nullptr || !baseClass->hasDefinition())
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
