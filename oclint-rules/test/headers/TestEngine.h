class TestProcessor : public ASTConsumer
{
private:
    RuleBase* _rule;
    ViolationSet *_violationSet;

public:
    TestProcessor(RuleBase *rule, ViolationSet *violationSet);
    virtual void HandleTranslationUnit(ASTContext &astContext);
};

TestProcessor::TestProcessor(RuleBase *rule, ViolationSet *violationSet)
{
    _rule = rule;
    _violationSet = violationSet;
}

void TestProcessor::HandleTranslationUnit(ASTContext &astContext)
{
    _rule->apply(astContext, _violationSet);
}

class TestFrontendAction : public clang::ASTFrontendAction
{
private:
    TestProcessor* _processor;

public:
    TestFrontendAction(TestProcessor* processor)
    {
        _processor = processor;
    }

    clang::ASTConsumer *CreateASTConsumer(clang::CompilerInstance &, llvm::StringRef)
    {
        return _processor;
    }
};
