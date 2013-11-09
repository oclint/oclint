#include "TestHeaders.h"

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/AbstractASTMatcherRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class DefaultASTRule : public AbstractASTVisitorRule<DefaultASTRule>
{
public:
    virtual const string name() const
    {
        return "default ast rule";
    }

    virtual int priority() const
    {
        return 0;
    }

    bool VisitDecl(clang::Decl *decl)
    {
        addViolation(decl, this);
        return true;
    }

    bool VisitStmt(clang::Stmt *stmt)
    {
        addViolation(stmt, this);
        return true;
    }
};

TEST(DefaultASTRuleTest, PropertyTest)
{
    DefaultASTRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("default ast rule", rule.name());
}

TEST(DefaultASTRuleTest, RunOnC)
{
    testRuleOnCode(new DefaultASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(DefaultASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new DefaultASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(DefaultASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new DefaultASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(DefaultASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new DefaultASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

class LanguageSelectionASTRule : public AbstractASTVisitorRule<DefaultASTRule>
{
private:
    unsigned _supportedLanguages;

public:
    explicit LanguageSelectionASTRule(unsigned languageOpts)
    {
        _supportedLanguages = languageOpts;
    }

    virtual unsigned int supportedLanguages() const
    {
        return _supportedLanguages;
    }

    virtual const std::string name() const
    {
        return "";
    }

    virtual int priority() const
    {
        return 0;
    }

    bool VisitDecl(clang::Decl *decl)
    {
        addViolation(decl, this);
        return true;
    }

    bool VisitStmt(clang::Stmt *stmt)
    {
        addViolation(stmt, this);
        return true;
    }
};

TEST(COnlyASTRuleTest, RunOnC)
{
    testRuleOnCode(new LanguageSelectionASTRule(LANG_C), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(COnlyASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new LanguageSelectionASTRule(LANG_C), "void a() {}");
}

TEST(COnlyASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new LanguageSelectionASTRule(LANG_C), "void a() {}");
}

TEST(COnlyASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new LanguageSelectionASTRule(LANG_C), "void a() {}");
}

TEST(CXXOnlyASTRuleTest, RunOnC)
{
    testRuleOnCode(new LanguageSelectionASTRule(LANG_CXX), "void a() {}");
}

TEST(CXXOnlyASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new LanguageSelectionASTRule(LANG_CXX), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(CXXOnlyASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new LanguageSelectionASTRule(LANG_CXX), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(CXXOnlyASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new LanguageSelectionASTRule(LANG_CXX), "void a() {}");
}

TEST(ObjCOnlyASTRuleTest, RunOnC)
{
    testRuleOnCode(new LanguageSelectionASTRule(LANG_OBJC), "void a() {}");
}

TEST(ObjCOnlyASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new LanguageSelectionASTRule(LANG_OBJC), "void a() {}");
}

TEST(ObjCOnlyASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new LanguageSelectionASTRule(LANG_OBJC), "void a() {}");
}

TEST(ObjCOnlyASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new LanguageSelectionASTRule(LANG_OBJC), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(MultipleLanguagesSupportASTRuleTest, RunOnC)
{
    testRuleOnCode(new LanguageSelectionASTRule(LANG_C | LANG_OBJC), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(MultipleLanguagesSupportASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new LanguageSelectionASTRule(LANG_C | LANG_OBJC), "void a() {}");
}

TEST(MultipleLanguagesSupportASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new LanguageSelectionASTRule(LANG_C | LANG_OBJC), "void a() {}");
}

TEST(MultipleLanguagesSupportASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new LanguageSelectionASTRule(LANG_C | LANG_OBJC), "void a() {}", 0, 1, 1, 1, 11);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
