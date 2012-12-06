#include <clang/AST/AST.h>

#include "oclint/Rule.h"
#include "oclint/RuleSet.h"
#include "oclint/util/StdUtil.h"

class LongClassRule : public Rule<LongClassRule>
{
private:
    static RuleSet rules;

    int _threshold;

    int getNumberOfLines(SourceRange sourceRange)
    {
        SourceLocation startLocation = sourceRange.getBegin();
        SourceLocation endLocation = sourceRange.getEnd();
        SourceManager *sourceManager = &_astContext->getSourceManager();

        unsigned startLineNumber = sourceManager->getPresumedLineNumber(startLocation);
        unsigned endLineNumber = sourceManager->getPresumedLineNumber(endLocation);
        return endLineNumber - startLineNumber + 1;
    }

public:
    virtual const string name() const
    {
        return "long class";
    }

    virtual const int priority() const
    {
        return 3;
    }

    virtual void setUp()
    {
        _threshold = ruleConfiguration("LONG_CLASS", 1000);
    }

    bool VisitObjCInterfaceDecl(ObjCInterfaceDecl *decl)
    {
        int length = getNumberOfLines(decl->getSourceRange());
        if (length > _threshold)
        {
            string description = "Objective-C interface with " +
                intToString(length) + " lines exceeds limit of " + intToString(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }

    bool VisitObjCCategoryDecl(ObjCCategoryDecl *decl)
    {
        int length = getNumberOfLines(decl->getSourceRange());
        if (length > _threshold)
        {
            string description = "Objective-C category with " +
                intToString(length) + " lines exceeds limit of " + intToString(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }

    bool VisitObjCProtocolDecl(ObjCProtocolDecl *decl)
    {
        int length = getNumberOfLines(decl->getSourceRange());
        if (length > _threshold)
        {
            string description = "Objective-C protocol with " +
                intToString(length) + " lines exceeds limit of " + intToString(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }

    bool VisitObjCImplDecl(ObjCImplDecl *decl)
    {
        int length = getNumberOfLines(decl->getSourceRange());
        if (length > _threshold)
        {
            string description = "Objective-C implementation with " +
                intToString(length) + " lines exceeds limit of " + intToString(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }

    bool VisitCXXRecordDecl(CXXRecordDecl *decl)
    {
        int length = getNumberOfLines(decl->getSourceRange());
        if (length > _threshold)
        {
            string description = "C++ class with " +
                intToString(length) + " lines exceeds limit of " + intToString(_threshold);
            addViolation(decl, this, description);
        }
        return true;
    }
};

RuleSet LongClassRule::rules(new LongClassRule());
