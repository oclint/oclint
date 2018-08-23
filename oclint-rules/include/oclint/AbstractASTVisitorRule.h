#ifndef OCLINT_ABSTRACTASTVISITORRULE_H
#define OCLINT_ABSTRACTASTVISITORRULE_H

#include <clang/AST/RecursiveASTVisitor.h>
#include "oclint/AbstractASTRuleBase.h"

namespace oclint
{

template<typename T>
class AbstractASTVisitorRule : public AbstractASTRuleBase, protected clang::RecursiveASTVisitor<T>
{
    friend class clang::RecursiveASTVisitor<T>;
private:
    virtual bool shouldTraverseFile(clang::SourceLocation startLocation)
    {
        if(startLocation.isInvalid())
        {
            return false;
        }

        clang::SourceManager *sourceManager = &_carrier->getSourceManager();

        /* If the decl is in the current file, traverse it */
        if (sourceManager->getMainFileID() == sourceManager->getFileID(startLocation))
        {
            return true;
        }

        /* If the user want to lint header files and decl is in the associated header file, traverse it */
        if (!visitHeaders())
        {
            return false;
        }

        llvm::StringRef sourceFileName = sourceManager->getFilename(startLocation);
        return isAnalyzeFileHeadFile(sourceFileName);
    }

    /* Check if decl is in the associated header file */
    virtual bool isAnalyzeFileHeadFile(llvm::StringRef fileNameStrRef) {
        if (fileNameStrRef.empty())
        {
            return false;
        }

        std::string currentAnalyzeFilePath = _carrier->getMainFilePath().c_str();
        std::string subString = currentAnalyzeFilePath.erase(currentAnalyzeFilePath.length() - 1);
        return fileNameStrRef.str().find(subString) == 0;
    }

protected:
    virtual bool visitHeaders()
    {
        return false;
    }

    virtual void apply()
    {
        if (!isLanguageSupported())
        {
            return;
        }

        setUp();
        clang::DeclContext *decl = _carrier->getTranslationUnitDecl();
        for (clang::DeclContext::decl_iterator it = decl->decls_begin(), declEnd = decl->decls_end();
            it != declEnd; ++it)
        {
            clang::SourceLocation startLocation = (*it)->getLocStart();
            if(shouldTraverseFile(startLocation)){
                (void) /* explicitly ignore the return of this function */
                clang::RecursiveASTVisitor<T>::TraverseDecl(*it);
            }
        }
        tearDown();
    }

public:
    virtual ~AbstractASTVisitorRule() {}

    virtual void setUp() {}
    virtual void tearDown() {}
};

} // end namespace oclint

#endif
