#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <string>

using namespace std;

#include <clang/Tooling/Tooling.h>
#include <clang/AST/ASTConsumer.h>

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

#define OCLINT_RULECONFIGURATION_H

class RuleConfiguration
{
public:
    static void addConfiguration(string key, string value);
    static bool hasKey(string key);
    static string valueForKey(string key);
    static void removeAll();
};

void RuleConfiguration::addConfiguration(string key, string value) {}

bool RuleConfiguration::hasKey(string key) { return false; }

string RuleConfiguration::valueForKey(string key) { return ""; }

void RuleConfiguration::removeAll() {}

#define OCLINT_RULESET_H

#include "oclint/RuleBase.h"

class RuleSet
{
public:
    RuleSet(RuleBase* rule) {}
    void apply(RuleBase* rule, ASTContext &astContext, ViolationSet* violationSet)
    {
        rule->apply(astContext, violationSet);
    }
};

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
    RuleSet ruleSet(_rule);
    ruleSet.apply(_rule, astContext, _violationSet);
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

void testRuleOnCode(const Twine &fileName,
    RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "")
{
    ViolationSet *violationSet = new ViolationSet();
    TestProcessor *processor = new TestProcessor(rule, violationSet);
    FrontendAction *action = new TestFrontendAction(processor);
    Twine twine(code);
    if (runToolOnCode(action, twine, fileName))
    {
        vector<Violation> violations = violationSet->getViolations();

        EXPECT_LT(violationIndex, violations.size());
        if (violationIndex < violations.size())
        {
            Violation violation = violations.at(violationIndex);
            EXPECT_EQ(expectStartLine, violation.startLine);
            EXPECT_EQ(expectStartColumn, violation.startColumn);
            EXPECT_EQ(expectEndLine, violation.endLine);
            EXPECT_EQ(expectEndColumn, violation.endColumn);
            EXPECT_EQ(expectMessage, violation.message);
        }
    }
    else
    {
        FAIL();
    }
}

void testRuleOnCode(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "")
{
    testRuleOnCode("input.c", rule, code, violationIndex,
        expectStartLine, expectStartColumn, expectEndLine, expectEndColumn, expectMessage);
}

void testRuleOnCXXCode(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "")
{
    testRuleOnCode("input.cpp", rule, code, violationIndex,
        expectStartLine, expectStartColumn, expectEndLine, expectEndColumn, expectMessage);
}

void testRuleOnObjCCode(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "")
{
    testRuleOnCode("input.m", rule, code, violationIndex,
        expectStartLine, expectStartColumn, expectEndLine, expectEndColumn, expectMessage);
}

#include "CanaryTest.h"
#include "basic/DeadCodeRuleTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
