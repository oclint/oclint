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
    singleLineMapping.clear();
    rangeMapping.clear();

    ViolationSet *violationSet = new ViolationSet();
    TestProcessor *processor = new TestProcessor(rule, violationSet);
    FrontendAction *action = new TestFrontendAction(processor);
    Twine twine(code);
    if (runToolOnCode(action, twine, fileName))
    {
        vector<Violation> violations = violationSet->getViolations();
        if (violationIndex < 0)
        {
            EXPECT_THAT(violations.size(), Eq(0));
        }
        else
        {
            EXPECT_LT(violationIndex, violations.size());
            if (violationIndex < violations.size())
            {
                Violation violation = violations.at(violationIndex);
                EXPECT_THAT(violation.startLine, Eq(expectStartLine));
                EXPECT_THAT(violation.startColumn, Eq(expectStartColumn));
                EXPECT_THAT(violation.endLine, Eq(expectEndLine));
                EXPECT_THAT(violation.endColumn, Eq(expectEndColumn));
                EXPECT_THAT(violation.message, StrEq(expectMessage));
            }
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

void testRuleOnCode(RuleBase *rule, const string &code)
{
    testRuleOnCode(rule, code, -1, 0, 0, 0, 0);
}

void testRuleOnCXXCode(RuleBase *rule, const string &code)
{
    testRuleOnCXXCode(rule, code, -1, 0, 0, 0, 0);
}

void testRuleOnObjCCode(RuleBase *rule, const string &code)
{
    testRuleOnObjCCode(rule, code, -1, 0, 0, 0, 0);
}
