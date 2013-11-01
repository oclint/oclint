inline void testRuleOnCode(const Twine &fileName,
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
        const vector<Violation>& violations = violationSet->getViolations();
        if (violationIndex < 0)
        {
            EXPECT_THAT(violations.size(), Eq(0u));
        }
        else
        {
            EXPECT_LT(size_t(violationIndex), violations.size());
            if (size_t(violationIndex) < violations.size())
            {
                const Violation& violation = violations.at(violationIndex);
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

inline void testRuleOnCode(RuleBase *rule,
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

inline void testRuleOnCXXCode(RuleBase *rule,
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

inline void testRuleOnCXX11Code(RuleBase *rule,
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

inline void testRuleOnObjCCode(RuleBase *rule,
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

inline void testRuleOnCode(RuleBase *rule, const string &code)
{
    testRuleOnCode(rule, code, -1, 0, 0, 0, 0);
}

inline void testRuleOnCXXCode(RuleBase *rule, const string &code)
{
    testRuleOnCXXCode(rule, code, -1, 0, 0, 0, 0);
}

inline void testRuleOnCXX11Code(RuleBase *rule, const string &code)
{
    testRuleOnCXX11Code(rule, code, -1, 0, 0, 0, 0);
}

inline void testRuleOnObjCCode(RuleBase *rule, const string &code)
{
    testRuleOnObjCCode(rule, code, -1, 0, 0, 0, 0);
}

#define VIOLATION_START "/*VIOLATION_START*/"
#define VIOLATION_END "/*VIOLATION_END*/"

inline void ComputeLocalisation(const std::string& s, size_t offset,
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

// {start/end offset} ordered by increasing end-location.
inline std::vector<std::pair<size_t, size_t>> getViolationLocations(std::string& code)
{
    size_t findOffset = 0;
    std::vector<size_t> startOffsets;
    std::vector<std::pair<size_t,size_t>> rangeOffsets;

    for (;;)
    {
        const size_t startOffset = code.find(VIOLATION_START, findOffset);
        const size_t endOffset = code.find(VIOLATION_END, findOffset);
        if (startOffset == std::string::npos && endOffset == std::string::npos)
        {
            assert(startOffsets.empty());
            return rangeOffsets;
        }
        if (startOffset < endOffset) {
            code.erase(startOffset, strlen(VIOLATION_START));
            startOffsets.push_back(startOffset);
            findOffset = startOffset;
        } else {
            code.erase(endOffset, strlen(VIOLATION_END));
            assert(!startOffsets.empty());
            rangeOffsets.push_back({startOffsets.back(), endOffset});
            startOffsets.pop_back();
            findOffset = endOffset;
        }
    }
}

inline void testRuleOnCode(const Twine& filename,
                           const std::vector<std::string>& args,
                           RuleBase* rule,
                           std::string code,
                           const std::vector<std::string>& messages)
{
    const auto& rangeOffsets = getViolationLocations(code);

    EXPECT_THAT(rangeOffsets.size(), Eq(messages.size()));

    if (rangeOffsets.empty())
    {
        testRuleOnCode(filename, rule, code, args, -1, 0, 0, 0, 0, "");
        return;
    }
    for (size_t i = 0; i != rangeOffsets.size(); ++i)
    {
        const size_t startOffset = rangeOffsets[i].first;
        const size_t endOffset = rangeOffsets[i].second;
        int startLine;
        int startColumn;
        int endLine;
        int endColumn;

        ComputeLocalisation(code, startOffset, &startLine, &startColumn);
        ComputeLocalisation(code, endOffset, &endLine, &endColumn);
        testRuleOnCode(filename, rule, code, args,
                       i, startLine, startColumn, endLine, endColumn, messages[i]);
    }
}

inline void testRuleOnCode(RuleBase* rule, std::string code, const std::vector<std::string>& messages)
{
    return testRuleOnCode("input.c", {}, rule, code, messages);
}

inline void testRuleOnCXXCode(RuleBase* rule, std::string code, const std::vector<std::string>& messages)
{
    return testRuleOnCode("input.cpp", {}, rule, code, messages);
}

inline void testRuleOnCXX11Code(RuleBase* rule, std::string code, const std::vector<std::string>& messages)
{
    return testRuleOnCode("input.cpp", {"-std=c++11"}, rule, code, messages);
}

inline void testRuleOnObjCCode(RuleBase* rule, std::string code, const std::vector<std::string>& messages)
{
    return testRuleOnCode("input.m", {}, rule, code, messages);
}
