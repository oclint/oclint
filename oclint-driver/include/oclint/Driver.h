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
#include "oclint/ThreadPool.h"

namespace oclint
{

class ViolationSet;

class Driver
{
private:
    ThreadPool _threadPool;

public:
    void run(const clang::tooling::CompilationDatabase &compilationDatabase,
        llvm::ArrayRef<std::string> sourcePaths, oclint::Analyzer &analyzer);

private:
    typedef std::pair<std::string, clang::tooling::CompileCommand> CompileCommandPair;
    typedef std::vector<CompileCommandPair> CompileCommandPairs;

    void runAnalyzerGlobally(const CompileCommandPairs &compileCommands,
        oclint::Analyzer &analyzer);
    void runAnalyzerLocally(const CompileCommandPairs &compileCommands,
        oclint::Analyzer &analyzer);
    void runClangChecker(const CompileCommandPairs &compileCommands);
};

} // end namespace oclint

#endif
