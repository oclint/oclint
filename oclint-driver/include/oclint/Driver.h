#ifndef OCLINT_DRIVER_H
#define OCLINT_DRIVER_H

#include <string>

namespace llvm
{
    template<typename T> class ArrayRef;
}

namespace clang
{
namespace tooling
{
    class CompilationDatabase;
}
}

#include "oclint/Analyzer.h"

namespace oclint
{

class ViolationSet;

class Driver
{
public:
    void run(const clang::tooling::CompilationDatabase &compilationDatabase,
        llvm::ArrayRef<std::string> sourcePaths,
        oclint::Analyzer &analyzer,
        ViolationSet &errorSet,
        ViolationSet &warningSet);
};

} // end namespace oclint

#endif
