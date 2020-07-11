#include "TestRuleOnCode.h"

#include "oclint/AbstractASTVisitorRule.h"

using namespace std;
using namespace oclint;

class ImplicitNoCUDARule : public AbstractASTVisitorRule<ImplicitNoCUDARule> {
public:
    virtual const string name() const override {
        return "implicit no cuda rule";
    }

    virtual int priority() const override {
        return 0;
    }

    virtual const string category() const override {
        return "test";
    }

    bool VisitDecl(clang::Decl *decl) {
        addViolation(decl, this);
        return true;
    }
};

TEST(ImplicitNoCUDARuleTest, PropertyTest) {
    ImplicitNoCUDARule rule;
    EXPECT_EQ(0, rule.priority());
    EXPECT_EQ("implicit no cuda rule", rule.name());
}

TEST(ImplicitNoCUDARuleTest, RunOnC) {
    testRuleOnCode(new ImplicitNoCUDARule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(ImplicitNoCUDARuleTest, RunOnCXX) {
    testRuleOnCXXCode(new ImplicitNoCUDARule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(ImplicitNoCUDARuleTest, RunOnCXX11) {
    testRuleOnCXX11Code(new ImplicitNoCUDARule(), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(ImplicitNoCUDARuleTest, RunOnCUDA) {
    testRuleOnCUDACode(new ImplicitNoCUDARule(),
        "__global__ void kernel() {} void host() {}", 0, 1, 29, 1, 42);
}

class ExplicitCUDARule : public AbstractASTVisitorRule<ExplicitCUDARule> {
private:
    unsigned _supportedCUDAFunctionFlags;

public:
    explicit ExplicitCUDARule(unsigned cudaSupportFlags) {
        _supportedCUDAFunctionFlags = cudaSupportFlags;
    }

    virtual unsigned int supportedCUDAFunctionAttrs() const override
    {
        return _supportedCUDAFunctionFlags;
    }

    virtual const string name() const override {
        return "explicit cuda rule";
    }

    virtual int priority() const override {
        return 0;
    }

    virtual const string category() const override {
        return "test";
    }

    bool VisitDecl(clang::Decl *decl) {
        addViolation(decl, this);
        return true;
    }
};

TEST(ExplicitCUDARuleTest, RunOnCHostFunctions) {
    testRuleOnCode(new ExplicitCUDARule(CUDA_HOST), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(ExplicitCUDARuleTest, RunOnCXXHostFunctions) {
    testRuleOnCXXCode(new ExplicitCUDARule(CUDA_HOST), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(ExplicitCUDARuleTest, RunOnCUDAHostFunctions) {
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_HOST),
        "__global__ void kernel() {}");
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_HOST),
        "void host() {}", 0, 1, 1, 1, 14);
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_HOST),
        "__global__ void kernel() {} void host() {}", 0, 1, 29, 1, 42);
}

TEST(ExplicitCUDARuleTest, RunOnCGlobalFunctions) {
    testRuleOnCode(new ExplicitCUDARule(CUDA_GLOBAL), "void a() {}");
}

TEST(ExplicitCUDARuleTest, RunOnCXXGlobalFunctions) {
    testRuleOnCXXCode(new ExplicitCUDARule(CUDA_GLOBAL), "void a() {}");
}

TEST(ExplicitCUDARuleTest, RunOnCUDAGlobalFunctions) {
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_GLOBAL),
        "__global__ void kernel() {}", 0, 1, 1, 1, 27);
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_GLOBAL),
        "void host() {}");
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_GLOBAL),
        "__global__ void kernel() {} void host() {}", 0, 1, 1, 1, 27);
}

TEST(ExplicitCUDARuleTest, RunOnCBoth) {
    testRuleOnCode(new ExplicitCUDARule(CUDA_HOST | CUDA_GLOBAL), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(ExplicitCUDARuleTest, RunOnCXXBoth) {
    testRuleOnCXXCode(new ExplicitCUDARule(CUDA_HOST | CUDA_GLOBAL), "void a() {}", 0, 1, 1, 1, 11);
}

TEST(ExplicitCUDARuleTest, RunOnCUDABoth) {
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_HOST | CUDA_GLOBAL),
        "__global__ void kernel() {}", 0, 1, 1, 1, 27);
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_HOST | CUDA_GLOBAL),
        "void host() {}", 0, 1, 1, 1, 14);
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_HOST | CUDA_GLOBAL),
        "__global__ void kernel() {} void host() {}", 0, 1, 1, 1, 27);
    testRuleOnCUDACode(new ExplicitCUDARule(CUDA_HOST | CUDA_GLOBAL),
        "__global__ void kernel() {} void host() {}", 1, 1, 29, 1, 42);
}
