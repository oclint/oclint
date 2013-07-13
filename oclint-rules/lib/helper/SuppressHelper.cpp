#include <utility>
#include <vector>

#include <clang/AST/Attr.h>
#include <clang/AST/RecursiveASTVisitor.h>

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

class DeclAnnotationRangeCollector : public clang::RecursiveASTVisitor<DeclAnnotationRangeCollector>
{
private:
    oclint::RuleBase *_rule;
    clang::SourceManager *_sourceManager;
    std::vector<std::pair<int, int> > _range;

public:
    std::vector<std::pair<int, int> > collect(clang::ASTContext &astContext, oclint::RuleBase *rule)
    {
        _rule = rule;
        _sourceManager = &astContext.getSourceManager();
        _range.clear();

        clang::DeclContext *decl = astContext.getTranslationUnitDecl();
        for (clang::DeclContext::decl_iterator it = decl->decls_begin(),
            declEnd = decl->decls_end(); it != declEnd; ++it)
        {
            clang::SourceLocation startLocation = (*it)->getLocStart();
            if (startLocation.isValid() &&
                _sourceManager->getMainFileID() == _sourceManager->getFileID(startLocation))
            {
                (void) /* explicitly ignore the return of this function */
                    clang::RecursiveASTVisitor<DeclAnnotationRangeCollector>::TraverseDecl(*it);
            }
        }

        return _range;
    }

    bool VisitDecl(clang::Decl *decl)
    {
        if (markedAsSuppress(decl, _rule))
        {
            clang::SourceLocation startLocation = decl->getLocStart();
            clang::SourceLocation endLocation = decl->getLocEnd();
            unsigned startLineNumber = _sourceManager->getPresumedLineNumber(startLocation);
            unsigned endLineNumber = _sourceManager->getPresumedLineNumber(endLocation);
            _range.push_back(std::make_pair(startLineNumber, endLineNumber));
        }

        return true;
    }
};

bool shouldSuppress(int beginLine, clang::ASTContext &context, oclint::RuleBase *rule)
{
    clang::RawCommentList commentList = context.getRawCommentList();
    clang::ArrayRef<clang::RawComment *> commentArray = commentList.getComments();

    // TODO: Big-O is very high by iterating the entire comment set every time!!!
    for (clang::ArrayRef<clang::RawComment *>::iterator it = commentArray.begin(),
        itEnd = commentArray.end(); it != itEnd; it++)
    {
        clang::RawComment *comment = *it;
        if (beginLine == comment->getBeginLine(context.getSourceManager()) &&
            std::string::npos != comment->getRawText(context.getSourceManager()).find("//!OCLINT"))
        {
            return true;
        }
    }

    if (rule)
    {
        // TODO: The algorithm also needs to be improved!!!
        DeclAnnotationRangeCollector annotationCollector;
        std::vector<std::pair<int, int> > annotationRanges = annotationCollector.collect(context, rule);
        for (std::vector<std::pair<int, int> >::iterator it = annotationRanges.begin(),
            itEnd = annotationRanges.end(); it != itEnd; ++it)
        {
            std::pair<int, int> range = *it;
            if (beginLine >= range.first && beginLine <= range.second)
            {
                return true;
            }
        }
    }

    return false;
}
