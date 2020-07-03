#include <clang/AST/Attr.h>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleSet.h"
#include "oclint/helper/EnforceHelper.h"
#include "oclint/util/ASTUtil.h"

using namespace std;
using namespace clang;
using namespace oclint;


class CudaBranchDivergenceRule : public AbstractASTVisitorRule<CudaBranchDivergenceRule>
{
public:
    virtual const string name() const override
    {
        return "branch divergence";
    }

    virtual int priority() const override
    {
        return 10;
    }

    virtual const string category() const override
    {
        return "cuda";
    }

    virtual unsigned int supportedCUDAFunctionAttrs() const override
    {
        return CUDA_GLOBAL;
    }

#ifdef DOCGEN
    virtual const std::string since() const override
    {
        return "0.15";
    }

    virtual const std::string description() const override
    {
        return "Branch divergence is quite expensive and needs to be avoided as much as possible.";
    }

    virtual const std::string fileName() const override
    {
        return "CudaBranchDivergenceRule.cpp";
    }

    virtual const std::string example() const override
    {
        return R"rst(
.. code-block:: cuda

    if (threadIdx.x == 0) {
        foo();
    } else {
        bar();
    }
    // This would cause branch divergence,
    // as while the first thread working on `foo()`,
    // the rest of the threads sit idle;
    // similarly, when the others running `bar()`,
    // the first thread now stall.
    // Go https://bit.ly/2ZxYJVR to read more.

    )rst";
    }
#endif

    bool VisitIfStmt(IfStmt *ifStmt)
    {
        if (ifStmt->getElse() != nullptr) // TODO: we can do smarter than this
        {
            addViolation(ifStmt, this);
        }

        return true;
    }
};


static RuleSet rules(new CudaBranchDivergenceRule());
