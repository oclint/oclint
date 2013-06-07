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

class Driver
{
public:
    int run(const clang::tooling::CompilationDatabase &compilationDatabase,
        llvm::ArrayRef<std::string> sourcePaths, oclint::Analyzer &analyzer);
};

} // end namespace oclint

#endif
