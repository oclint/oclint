#include <set>
#include <unordered_map>
#include <utility>

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
        ++attr)
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

typedef std::set<std::pair<int, int>> RangeSet;

class DeclAnnotationRangeCollector : public clang::RecursiveASTVisitor<DeclAnnotationRangeCollector>
{
private:
    oclint::RuleBase *_rule;
    clang::SourceManager *_sourceManager;
    RangeSet _range;

public:
    RangeSet collect(clang::ASTContext &astContext, oclint::RuleBase *rule)
    {
        _rule = rule;
        _sourceManager = &astContext.getSourceManager();
        _range.clear();

        clang::DeclContext *decl = astContext.getTranslationUnitDecl();
        for (clang::DeclContext::decl_iterator declIt = decl->decls_begin(),
            declEnd = decl->decls_end(); declIt != declEnd; ++declIt)
        {
            clang::SourceLocation startLocation = (*declIt)->getLocStart();
            if (startLocation.isValid() &&
                _sourceManager->getMainFileID() == _sourceManager->getFileID(startLocation))
            {
                (void) /* explicitly ignore the return of this function */
                    clang::RecursiveASTVisitor<DeclAnnotationRangeCollector>::TraverseDecl(*declIt);
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
            _range.insert(std::make_pair(startLineNumber, endLineNumber));
        }

        return true;
    }
};

// TODO: use the same method from RuleCarrier when it's ready
std::string getMainFilePath(clang::ASTContext &context)
{
    clang::FileID mainFileId = context.getSourceManager().getMainFileID();
    clang::SourceLocation mainSourceLocation =
        context.getSourceManager().getLocForStartOfFile(mainFileId);
    return context.getSourceManager().getFilename(mainSourceLocation).str();
}

typedef std::unordered_map<std::string, std::set<int>> LineMap;
static LineMap singleLineMapping;

bool lineBasedShouldSuppress(int beginLine, clang::ASTContext &context)
{
    std::string filePath = getMainFilePath(context);
    LineMap::iterator commentLinesIt = singleLineMapping.find(filePath);
    std::set<int> commentLines;
    if (commentLinesIt == singleLineMapping.end())
    {
        clang::RawCommentList commentList = context.getRawCommentList();
        clang::ArrayRef<clang::RawComment *> commentArray = commentList.getComments();

        for (clang::ArrayRef<clang::RawComment *>::iterator commentIt = commentArray.begin(),
            itEnd = commentArray.end(); commentIt != itEnd; ++commentIt)
        {
            clang::RawComment *comment = *commentIt;
            if (std::string::npos !=
                comment->getRawText(context.getSourceManager()).find("//!OCLINT"))
            {
                commentLines.insert(comment->getBeginLine(context.getSourceManager()));
            }
        }
        singleLineMapping[filePath] = commentLines;
    }
    else
    {
        commentLines = commentLinesIt->second;
    }

    return commentLines.find(beginLine) != commentLines.end();
}

typedef std::unordered_map<std::string, RangeSet> RangeMap;
static RangeMap rangeMapping;

bool rangeBasedShouldSuppress(int beginLine, clang::ASTContext &context, oclint::RuleBase *rule)
{
    std::string filePath = getMainFilePath(context);
    RangeMap::iterator commentRangesIt = rangeMapping.find(filePath);
    RangeSet commentRanges;
    if (commentRangesIt == rangeMapping.end())
    {
        DeclAnnotationRangeCollector annotationCollector;
        commentRanges = annotationCollector.collect(context, rule);
        rangeMapping[filePath] = commentRanges;
    }
    else
    {
        commentRanges = commentRangesIt->second;
    }

    for (const auto& range : commentRanges)
    {
        if (beginLine >= range.first && beginLine <= range.second)
        {
            return true;
        }
    }

    return false;
}

bool shouldSuppress(int beginLine, clang::ASTContext &context, oclint::RuleBase *rule)
{
    return lineBasedShouldSuppress(beginLine, context) ||
        (rule && rangeBasedShouldSuppress(beginLine, context, rule));
}
