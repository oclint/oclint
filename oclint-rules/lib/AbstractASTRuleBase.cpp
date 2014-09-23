#include "oclint/AbstractASTRuleBase.h"
#include "oclint/helper/SuppressHelper.h"

namespace oclint
{

/*virtual*/
AbstractASTRuleBase::~AbstractASTRuleBase() {}

void AbstractASTRuleBase::addViolation(clang::SourceLocation startLocation,
    clang::SourceLocation endLocation, RuleBase *rule, const std::string& message)
{
    clang::SourceManager *sourceManager = &_carrier->getSourceManager();
    /* handle macro locations : */
    startLocation = sourceManager->getFileLoc(startLocation);
    endLocation = sourceManager->getFileLoc(endLocation);
    int beginLine = sourceManager->getPresumedLineNumber(startLocation);
    if (!shouldSuppress(beginLine, *_carrier->getASTContext()))
    {
        llvm::StringRef filename = sourceManager->getFilename(startLocation);
        _carrier->addViolation(filename.str(),
            beginLine,
            sourceManager->getPresumedColumnNumber(startLocation),
            sourceManager->getPresumedLineNumber(endLocation),
            sourceManager->getPresumedColumnNumber(endLocation),
            rule,
            message);
    }
}

void AbstractASTRuleBase::addViolation(const clang::Decl *decl,
    RuleBase *rule, const std::string& message)
{
    if (decl && !shouldSuppress(decl, *_carrier->getASTContext(), rule))
    {
        addViolation(decl->getLocStart(), decl->getLocEnd(), rule, message);
    }
}

void AbstractASTRuleBase::addViolation(const clang::Stmt *stmt,
    RuleBase *rule, const std::string& message)
{
    if (stmt && !shouldSuppress(stmt, *_carrier->getASTContext(), rule))
    {
        addViolation(stmt->getLocStart(), stmt->getLocEnd(), rule, message);
    }
}

unsigned int AbstractASTRuleBase::supportedLanguages() const
{
    return LANG_C | LANG_CXX | LANG_OBJC;
}

bool AbstractASTRuleBase::supportsC() const
{
    return LANG_C & supportedLanguages();
}

bool AbstractASTRuleBase::supportsCXX() const
{
    return LANG_CXX & supportedLanguages();
}

bool AbstractASTRuleBase::supportsObjC() const
{
    return LANG_OBJC & supportedLanguages();
}

bool AbstractASTRuleBase::isLanguageSupported() const
{
    const auto &langOpts = _carrier->getASTContext()->getLangOpts();
    if (langOpts.ObjC1)
    {
        return supportsObjC();
    }
    if (langOpts.CPlusPlus)
    {
        return supportsCXX();
    }
    if (langOpts.C99)
    {
        return supportsC();
    }
    return false;
}

} // end namespace oclint
