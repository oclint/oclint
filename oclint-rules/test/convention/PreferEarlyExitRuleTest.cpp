#include "TestHeaders.h"
#include "rules/convention/PreferEarlyExitRule.cpp"

class PreferEarlyExitRuleTest : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        RuleConfiguration::addConfiguration("MAXIMUM_IF_LENGTH", "3");
    }

    virtual void TearDown() override
    {
        RuleConfiguration::removeAll();
    }

    std::string filler(const std::string& filling, int count)
    {
        std::string result;
        result.reserve(filling.size() * count);
        for (int i = 0; i != count; ++i)
        {
            result += filling;
        }
        return result;
    }
};

struct Loop
{
    const char* name;
    const char* start;
    const char* end;
};

::std::ostream& operator<<(::std::ostream& os, const Loop& loop) {
    return os << loop.name;
}

const Loop loops[] = {
    {
        "for loop",
        "\n"
        "for (int i = 0; i != 5; ++i) {\n",
        "}\n"
    },
    {
        "do/while loop",
        "int i = 0;\n"
        "do {\n",
        "} while (++i != 5);\n"
    },
    {
        "while loop",
        "int i = 0;\n"
        "while (i++ != 5) {\n",
        "}\n"
    }
};

class LoopsTest : public PreferEarlyExitRuleTest,
                  public ::testing::WithParamInterface<Loop>
{
protected:
    std::string getTestCode(int ifStatementBodyLength)
    {
        const Loop loop = GetParam();
        std::string code = "int test(int a) {\n";
        code += loop.start;
        code += "if (a) {\n";
        code += filler("a *= 2;\n", ifStatementBodyLength);
        code += "}\n";
        code += loop.end;
        code += "return a;\n";
        code += "}\n";
        return code;
    }
};

class FlowControlStatementsTest : public PreferEarlyExitRuleTest,
                                  public ::testing::WithParamInterface<const char*>
{
protected:
    std::string getTestCode(int ifStatementBodyLength)
    {
        std::string code = "int test(int a) {\n";
        code += "void const* label_ptr = &&LABEL;\n";
        code += "LABEL:\n";
        code += "for (;;) {\n";
        code += "  int i = 2;\n";
        code += "  if (a) {\n";
        code += filler("i *= 2;\n", ifStatementBodyLength);
        code += "  }\n";
        code += GetParam();
        code += "\n}\n";
        code += "}\n";
        return code;
    }
};

TEST_F(PreferEarlyExitRuleTest, PropertyTest)
{
    PreferEarlyExitRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("use early exits and continue", rule.name());
}

TEST_P(LoopsTest, LoopWithShortIf)
{
    std::string code = getTestCode(1);
    testRuleOnCode(new PreferEarlyExitRule(), code);
}

TEST_P(LoopsTest, LoopWithLongIf)
{
    std::string code = getTestCode(2);
    testRuleOnCode(new PreferEarlyExitRule(), code, 0, 4, 1, 7, 1,
                   PreferEarlyExitRule::getMessage());
}

TEST_P(FlowControlStatementsTest, FunctionWithShortIf)
{
    std::string code = getTestCode(1);
    testRuleOnCode(new PreferEarlyExitRule(), code);
}

TEST_P(FlowControlStatementsTest, FunctionWithLongIf)
{
    std::string code = getTestCode(2);
    testRuleOnCode(new PreferEarlyExitRule(), code, 0, 6, 3, 9, 3,
                   PreferEarlyExitRule::getMessage());
}

TEST_F(PreferEarlyExitRuleTest, LongIfAndReturnInsideBlock)
{
    std::string code = "int test(int a) {\n";
    code += "  int i = 2;\n";
    code += "  if (a) {\n";
    code += filler("i *= 2;\n", 2);
    code += "  }\n";
    code += "  return i;\n";
    code += "}\n";
    testRuleOnCode(new PreferEarlyExitRule(), code, 0, 3, 3, 6, 3,
                   PreferEarlyExitRule::getMessage());
}

INSTANTIATE_TEST_CASE_P(PreferEarlyExit, LoopsTest, ::testing::ValuesIn(loops));
INSTANTIATE_TEST_CASE_P(
    PreferEarlyExit, FlowControlStatementsTest,
    ::testing::Values("break;", "continue;", "goto LABEL;", "goto *label_ptr;"));

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
