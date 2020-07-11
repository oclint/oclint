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
    /* if it is a macro location return the expansion location or the spelling location */
    clang::SourceLocation startFileLoc = sourceManager->getFileLoc(startLocation);
    clang::SourceLocation endFileLoc = sourceManager->getFileLoc(endLocation);
    int beginLine = sourceManager->getPresumedLineNumber(startFileLoc);
    if (!shouldSuppress(beginLine, *_carrier->getASTContext()))
    {
        llvm::StringRef filename = sourceManager->getFilename(startFileLoc);
        _carrier->addViolation(filename.str(),
            beginLine,
            sourceManager->getPresumedColumnNumber(startFileLoc),
            sourceManager->getPresumedLineNumber(endFileLoc),
            sourceManager->getPresumedColumnNumber(endFileLoc),
            rule,
            message);
    }
}

void AbstractASTRuleBase::addViolation(const clang::Decl *decl,
    RuleBase *rule, const std::string& message)
{
    if (decl && !shouldSuppress(decl, *_carrier->getASTContext(), rule))
    {
        addViolation(decl->getBeginLoc(), decl->getEndLoc(), rule, message);
    }
}

void AbstractASTRuleBase::addViolation(const clang::Stmt *stmt,
    RuleBase *rule, const std::string& message)
{
    if (stmt && !shouldSuppress(stmt, *_carrier->getASTContext(), rule))
    {
        addViolation(stmt->getBeginLoc(), stmt->getEndLoc(), rule, message);
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
    if (langOpts.ObjC)
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

unsigned int AbstractASTRuleBase::supportedCUDAFunctionAttrs() const
{
    return CUDA_HOST;
}

bool AbstractASTRuleBase::isCUDASourceFile()
{
    const auto &langOpts = _carrier->getASTContext()->getLangOpts();
    return langOpts.CUDA;
}

} // end namespace oclint
