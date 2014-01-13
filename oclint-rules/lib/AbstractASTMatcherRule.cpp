#include "oclint/AbstractASTMatcherRule.h"

namespace oclint
{

/*virtual*/
AbstractASTMatcherRule::~AbstractASTMatcherRule() {}

/*virtual*/
void AbstractASTMatcherRule::run(const clang::ast_matchers::MatchFinder::MatchResult &result)
{
    callback(result);
}

/*virtual*/
void AbstractASTMatcherRule::setUp()
{
    _finder = new clang::ast_matchers::MatchFinder();
    setUpMatcher();
}

bool AbstractASTMatcherRule::VisitDecl(clang::Decl *decl)
{
    _finder->match(*decl, *_carrier->getASTContext());
    return true;
}

bool AbstractASTMatcherRule::VisitStmt(clang::Stmt *stmt)
{
    _finder->match(*stmt, *_carrier->getASTContext());
    return true;
}

/*virtual*/
void AbstractASTMatcherRule::tearDown()
{
    delete _finder;
    _finder = nullptr;
}

} // end namespace oclint
