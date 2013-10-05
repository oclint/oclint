#ifndef OCLINT_UTIL_ASTUTIL_H
#define OCLINT_UTIL_ASTUTIL_H

#include <clang/AST/AST.h>

bool isObjCMethodDeclaredInSuperClass(clang::ObjCMethodDecl *decl);
bool isObjCMethodDeclaredInProtocol(clang::ObjCMethodDecl *decl);
bool isObjCMethodDeclLocatedInInterfaceContainer(clang::ObjCMethodDecl *decl);
bool isCppMethodDeclLocatedInCppRecordDecl(clang::CXXMethodDecl *decl);
bool isANullPointerExpr(const clang::Expr& expr);
bool areSameExpr(clang::ASTContext& context, const clang::Expr& lhs, const clang::Expr& rhs);
const clang::Expr* ignoreCastExpr(const clang::Expr& expr);

#endif
