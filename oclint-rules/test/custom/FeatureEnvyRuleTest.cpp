#include "TestHeaders.h"

#include "rules/custom/FeatureEnvyRule.cpp"

TEST(FeatureEnvyRuleTest, PropertyTest)
{
    FeatureEnvyRule rule;
    EXPECT_EQ(3, rule.priority());
    EXPECT_EQ("feature envy", rule.name());
}

TEST(FeatureEnvyRuleTest, ObjCImplementationNoMethod)
{
  testRuleOnObjCCode(new FeatureEnvyRule(), "@interface a\n@end\n@implementation a\n@end\n");
}

TEST(FeatureEnvyRuleTest, ObjcMethodMessageOther)
{
    testRuleOnObjCCode(new FeatureEnvyRule(),
      "@interface B \n"
      "@end \n"

      "@implementation B \n"
      "- (void) f { \n"
      "} \n"
      "@end \n"

      "@interface A \n"
      "@end \n"

      "@implementation A \n"
      "- (void)f { \n"
      "    B *b; \n"
      "    [b f]; \n"
      "} \n"
      "@end\n",
      0, 10, 1, 13, 1, "Method f messages B more than self.");
}

TEST(FeatureEnvyRuleTest, ObjcMethodMessageSelf)
{
    testRuleOnObjCCode(new FeatureEnvyRule(),
      "@interface B \n"
      "@end \n"

      "@implementation B \n"
      "- (void) f { \n"
      "} \n"
      "@end \n"

      "@interface A \n"
      "@end \n"

      "@implementation A \n"
      "- (void)f { \n"
      "    [self f]; \n"
      "} \n"
      "@end\n");
}

TEST(FeatureEnvyRuleTest, ObjcMethodMessageSelfAndOtherEqually)
{
    testRuleOnObjCCode(new FeatureEnvyRule(),
      "@interface B \n"
      "@end \n"

      "@implementation B \n"
      "- (void) f { \n"
      "} \n"
      "@end \n"

      "@interface A \n"
      "@end \n"

      "@implementation A \n"
      "- (void)f { \n"
      "    B *b; \n"
      "    [b f]; \n"
      "    [self f]; \n"
      "} \n"
      "@end\n");
}

TEST(FeatureEnvyRuleTest, ObjcMethodMessageSelfLessThanOther)
{
    testRuleOnObjCCode(new FeatureEnvyRule(),
      "@interface B \n"
      "@end \n"

      "@implementation B \n"
      "- (void) f { \n"
      "} \n"
      "@end \n"

      "@interface A \n"
      "@end \n"

      "@implementation A \n"
      "- (void)f { \n"
      "    B *b; \n"
      "    [b f]; \n"
      "    [self f]; \n"
      "    [b f]; \n"
      "} \n"
      "@end\n",
      0, 10, 1, 15, 1, "Method f messages B more than self.");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}