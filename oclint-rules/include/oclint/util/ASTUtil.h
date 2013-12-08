#ifndef OCLINT_UTIL_ASTUTIL_H
#define OCLINT_UTIL_ASTUTIL_H

#include <clang/AST/AST.h>

bool isObjCMethodDeclaredInSuperClass(clang::ObjCMethodDecl *decl);
bool isObjCMethodDeclaredInProtocol(clang::ObjCMethodDecl *decl);
bool isObjCMethodDeclLocatedInInterfaceContainer(clang::ObjCMethodDecl *decl);
bool isObjCMethodDeclInChildOfClass(const clang::ObjCMethodDecl* decl, const std::string& className);
bool isObjCInterfaceClassOrSubclass(const clang::ObjCInterfaceDecl* decl, const std::string& className);
bool isCppMethodDeclLocatedInCppRecordDecl(clang::CXXMethodDecl *decl);
bool isANullPointerExpr(const clang::Expr& expr);
bool areSameExpr(clang::ASTContext& context, const clang::Expr& lhs, const clang::Expr& rhs);
const clang::Expr* ignoreCastExpr(const clang::Expr& expr);
int getLineCount(clang::SourceRange sourceRange, const clang::SourceManager& sourceManager);

#endif
