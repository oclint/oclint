#include "TestHeaders.h"

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/AbstractSourceCodeReaderRule.h"
#include "oclint/helper/AttributeHelper.h"

using namespace std;
using namespace clang;
using namespace oclint;

class AttributeHelperTestCallRule : public AbstractASTVisitorRule<AttributeHelperTestCallRule>
{
public:
    virtual const string name() const
    {
        return "test ast rule";
    }

    virtual const string attributeName() const {
        return "test attribute";
    }

    virtual int priority() const
    {
        return 0;
    }

    bool VisitObjCMessageExpr(ObjCMessageExpr* expr)
    {
        const auto method = expr->getMethodDecl();
        if(declHasActionAttribute(method, "enforce", *this)) {
            addViolation(expr, this);
        }
        return true;
    }

    bool VisitCallExpr(CallExpr* expr)
    {
        if(declHasActionAttribute(expr->getCalleeDecl(), "enforce", *this)) {
            addViolation(expr, this);
        }
        return true;
    }
};

class AttributeHelperTestCallCommentRule : public AbstractASTVisitorRule<AttributeHelperTestCallCommentRule>
{
public:
    virtual const string name() const
    {
        return "test ast rule";
    }

    virtual const string attributeName() const {
        return "test attribute";
    }

    virtual int priority() const
    {
        return 0;
    }
    bool VisitCallExpr(CallExpr* expr)
    {
        std::string comment;
        if(declHasActionAttribute(expr->getCalleeDecl(), "enforce", *this, &comment)) {
            if(!comment.empty()) {
                addViolation(expr, this, comment);
            }
            else {
                addViolation(expr, this, "no comment");
            }
        }
        return true;
    }
};

TEST(AttributeHelperTestCallRuleTest, PropertyTest)
{
    AttributeHelperTestCallRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("test ast rule", rule.name());
    EXPECT_EQ("test attribute", rule.attributeName());
}

TEST(AttributeHelperTestCallRuleTest, NoAttributeFunctionCall)
{
    AttributeHelperTestCallRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a();
            void b() {
                a();
            }
        )END"
    );
}

TEST(AttributeHelperTestCallRuleTest, AttributeFunctionCall)
{
    AttributeHelperTestCallRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[test attribute]")));
            void b() {
                a();
            }
        )END",
        0, 4, 17, 4, 19);
}

TEST(AttributeHelperTestCallRuleTest, IncorrectAction)
{
    AttributeHelperTestCallRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:wrong[test attribute]")));
            void b() {
                a();
            }
        )END"
    );
}

TEST(AttributeHelperTestCallRuleTest, BadFormat)
{
    AttributeHelperTestCallRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint-sfdklj]")));
            void b() {
                a();
            }
        )END"
    );
}

TEST(AttributeHelperTestCallRuleTest, IncorrectAttribute)
{
    AttributeHelperTestCallRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[wrong attribute]")));
            void b() {
                a();
            }
        )END"
    );
}

TEST(AttributeHelperTestCallRuleTest, ExtraFunctionDeclBefore)
{
    AttributeHelperTestCallRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a();
            void a() __attribute__((annotate("oclint:enforce[test attribute]")));
            void b() {
                a();
            })END",
        0, 5, 17, 5, 19);
}

TEST(AttributeHelperTestCallRuleTest, ExtraFunctionDeclAfter)
{
    AttributeHelperTestCallRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[test attribute]")));
            void a();
            void b() {
                a();
            })END",
        0, 5, 17, 5, 19);
}

TEST(AttributeHelperTestCallRuleTest, MethodNoAttribute)
{
    AttributeHelperTestCallRule rule;
    testRuleOnObjCCode(&rule,
       R"END(
            __attribute__((objc_root_class))
            @interface SomeObject 
            @end

            @implementation SomeObject
            - (void)foo {
                [self foo];
            }
            @end
        )END"
    );
}

TEST(AttributeHelperTestCallRuleTest, MethodAttribute)
{
    AttributeHelperTestCallRule rule;
    testRuleOnObjCCode(&rule,
       R"END(
            __attribute__((objc_root_class))
            @interface SomeObject
            - (void)foo __attribute__((annotate("oclint:enforce[test attribute]")));
            @end

            @implementation SomeObject
            - (void)foo {
                [self foo];
            }
            @end
        )END",
    0, 9, 17, 9, 26);
}

