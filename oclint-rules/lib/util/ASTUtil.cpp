#include "oclint/util/ASTUtil.h"

bool isObjCMethodDeclaredInSuperClass(clang::ObjCMethodDecl *decl)
{
    if (decl && !isObjCMethodDeclLocatedInInterfaceContainer(decl))
    {
        clang::Selector selector = decl->getSelector();
        clang::ObjCInterfaceDecl *interfaceDecl = decl->getClassInterface();
        if (interfaceDecl)
        {
            clang::ObjCInterfaceDecl *superInterface = interfaceDecl->getSuperClass();
            return superInterface && superInterface->lookupInstanceMethod(selector);
        }
    }
    return false;
}

bool isObjCMethodDeclaredInProtocol(clang::ObjCMethodDecl *decl)
{
    if (!decl || isObjCMethodDeclLocatedInInterfaceContainer(decl))
    {
        return false;
    }
    
    clang::Selector selector = decl->getSelector();
    clang::ObjCInterfaceDecl *interfaceDecl = decl->getClassInterface();
    if (interfaceDecl)
    {
        for (clang::ObjCProtocolList::iterator protocol = interfaceDecl->protocol_begin(),
            protocolEnd = interfaceDecl->protocol_end(); protocol != protocolEnd; protocol++)
        {
            clang::ObjCProtocolDecl *protocolDecl = (clang::ObjCProtocolDecl *)*protocol;
            if (protocolDecl->lookupInstanceMethod(selector))
            {
                return true;
            }
        }
    }

    return false;
}

bool isObjCMethodDeclLocatedInInterfaceContainer(clang::ObjCMethodDecl *decl)
{
    if (decl)
    {
        clang::DeclContext *context = decl->getDeclContext();
        return context && (clang::isa<clang::ObjCInterfaceDecl>(context) ||
            clang::isa<clang::ObjCProtocolDecl>(context) ||
            clang::isa<clang::ObjCCategoryDecl>(context));
    }
    return false;
}

bool isObjCMethodDeclInChildOfClass(
    const clang::ObjCMethodDecl* decl, const std::string& className) {
    const clang::ObjCInterfaceDecl* interface = decl->getClassInterface();
    while (interface->getSuperClass() != nullptr)
    {
        interface = interface->getSuperClass();
        if(interface->getNameAsString() == className) {
            return true;
        }
    }
    return false;
}

bool isObjCInterfaceClassOrSubclass(
    const clang::ObjCInterfaceDecl* decl, const std::string& className) {
    for(auto current = decl; current != nullptr; current = current->getSuperClass()) {
        if(current->getNameAsString() == className) {
            return true;
        }
    }
    return false;
}

bool isCppMethodDeclLocatedInCppRecordDecl(clang::CXXMethodDecl *decl)
{
    if (decl)
    {
        clang::DeclContext *context = decl->getLexicalDeclContext();
        return !decl->hasBody() || (context && clang::isa<clang::CXXRecordDecl>(context));
    }
    return false;
}

bool isANullPointerExpr(const clang::Expr& expr)
{
    for (const clang::CastExpr* castExpr = clang::dyn_cast<clang::CastExpr>(&expr);
         castExpr;
         castExpr = clang::dyn_cast<clang::CastExpr>(castExpr->getSubExpr())) {
        if (castExpr->getCastKind() == clang::CK_NullToPointer) {
            return true;
        }
    }
    return false;
}

bool areSameExpr(clang::ASTContext& context, const clang::Expr& lhs, const clang::Expr& rhs)
{
    llvm::FoldingSetNodeID lhsID;
    llvm::FoldingSetNodeID rhsID;
    lhs.Profile(lhsID, context, true);
    rhs.Profile(rhsID, context, true);
    return lhsID == rhsID;
}

const clang::Expr* ignoreCastExpr(const clang::Expr& expr)
{
    const clang::Expr* last = &expr;
    for (const clang::CastExpr* e = clang::dyn_cast<clang::CastExpr>(&expr);
         e;
         e = clang::dyn_cast<clang::CastExpr>(e->getSubExpr())) {
        last = e->getSubExpr();
    }
    return last;
}

int getLineCount(clang::SourceRange sourceRange, const clang::SourceManager& sourceManager)
{
    clang::SourceLocation startLocation = sourceRange.getBegin();
    clang::SourceLocation endLocation = sourceRange.getEnd();

    unsigned startLineNumber = sourceManager.getPresumedLineNumber(startLocation);
    unsigned endLineNumber = sourceManager.getPresumedLineNumber(endLocation);
    return endLineNumber - startLineNumber + 1;
}
