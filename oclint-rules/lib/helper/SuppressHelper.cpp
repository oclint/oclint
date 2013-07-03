#include <clang/AST/Attr.h>

#include "oclint/helper/SuppressHelper.h"

bool markedAsSuppress(const clang::Decl *decl, oclint::RuleBase *rule)
{
    if (!decl)
    {
        return false;
    }

    for (clang::specific_attr_iterator<clang::AnnotateAttr> attr =
        decl->specific_attr_begin<clang::AnnotateAttr>(),
        attrEnd = decl->specific_attr_end<clang::AnnotateAttr>();
        attr != attrEnd;
        attr++)
    {
        clang::AnnotateAttr *annotate = clang::dyn_cast<clang::AnnotateAttr>(*attr);
        if (annotate && (annotate->getAnnotation() == "oclint:suppress" ||
            annotate->getAnnotation() == "oclint:suppress[" + rule->name() + "]"))
        {
            return true;
        }
    }

    return false;
}

template <typename T>
bool markedParentsAsSuppress(const T &node, clang::ASTContext &context, oclint::RuleBase *rule)
{
    clang::ASTContext::ParentVector parentVector = context.getParents(node);
    if (parentVector.size() > 0)
    {
        clang::ast_type_traits::DynTypedNode *dynTypedNode = parentVector.begin();
        const clang::Decl *aDecl = dynTypedNode->get<clang::Decl>();
        if (aDecl) {
            if (markedAsSuppress(aDecl, rule))
            {
                return true;
            }
            return markedParentsAsSuppress(*aDecl, context, rule);
        }
        const clang::Stmt *aStmt = dynTypedNode->get<clang::Stmt>();
        if (aStmt) {
            return markedParentsAsSuppress(*aStmt, context, rule);
        }
    }

    return false;
}

bool shouldSuppress(const clang::Decl *decl, clang::ASTContext &context, oclint::RuleBase *rule)
{
    return markedAsSuppress(decl, rule) || markedParentsAsSuppress(*decl, context, rule);
}

bool shouldSuppress(const clang::Stmt *stmt, clang::ASTContext &context, oclint::RuleBase *rule)
{
    return markedParentsAsSuppress(*stmt, context, rule);
}
