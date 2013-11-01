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

class COnlyASTRule : public AbstractASTVisitorRule<COnlyASTRule>
{
public:
    virtual int supportedLanguages()
    {
        return LANG_C;
    }

    virtual const string name() const
    {
        return "c only ast rule";
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

TEST(COnlyASTRuleTest, PropertyTest)
{
    COnlyASTRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("c only ast rule", rule.name());
}

TEST(COnlyASTRuleTest, RunOnC)
{
    testRuleOnCode(new COnlyASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(COnlyASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new COnlyASTRule(), "void a() {}");
}

TEST(COnlyASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new COnlyASTRule(), "void a() {}");
}

TEST(COnlyASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new COnlyASTRule(), "void a() {}");
}

class CXXOnlyASTRule : public AbstractASTVisitorRule<CXXOnlyASTRule>
{
public:
    virtual int supportedLanguages()
    {
        return LANG_CXX;
    }

    virtual const string name() const
    {
        return "cpp only ast rule";
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

TEST(CXXOnlyASTRuleTest, PropertyTest)
{
    CXXOnlyASTRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("cpp only ast rule", rule.name());
}

TEST(CXXOnlyASTRuleTest, RunOnC)
{
    testRuleOnCode(new CXXOnlyASTRule(), "void a() {}");
}

TEST(CXXOnlyASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new CXXOnlyASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(CXXOnlyASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new CXXOnlyASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(CXXOnlyASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new CXXOnlyASTRule(), "void a() {}");
}

class ObjCOnlyASTRule : public AbstractASTVisitorRule<ObjCOnlyASTRule>
{
public:
    virtual int supportedLanguages()
    {
        return LANG_OBJC;
    }

    virtual const string name() const
    {
        return "objc only ast rule";
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

TEST(ObjCOnlyASTRuleTest, PropertyTest)
{
    ObjCOnlyASTRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("objc only ast rule", rule.name());
}

TEST(ObjCOnlyASTRuleTest, RunOnC)
{
    testRuleOnCode(new ObjCOnlyASTRule(), "void a() {}");
}

TEST(ObjCOnlyASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new ObjCOnlyASTRule(), "void a() {}");
}

TEST(ObjCOnlyASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new ObjCOnlyASTRule(), "void a() {}");
}

TEST(ObjCOnlyASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new ObjCOnlyASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

class MultipleLanguagesSupportASTRule : public AbstractASTMatcherRule
{
protected:
    virtual int supportedLanguages()
    {
        return LANG_OBJC | LANG_C;
    }

public:

    virtual const string name() const
    {
        return "multiple languages support ast rule";
    }

    virtual int priority() const
    {
        return 0;
    }

    virtual void callback(const clang::ast_matchers::MatchFinder::MatchResult &result)
    {
        addViolation(result.Nodes.getNodeAs<Decl>("testDecl"), this);
    }

    virtual void setUpMatcher()
    {
        addMatcher(clang::ast_matchers::decl().bind("testDecl"));
    }
};

TEST(MultipleLanguagesSupportASTRuleTest, PropertyTest)
{
    MultipleLanguagesSupportASTRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("multiple languages support ast rule", rule.name());
}

TEST(MultipleLanguagesSupportASTRuleTest, RunOnC)
{
    testRuleOnCode(new MultipleLanguagesSupportASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(MultipleLanguagesSupportASTRuleTest, RunOnCXX)
{
    testRuleOnCXXCode(new MultipleLanguagesSupportASTRule(), "void a() {}");
}

TEST(MultipleLanguagesSupportASTRuleTest, RunOnCXX11)
{
    testRuleOnCXX11Code(new MultipleLanguagesSupportASTRule(), "void a() {}");
}

TEST(MultipleLanguagesSupportASTRuleTest, RunOnObjC)
{
    testRuleOnObjCCode(new MultipleLanguagesSupportASTRule(), "void a() {}", 0, 1, 1, 1, 11);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
