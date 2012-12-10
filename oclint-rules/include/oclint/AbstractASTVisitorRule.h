#ifndef OCLINT_RULE_H
#define OCLINT_RULE_H

#include <string>

#include <clang/AST/ASTContext.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include "oclint/RuleBase.h"

using namespace std;
using namespace clang;

template<typename T>
class AbstractASTVisitorRule : public RuleBase, public RecursiveASTVisitor<T>
{
protected:
    virtual void apply()
    {
        setUp();
        SourceManager *sourceManager = &_astContext->getSourceManager();
        DeclContext *decl = _astContext->getTranslationUnitDecl();
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

public:
    virtual ~AbstractASTVisitorRule() {}

    virtual void setUp() {}
    virtual void tearDown() {}
};

#endif
