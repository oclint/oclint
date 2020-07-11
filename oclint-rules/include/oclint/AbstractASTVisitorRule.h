#ifndef OCLINT_ABSTRACTASTVISITORRULE_H
#define OCLINT_ABSTRACTASTVISITORRULE_H

#include <clang/AST/RecursiveASTVisitor.h>

#include "oclint/AbstractASTRuleBase.h"

namespace oclint {

template<typename T>
class AbstractASTVisitorRule : public AbstractASTRuleBase, protected clang::RecursiveASTVisitor<T> {
    friend class clang::RecursiveASTVisitor<T>;
protected:
    virtual void apply() {
        if (isCUDASourceFile()) {
            applyCUDA();
        } else {
            applyC();
        }
    }

    void applyC() {
        if (!isLanguageSupported() || !(supportedCUDAFunctionAttrs() & CUDA_HOST)) { return; }

        setUp();
        clang::DeclContext *tu = getTranslationUnit();
        for (clang::DeclContext::decl_iterator it = tu->decls_begin(), declEnd = tu->decls_end();
            it != declEnd; ++it)
        {
            if (isValidDecl(*it)) {
                traverse(*it);
            }
        }
        tearDown();
    }

    void applyCUDA() {
        setUp();
        clang::DeclContext *tu = getTranslationUnit();
        for (clang::DeclContext::decl_iterator it = tu->decls_begin(), declEnd = tu->decls_end();
            it != declEnd; ++it)
        {
            applyCUDADecl(*it);
        }
        tearDown();
    }

    void applyCUDADecl(clang::Decl *decl) {
        if (!isValidDecl(decl)) { return; }

        if (clang::isa<clang::FunctionDecl>(decl)) {
            auto funcDecl = clang::cast<clang::FunctionDecl>(decl);
            if (funcDecl->hasAttr<clang::CUDAGlobalAttr>()) {
                if (supportedCUDAFunctionAttrs() & CUDA_GLOBAL) {
                    traverse(decl);
                }
            } else if (supportedCUDAFunctionAttrs() & CUDA_HOST) {
                traverse(decl);
            }
        } else {
            traverse(decl);
        }
    }

    void traverse(clang::Decl *decl) {
        (void) /* explicitly ignore the return of this function */
            clang::RecursiveASTVisitor<T>::TraverseDecl(decl);
    }

private:
    clang::DeclContext *getTranslationUnit()
    {
        return _carrier->getTranslationUnitDecl();
    }

    bool isValidDecl(clang::Decl *decl)
    {
        clang::SourceManager *sourceManager = &_carrier->getSourceManager();
        clang::SourceLocation startLocation = decl->getBeginLoc();
        return startLocation.isValid() && sourceManager->isInMainFile(startLocation);
    }

public:
    virtual ~AbstractASTVisitorRule() {}

    virtual void setUp() {}
    virtual void tearDown() {}
};

} // end namespace oclint

#endif
