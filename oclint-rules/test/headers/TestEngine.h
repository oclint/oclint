#include "oclint/RuleBase.h"

using namespace oclint;

class TestProcessor : public ASTConsumer
{
private:
    RuleBase* _rule;
    ViolationSet *_violationSet;

public:
    TestProcessor(RuleBase *rule, ViolationSet *violationSet)
    {
        _rule = rule;
        _violationSet = violationSet;
    }

    virtual void HandleTranslationUnit(ASTContext &astContext) override
    {
        RuleCarrier *carrier = new RuleCarrier(&astContext, _violationSet);
        _rule->takeoff(carrier);
    }
};

class TestFrontendAction : public clang::ASTFrontendAction
{
private:
    RuleBase* _rule;
    ViolationSet *_violationSet;

public:
    TestFrontendAction(RuleBase *rule, ViolationSet *violationSet)
    {
        _rule = rule;
        _violationSet = violationSet;
    }

    std::unique_ptr<clang::ASTConsumer>
        CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef) override
    {
        return std::make_unique<TestProcessor>(_rule, _violationSet);
    }
};