TEST(AttributeHelperTestCallRuleTest, CategoryAttribute)
{
    AttributeHelperTestCallRule rule;
    testRuleOnObjCCode(&rule,
       R"END(
            __attribute__((objc_root_class))
            @interface SomeObject
            - (void)foo;
            @end

            @interface SomeObject (Category)
            - (void)foo __attribute__((annotate("oclint:enforce[test attribute]")));
            @end

            @implementation SomeObject
            - (void)foo {
                [self foo];
            }
            @end
        )END",
    0, 13, 17, 13, 26);
}

TEST(AttributeHelperTestCallRuleTest, PropertyAttribute)
{
    AttributeHelperTestCallRule rule;
    testRuleOnObjCCode(&rule,
       R"END(
            __attribute__((objc_root_class))
            @interface SomeObject
            @property (strong, nonatomic) SomeObject* foo __attribute__((annotate("oclint:enforce[test attribute]")));
            @end

            @implementation SomeObject
            - (void)bar {
                self.foo = 0;
            }
            @end
        )END",
    0, 9, 22, 9, 22);
}


TEST(AttributeHelperTestCallRuleTest, ProtocolAttribute)
{
    AttributeHelperTestCallRule rule;
    testRuleOnObjCCode(&rule,
       R"END(
            @protocol SomeProtocol
            - (void)foo __attribute__((annotate("oclint:enforce[test attribute]")));
            @end

            __attribute__((objc_root_class))
            @interface SomeObject <SomeProtocol>
            @end

            @implementation SomeObject
            - (void)foo {
                [self foo];
            }
            @end
        )END",
    0, 12, 17, 12, 26);
}

TEST(AttributeHelperTestCallRuleTest, CategoryPropertyAttribute)
{
    AttributeHelperTestCallRule rule;
    testRuleOnObjCCode(&rule,
       R"END(
            __attribute__((objc_root_class))
            @interface SomeObject
            @property (strong, nonatomic) SomeObject* foo;
            @end

            @interface SomeObject (SomeCategory)
            @property (strong, nonatomic) SomeObject* foo __attribute__((annotate("oclint:enforce[test attribute]")));
            @end

            @implementation SomeObject
            - (void)bar {
                self.foo = 0;
            }
            @end
        )END",
    0, 13, 22, 13, 22);
}

TEST(AttributeHelperTestCallRuleTest, CategoryPropertyRedeclaredAttribute)
{
    AttributeHelperTestCallRule rule;
    testRuleOnObjCCode(&rule,
       R"END(
            __attribute__((objc_root_class))
            @interface SomeObject
            @property (strong, nonatomic) SomeObject* foo __attribute__((annotate("oclint:enforce[test attribute]")));
            @end

            @interface SomeObject (SomeCategory)
            @property (strong, nonatomic) SomeObject* foo;
            @end

            @implementation SomeObject
            - (void)bar {
                self.foo = 0;
            }
            @end
        )END",
    0, 13, 22, 13, 22);
}

TEST(AttributeHelperTestCallRuleTest, CorrectComment)
{
    AttributeHelperTestCallCommentRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[test attribute][foo]")));
            void b() {
                a();
            }
        )END"
    , 0, 4, 17, 4, 19, "foo");
}

TEST(AttributeHelperTestCallRuleTest, MissingClose)
{
    AttributeHelperTestCallCommentRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[test attribute][foo")));
            void b() {
                a();
            }
        )END"
    , 0, 4, 17, 4, 19, "no comment");
}

TEST(AttributeHelperTestCallRuleTest, ExtraClose)
{
    AttributeHelperTestCallCommentRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[test attribute][foo]]")));
            void b() {
                a();
            }
        )END"
    , 0, 4, 17, 4, 19, "foo]");
}

TEST(AttributeHelperTestCallRuleTest, NoComment)
{
    AttributeHelperTestCallCommentRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[test attribute]")));
            void b() {
                a();
            }
        )END"
    , 0, 4, 17, 4, 19, "no comment");
}

TEST(AttributeHelperTestCallRuleTest, ParensComment)
{
    AttributeHelperTestCallCommentRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[test attribute](comment)")));
            void b() {
                a();
            }
        )END"
    , 0, 4, 17, 4, 19, "no comment");
}

TEST(AttributeHelperTestCallRuleTest, ExtraJunk)
{
    AttributeHelperTestCallCommentRule rule;
    testRuleOnCode(&rule,
        R"END(
            void a() __attribute__((annotate("oclint:enforce[test attribute]sdkjfl")));
            void b() {
                a();
            }
        )END"
    , 0, 4, 17, 4, 19, "no comment");
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
