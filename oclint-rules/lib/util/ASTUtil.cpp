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
    if (decl && !isObjCMethodDeclLocatedInInterfaceContainer(decl))
    {
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

bool isCppMethodDeclLocatedInCppRecordDecl(clang::CXXMethodDecl *decl)
{
    if (decl)
    {
        clang::DeclContext *context = decl->getLexicalDeclContext();
        return !decl->hasBody() || (context && clang::isa<clang::CXXRecordDecl>(context));
    }
    return false;
}
