#ifndef OCLINT_ABSTRACTASTMATCHERRULE_H
#define OCLINT_ABSTRACTASTMATCHERRULE_H

#include <clang/ASTMatchers/ASTMatchFinder.h>

#include "oclint/AbstractASTRuleBase.h"

using namespace clang::ast_matchers;

class AbstractASTMatcherRule : public AbstractASTRuleBase, public MatchFinder::MatchCallback
{
private:
    MatchFinder _finder;

protected:
    virtual void apply()
    {
        setUpMatcher();
        SourceManager *sourceManager = &_carrier->astContext()->getSourceManager();
        DeclContext *decl = _carrier->astContext()->getTranslationUnitDecl();
        for (DeclContext::decl_iterator it = decl->decls_begin(), declEnd = decl->decls_end();
            it != declEnd; ++it)
        {
            SourceLocation startLocation = (*it)->getLocStart();
            if (startLocation.isValid() &&
                sourceManager->getMainFileID() == sourceManager->getFileID(startLocation))
            {
                _finder.findAll(**it, *_carrier->astContext());
            }
        }
    }

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
    virtual ~AbstractASTMatcherRule() {}

    virtual void setUpMatcher() = 0;
    virtual void callback(const MatchFinder::MatchResult &result) = 0;
};

#endif
