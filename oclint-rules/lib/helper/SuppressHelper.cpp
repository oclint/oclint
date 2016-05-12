#include "oclint/helper/SuppressHelper.h"

#include <regex>
#include <set>
#include <unordered_map>
#include <utility>

#include <clang/AST/RecursiveASTVisitor.h>

#include "oclint/RuleCarrier.h"

#include "oclint/helper/AttributeHelper.h"

bool markedAsSuppress(const clang::Decl *decl, oclint::RuleBase *rule)
{
    if(rule) {
        return declHasOCLintAttribute(decl, "suppress")
            || declHasActionAttribute(decl, "suppress", *rule);
    }
    return false;
}

template <typename T>
bool markedParentsAsSuppress(const T &node, clang::ASTContext &context, oclint::RuleBase *rule)
{
    const auto &parents = context.getParents(node);
    if (parents.empty())
    {
        return false;
    }

    const clang::ast_type_traits::DynTypedNode* dynTypedNode = parents.begin();
    const clang::Decl *aDecl = dynTypedNode->get<clang::Decl>();
    if (aDecl)
    {
        if (markedAsSuppress(aDecl, rule))
        {
            return true;
        }
        return markedParentsAsSuppress(*aDecl, context, rule);
    }
    const clang::Stmt *aStmt = dynTypedNode->get<clang::Stmt>();
    if (aStmt)
    {
        return markedParentsAsSuppress(*aStmt, context, rule);
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

std::string getMainFilePath(clang::ASTContext &context)
{
    oclint::RuleCarrier ruleCarrier(&context, nullptr);
    return ruleCarrier.getMainFilePath();
}

typedef std::unordered_map<std::string, std::set<int>> LineMap;
static LineMap singleLineMapping;

bool lineBasedShouldSuppress(int beginLine, clang::ASTContext &context)
{
    std::string filePath = getMainFilePath(context);
    auto commentLinesIt = singleLineMapping.find(filePath);
    std::set<int> commentLines;
    if (commentLinesIt == singleLineMapping.end())
    {
        clang::RawCommentList commentList = context.getRawCommentList();
        clang::ArrayRef<clang::RawComment *> commentArray = commentList.getComments();

        for (auto comment : commentArray)
        {
// g++ 4.8 on Ubuntu 14.04 LTS doesn't support regex yet,
// so we will ship this once Ubuntu 16.04 releases
#if defined(__APPLE__) || defined(__MACH__)
            std::string commentString = comment->getRawText(context.getSourceManager()).str();
            std::regex oclintRegex =
                std::regex("//! *OCLINT", std::regex::basic | std::regex::icase);
            if (std::regex_search(commentString, oclintRegex))
#else
            if (std::string::npos !=
                comment->getRawText(context.getSourceManager()).find("//!OCLINT"))
#endif
            {
                clang::SourceLocation startLocation = comment->getLocStart();
                int startLocationLine =
                    context.getSourceManager().getPresumedLineNumber(startLocation);
                commentLines.insert(startLocationLine);
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
    auto commentRangesIt = rangeMapping.find(filePath);
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
