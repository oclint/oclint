#ifndef OCLINT_ABSTRACTASTMATCHERRULE_H
#define OCLINT_ABSTRACTASTMATCHERRULE_H

#include <clang/ASTMatchers/ASTMatchFinder.h>

#include "oclint/AbstractASTVisitorRule.h"

using namespace clang::ast_matchers;

class AbstractASTMatcherRule :
    public AbstractASTVisitorRule<AbstractASTMatcherRule>, public MatchFinder::MatchCallback
{
private:
    MatchFinder _finder;

protected:
    virtual void run(const MatchFinder::MatchResult &result)
    {
        callback(result);
    }

    void addMatcher(const DeclarationMatcher &nodeMatch)
    {
        _finder.addMatcher(nodeMatch, this);
    }

    void addMatcher(const TypeMatcher &nodeMatch)
    {
        _finder.addMatcher(nodeMatch, this);
    }

    void addMatcher(const StatementMatcher &nodeMatch)
    {
        _finder.addMatcher(nodeMatch, this);
    }

    void addMatcher(const NestedNameSpecifierMatcher &nodeMatch)
    {
        _finder.addMatcher(nodeMatch, this);
    }

    void addMatcher(const NestedNameSpecifierLocMatcher &nodeMatch)
    {
        _finder.addMatcher(nodeMatch, this);
    }

    void addMatcher(const TypeLocMatcher &nodeMatch)
    {
        _finder.addMatcher(nodeMatch, this);
    }

public:
    virtual void setUp()
    {
        setUpMatcher();
    }

    bool VisitDecl(Decl *decl)
    {
        _finder.match(*decl, *_carrier->astContext());
        return true;
    }

    bool VisitStmt(Stmt *stmt)
    {
        _finder.match(*stmt, *_carrier->astContext());
        return true;
    }

public:
    virtual ~AbstractASTMatcherRule() {}

    virtual void setUpMatcher() = 0;
    virtual void callback(const MatchFinder::MatchResult &result) = 0;
};

#endif
