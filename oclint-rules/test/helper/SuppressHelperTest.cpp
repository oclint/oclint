#include "TestRuleOnCode.h"

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/AbstractSourceCodeReaderRule.h"

using namespace std;
using namespace clang;
using namespace oclint;

class SuppressHelperTestASTRule : public AbstractASTVisitorRule<SuppressHelperTestASTRule>
{
public:
    virtual const string name() const override
    {
        return "test ast rule";
    }

    virtual int priority() const override
    {
        return 0;
    }

    virtual const string category() const override
    {
        return "test";
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

TEST(SuppressHelperTestASTRuleTest, PropertyTest)
{
    SuppressHelperTestASTRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("test ast rule", rule.name());
}

TEST(SuppressHelperTestASTRuleTest, NoSuppress)
{
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {}", 0, 1, 1, 1, 11);
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {}", 1, 1, 10, 1, 11);
}

TEST(SuppressHelperTestASTRuleTest, SimpleSuppressEntireMethod)
{
    testRuleOnCode(new SuppressHelperTestASTRule(),
        "void __attribute__((annotate(\"oclint:suppress[test ast rule]\"))) a() { int i = 1; }");
}

TEST(SuppressHelperTestASTRuleTest, SimpleSuppressEntireCXXClass)
{
    testRuleOnCXXCode(new SuppressHelperTestASTRule(),
        "class __attribute__((annotate(\"oclint:suppress[test ast rule]\"))) c { void a() { int i = 1; } };");
}

TEST(SuppressHelperTestASTRuleTest, CXXClassSuppressOnClass)
{
    testRuleOnCXXCode(new SuppressHelperTestASTRule(),
        "class __attribute__((annotate(\"oclint:suppress\"))) c { void a(); };\n\
void c::a() {}\n",
        1, 2, 13, 2, 14);
}

TEST(SuppressHelperTestASTRuleTest, CXXClassSuppressOnMethod)
{
    testRuleOnCXXCode(new SuppressHelperTestASTRule(),
        "class c { void a(); };\n\
void __attribute__((annotate(\"oclint:suppress\"))) c::a() { int i = 1; }\n",
        1, 1, 11, 1, 18);
}

TEST(SuppressHelperTestASTRuleTest, ObjCContainerSuppressOnAtInterface)
{
    testRuleOnObjCCode(new SuppressHelperTestASTRule(),
        "__attribute__((annotate(\"oclint:suppress\"))) @interface a {\nint i;\n}\n@end");
}

TEST(SuppressHelperTestASTRuleTest, ObjCContainerSuppressOnAtProtocol)
{
    testRuleOnObjCCode(new SuppressHelperTestASTRule(),
        "__attribute__((annotate(\"oclint:suppress\"))) @protocol a\n-(void)m;\n@end");
}

// TEST(SuppressHelperTestASTRuleTest, ObjCContainerSuppressOnAtImplementation)
// Prefix attribute must be followed by an interface or protocol,
// in other words, adding attribute to @implementation is prohibited.

TEST(SuppressHelperTestASTRuleTest, ObjCContainerSuppressOnMethod)
{
    testRuleOnObjCCode(new SuppressHelperTestASTRule(),
        "__attribute__((annotate(\"oclint:suppress\"))) @interface a {\nint i;\n}\n-(void)m __attribute__((annotate(\"oclint:suppress\")));\n@end\n\
/* __attribute__((annotate(\"oclint:suppress\"))) */ @implementation a\n-(void)m __attribute__((annotate(\"oclint:suppress\"))) {}\n@end\n",
        0, 6, 52, 8, 1);
}

TEST(SuppressHelperTestASTRuleTest, SimpleNOLINT)
{
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLINT");
#if defined(__APPLE__) || defined(__MACH__)
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLINT");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLint");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLint");
#endif
}

TEST(SuppressHelperTestASTRuleTest, MultipleLineNOLINT)
{
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() { //!OCLINT\n if (1) {//!OCLINT\n}}");
#if defined(__APPLE__) || defined(__MACH__)
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() { //! OCLINT\n if (1) {//! OCLINT\n}}");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() { //!OCLint\n if (1) {//!OCLint\n}}");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() { //! OCLint\n if (1) {//! OCLint\n}}");
#endif
}

TEST(SuppressHelperTestASTRuleTest, CommentWithDescriptionNOLINT)
{
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLINT(reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLINT[reason for suppressing this is blahblah]");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLINT:reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLINT     ");
#if defined(__APPLE__) || defined(__MACH__)
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLINT(reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLINT[reason for suppressing this is blahblah]");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLINT:reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLINT     ");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLint(reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLint[reason for suppressing this is blahblah]");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLint:reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!OCLint     ");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLint(reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLint[reason for suppressing this is blahblah]");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLint:reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //! OCLint     ");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!  OCLint(reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!   OCLint[reason for suppressing this is blahblah]");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!    OCLint:reason for suppressing this is blahblah)");
    testRuleOnCode(new SuppressHelperTestASTRule(), "void a() {} //!     OCLint     ");
#endif
}

class SuppressHelperTestSourceCodeReaderRule : public AbstractSourceCodeReaderRule
{
public:
    virtual const string name() const override
    {
        return "test source code rule";
    }

    virtual int priority() const override
    {
        return 0;
    }

    virtual const string category() const override
    {
        return "test";
    }

    void eachLine(int lineNumber, string line) override
    {
        addViolation(lineNumber, 1, lineNumber, line.size(), this, "");
    }
};

TEST(SuppressHelperTestSourceCodeReaderRuleTest, PropertyTest)
{
    SuppressHelperTestSourceCodeReaderRule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("test source code rule", rule.name());
}

TEST(SuppressHelperTestSourceCodeReaderRuleTest, NoSuppress)
{
    testRuleOnCode(new SuppressHelperTestSourceCodeReaderRule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(SuppressHelperTestSourceCodeReaderRuleTest, SuppressByComment)
{
    testRuleOnCode(new SuppressHelperTestSourceCodeReaderRule(), "void a() {} //!OCLINT");
#if defined(__APPLE__) || defined(__MACH__)
    testRuleOnCode(new SuppressHelperTestSourceCodeReaderRule(), "void a() {} //! OCLINT");
    testRuleOnCode(new SuppressHelperTestSourceCodeReaderRule(), "void a() {} //!OCLint");
    testRuleOnCode(new SuppressHelperTestSourceCodeReaderRule(), "void a() {} //! OCLint");
#endif
}

TEST(SuppressHelperTestSourceCodeReaderRuleTest, SuppressEntireMethod)
{
    testRuleOnCode(new SuppressHelperTestSourceCodeReaderRule(),
        "void __attribute__((annotate(\"oclint:suppress[test source code rule]\"))) a()\n{\nint i = 1;\n}\n");
}

TEST(SuppressHelperTestSourceCodeReaderRuleTest, SimpleSuppressEntireCXXClass)
{
    testRuleOnCXXCode(new SuppressHelperTestSourceCodeReaderRule(),
        "class __attribute__((annotate(\"oclint:suppress[test source code rule]\"))) c\n{\nvoid a()\n{\nint i = 1;\n}\n};\n");
}

TEST(SuppressHelperTestSourceCodeReaderRuleTest, ObjCContainerSuppressOnAtInterface)
{
    testRuleOnObjCCode(new SuppressHelperTestSourceCodeReaderRule(),
        "__attribute__((annotate(\"oclint:suppress\"))) @interface a {\nint i;\n}\n@end");
}
