#ifndef OCLINT_ABSTRACTASTMATCHERRULE_H
#define OCLINT_ABSTRACTASTMATCHERRULE_H

#include <clang/ASTMatchers/ASTMatchFinder.h>

#include "oclint/AbstractASTVisitorRule.h"

namespace oclint
{


class AbstractASTMatcherRule :
    public AbstractASTVisitorRule<AbstractASTMatcherRule>,
    public clang::ast_matchers::MatchFinder::MatchCallback
{
private:
    clang::ast_matchers::MatchFinder *_finder;

protected:
    virtual void
    run(const clang::ast_matchers::MatchFinder::MatchResult& result) override;

    template<typename T>
    void addMatcher(const T &nodeMatch)
    {
        _finder->addMatcher(nodeMatch, this);
    }

public:
    virtual void setUp() override;

    bool VisitDecl(clang::Decl *decl);

    bool VisitStmt(clang::Stmt *stmt);

    virtual void tearDown() override;

public:
    virtual ~AbstractASTMatcherRule();

    virtual void setUpMatcher() = 0;
    virtual void callback(const clang::ast_matchers::MatchFinder::MatchResult &result) = 0;
};

} // end namespace oclint

#endif
