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
                (void) /* explicitly ignore the return of this function */
                    RecursiveASTVisitor<T>::TraverseDecl(*it);
            }
        }
        tearDown();
    }

public:
    virtual ~AbstractASTVisitorRule() {}

    virtual void setUp() {}
    virtual void tearDown() {}
};

#endif
