#ifndef OCLINT_ABSTRACTASTVISITORRULE_H
#define OCLINT_ABSTRACTASTVISITORRULE_H

#include <clang/AST/RecursiveASTVisitor.h>

#include "oclint/AbstractASTRuleBase.h"

template<typename T>
class AbstractASTVisitorRule : public AbstractASTRuleBase, public RecursiveASTVisitor<T>
{
protected:
    virtual void apply()
    {
        setUp();
        SourceManager *sourceManager = &_carrier->astContext()->getSourceManager();
        DeclContext *decl = _carrier->astContext()->getTranslationUnitDecl();
        for (DeclContext::decl_iterator it = decl->decls_begin(), declEnd = decl->decls_end();
            it != declEnd; ++it)
        {
            SourceLocation startLocation = (*it)->getLocStart();
            if (startLocation.isValid() &&
                sourceManager->getMainFileID() == sourceManager->getFileID(startLocation))
            {
                RecursiveASTVisitor<T>::TraverseDecl(*it);
            }
        }
        tearDown();
    }


    void addViolation(SourceLocation startLocation,
        SourceLocation endLocation, RuleBase *rule, const string& message = "")
    {
        SourceManager *sourceManager = &_carrier->astContext()->getSourceManager();
        StringRef filename = sourceManager->getFilename(startLocation);
        _carrier->addViolation(filename.str(),
            sourceManager->getPresumedLineNumber(startLocation),
            sourceManager->getPresumedColumnNumber(startLocation),
            sourceManager->getPresumedLineNumber(endLocation),
            sourceManager->getPresumedColumnNumber(endLocation),
            rule,
            message);
    }

    void addViolation(Decl *decl, RuleBase *rule, const string& message = "")
    {
        addViolation(decl->getLocStart(), decl->getLocEnd(), rule, message);
    }

    void addViolation(Stmt *stmt, RuleBase *rule, const string& message = "")
    {
        addViolation(stmt->getLocStart(), stmt->getLocEnd(), rule, message);
    }

public:
    virtual ~AbstractASTVisitorRule() {}

    virtual void setUp() {}
    virtual void tearDown() {}
};

#endif
