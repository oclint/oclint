#ifndef OCLINT_UTIL_ASTUTIL_H
#define OCLINT_UTIL_ASTUTIL_H

#include <clang/AST/AST.h>

using namespace clang;

bool isObjCMethodDeclaredInSuperClass(ObjCMethodDecl *decl);
bool isObjCMethodDeclaredInProtocol(ObjCMethodDecl *decl);
bool isObjCMethodDeclLocatedInInterfaceContainer(ObjCMethodDecl *decl);
bool isCppMethodDeclLocatedInCppRecordDecl(CXXMethodDecl *decl);

#endif
