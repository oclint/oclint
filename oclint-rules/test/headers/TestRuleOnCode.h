void testRuleOnCode(const Twine &fileName,
    RuleBase *rule,
    const string &code,
    const std::vector<std::string> &args,
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

    const std::size_t randomPrefixLength = 6;
    std::string randomPrefix(randomPrefixLength, 0);
    const char randomChars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const size_t randomCharSize = sizeof(randomChars) - 1;
    std::generate_n(randomPrefix.begin(), randomPrefixLength,
        [randomChars, randomCharSize]() -> char { return randomChars[ rand() % randomCharSize ]; });

    if (runToolOnCodeWithArgs(action, twine, args, randomPrefix + fileName))
    {
        vector<Violation> violations = violationSet->getViolations();
        if (violationIndex < 0)
        {
            EXPECT_THAT(violations.size(), Eq(0));
        }
        else
        {
            EXPECT_LT(size_t(violationIndex), violations.size());
            if (size_t(violationIndex) < violations.size())
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
    testRuleOnCode("input.c", rule, code, {}, violationIndex,
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
    testRuleOnCode("input.cpp", rule, code, {}, violationIndex,
        expectStartLine, expectStartColumn, expectEndLine, expectEndColumn, expectMessage);
}

void testRuleOnCXX11Code(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "")
{
    testRuleOnCode("input.cpp", rule, code, {"-std=c++11"}, violationIndex,
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
    testRuleOnCode("input.m", rule, code, {}, violationIndex,
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

void testRuleOnCXX11Code(RuleBase *rule, const string &code)
{
    testRuleOnCXX11Code(rule, code, -1, 0, 0, 0, 0);
}

void testRuleOnObjCCode(RuleBase *rule, const string &code)
{
    testRuleOnObjCCode(rule, code, -1, 0, 0, 0, 0);
}

#define TAG_START "TAG_START"
#define TAG_END "TAG_END"

void ComputeLocalisation(const std::string& s, size_t offset,
                            int* line, int* column)
{
    int c = 1;
    int l = 1;

    for (size_t i = 0; i != offset; ++i) {
        if (s[i] == '\n') {
            c = 1;
            ++l;
        } else {
            ++c;
        }
    }
    *line = l;
    *column = c;
}

void testRuleOnCXXCodeWithViolation(RuleBase* rule, std::string code,
                        const std::string& message = "")
{
    const size_t startOffset = code.find(TAG_START);
    assert(startOffset != std::string::npos);
    code.erase(startOffset, strlen(TAG_START));
    const size_t endOffset = code.find(TAG_END, startOffset);
    assert(endOffset != std::string::npos);
    code.erase(endOffset, strlen(TAG_END));

    int startLine;
    int startColumn;
    int endLine;
    int endColumn;
    ComputeLocalisation(code, startOffset, &startLine, &startColumn);
    ComputeLocalisation(code, endOffset, &endLine, &endColumn);
    testRuleOnCXXCode(rule, code, 0, startLine, startColumn, endLine, endColumn, message);
}
