#ifndef OCLINT_UTIL_ASTUTIL_H
#define OCLINT_UTIL_ASTUTIL_H

#include <clang/AST/AST.h>

bool isObjCMethodDeclaredInSuperClass(clang::ObjCMethodDecl *decl);
bool isObjCMethodDeclaredInProtocol(clang::ObjCMethodDecl *decl);
bool isObjCMethodDeclLocatedInInterfaceContainer(clang::ObjCMethodDecl *decl);
bool isCppMethodDeclLocatedInCppRecordDecl(clang::CXXMethodDecl *decl);

#endif
