#include "oclint/AbstractASTRuleBase.h"

#include "oclint/helper/SuppressHelper.h"


namespace oclint
{

void AbstractASTRuleBase::addViolation(clang::SourceLocation startLocation,
	clang::SourceLocation endLocation, RuleBase *rule, const std::string& message)
{
	clang::SourceManager *sourceManager = &_carrier->getSourceManager();
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

void AbstractASTRuleBase::addViolation(const clang::Decl *decl, RuleBase *rule, const std::string& message)
{
	if (decl && !shouldSuppress(decl, *_carrier->getASTContext(), rule))
	{
		addViolation(decl->getLocStart(), decl->getLocEnd(), rule, message);
	}
}

void AbstractASTRuleBase::addViolation(const clang::Stmt *stmt, RuleBase *rule, const std::string& message)
{
	if (stmt && !shouldSuppress(stmt, *_carrier->getASTContext(), rule))
	{
		addViolation(stmt->getLocStart(), stmt->getLocEnd(), rule, message);
	}
}

/*virtual*/ AbstractASTRuleBase::~AbstractASTRuleBase() {}


} // end namespace oclint
