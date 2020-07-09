#include "TestRuleOnCode.h"
#include <gmock/gmock.h>

using namespace ::testing;

#include <clang/Tooling/Tooling.h>

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

#include "oclint/RuleBase.h"
#include "TestEngine.h"

bool computeViolationSet(const Twine &fileName,
    RuleBase *rule,
    const std::string &code,
    const std::vector<std::string> &args,
    ViolationSet *violationSet)
{
    std::unique_ptr<FrontendAction> action(new TestFrontendAction(rule, violationSet));
    Twine twine(code);

    const std::size_t randomPrefixLength = 6;
    std::string randomPrefix(randomPrefixLength, 0);
    const char randomChars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const size_t randomCharSize = sizeof(randomChars) - 1;

    std::generate_n(randomPrefix.begin(), randomPrefixLength,
        [randomChars, randomCharSize]() -> char { return randomChars[rand() % randomCharSize]; });
    return runToolOnCodeWithArgs(std::move(action), twine, args, randomPrefix + fileName);
}

void validateViolation(const Violation& violation,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage)
{
    EXPECT_THAT(violation.startLine, Eq(expectStartLine));
    EXPECT_THAT(violation.startColumn, Eq(expectStartColumn));
    EXPECT_THAT(violation.endLine, Eq(expectEndLine));
    EXPECT_THAT(violation.endColumn, Eq(expectEndColumn));
    EXPECT_THAT(violation.message, StrEq(expectMessage));
}

void testRuleOnCode(const Twine &fileName,
    RuleBase *rule,
    const string &code,
    const std::vector<std::string> &args,
    int violationIndex,
    int startLine,
    int startColumn,
    int endLine,
    int endColumn,
    const string& message)
{
    ViolationSet violationSet;

    if (computeViolationSet(fileName, rule, code, args, &violationSet))
    {
        const vector<Violation>& violations = violationSet.getViolations();

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
                validateViolation(violation, startLine, startColumn, endLine, endColumn, message);
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
    const string& expectMessage)
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
    const string& expectMessage)
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
    const string& expectMessage)
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
    const string& expectMessage)
{
    testRuleOnCode("input.m", rule, code, {}, violationIndex,
        expectStartLine, expectStartColumn, expectEndLine, expectEndColumn, expectMessage);
}

void testRuleOnCUDACode(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage)
{
    testRuleOnCode("input.cu", rule, code,
        {"-nocudalib", "--no-cuda-version-check", "-I/tmp/oclint-rules-test/clang"}, violationIndex,
        expectStartLine, expectStartColumn, expectEndLine, expectEndColumn, expectMessage);
}

void ComputeLocalisation(const std::string& code, size_t offset, int* line, int* column)
{
    int currentColumn = 1;
    int currentLine = 1;

    for (size_t i = 0; i != offset; ++i)
    {
        if (code[i] == '\n')
        {
            currentColumn = 1;
            ++currentLine;
        }
        else
        {
            ++currentColumn;
        }
    }
    *line = currentLine;
    *column = currentColumn;
}

// {start/end offset} ordered by increasing start-location then end-location.
std::vector<std::pair<size_t, size_t>> extractViolationLocations(std::string& code)
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
            std::sort(rangeOffsets.begin(), rangeOffsets.end());
            return rangeOffsets;
        }
        if (startOffset < endOffset)
        {
            code.erase(startOffset, strlen(VIOLATION_START));
            startOffsets.push_back(startOffset);
            findOffset = startOffset;
        }
        else
        {
            code.erase(endOffset, strlen(VIOLATION_END));
            assert(!startOffsets.empty());
            rangeOffsets.push_back({startOffsets.back(), endOffset});
            startOffsets.pop_back();
            findOffset = endOffset;
        }
    }
}

bool lessByLocation(const Violation& lhs, const Violation& rhs)
{
    if (lhs.startLine != rhs.startLine) return lhs.startLine < rhs.startLine;
    if (lhs.startColumn != rhs.startColumn) return lhs.startColumn < rhs.startColumn;
    if (lhs.endLine != rhs.endLine) return lhs.endLine < rhs.endLine;
    if (lhs.endColumn != rhs.endColumn) return lhs.endColumn < rhs.endColumn;
    return lhs.message < rhs.message;
}

void testRuleOnCode(const Twine& filename,
    const std::vector<std::string>& args,
    RuleBase* rule,
    std::string code,
    const std::vector<std::string>& messages)
{
    const auto& rangeOffsets = extractViolationLocations(code);
    ViolationSet violationSet;

    if (!computeViolationSet(filename, rule, code, args, &violationSet))
    {
        FAIL();
        return;
    }
    vector<Violation> violations = violationSet.getViolations();

    ASSERT_THAT(violations.size(), Eq(rangeOffsets.size()));

    std::sort(violations.begin(), violations.end(), &lessByLocation);
    for (size_t i = 0; i != rangeOffsets.size(); ++i)
    {
        const Violation& violation = violations[i];
        const size_t startOffset = rangeOffsets[i].first;
        const size_t endOffset = rangeOffsets[i].second;
        const std::string& message = i < messages.size() ? messages[i] : "";
        int startLine, startColumn, endLine, endColumn;
        ComputeLocalisation(code, startOffset, &startLine, &startColumn);
        ComputeLocalisation(code, endOffset, &endLine, &endColumn);
        validateViolation(violation, startLine, startColumn, endLine, endColumn, message);
    }
}

void testRuleOnCode(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages)
{
    testRuleOnCode("input.c", {}, rule, code, messages);
}

void testRuleOnCXXCode(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages)
{
    testRuleOnCode("input.cpp", {}, rule, code, messages);
}

void testRuleOnCXX11Code(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages)
{
    testRuleOnCode("input.cpp", {"-std=c++11"}, rule, code, messages);
}

void testRuleOnObjCCode(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages)
{
    testRuleOnCode("input.m", {}, rule, code, messages);
}

void testRuleOnCUDACode(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages)
{
    testRuleOnCode("input.cu",
        {"-nocudalib", "--no-cuda-version-check", "-I/tmp/oclint-rules-test/clang"},
        rule, code, messages);
}
