/*-
 * This code is derived from
 * http://llvm.org/svn/llvm-project/cfe/trunk/lib/Tooling/Tooling.cpp
 * with the following license:
 *
 * University of Illinois/NCSA
 * Open Source License
 *
 * Copyright (c) 2003-2013 University of Illinois at Urbana-Champaign.
 * All rights reserved.
 *
 * Developed by:
 *
 *     LLVM Team
 *
 *     University of Illinois at Urbana-Champaign
 *
 *     http://llvm.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal with
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimers.
 *
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimers in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the names of the LLVM Team, University of Illinois at
 *       Urbana-Champaign, nor the names of its contributors may be used to
 *       endorse or promote products derived from this Software without specific
 *       prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
 * SOFTWARE.
 */
#include "oclint/Driver.h"

#include <unistd.h>

#include <sstream>

#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Option/ArgList.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/raw_ostream.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Driver/Compilation.h>
#include <clang/Driver/Driver.h>
#include <clang/Driver/Job.h>
#include <clang/Driver/Tool.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendDiagnostic.h>
#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include "oclint/CompilerInstance.h"
#include "oclint/DiagnosticDispatcher.h"
#include "oclint/GenericException.h"
#include "oclint/Logger.h"
#include "oclint/Options.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

typedef std::vector<std::pair<std::string, clang::tooling::CompileCommand>> CompileCommandPairs;

static clang::driver::Driver *newDriver(clang::DiagnosticsEngine *diagnostics,
    const char *binaryName)
{
    clang::driver::Driver *driver =
        new clang::driver::Driver(binaryName, llvm::sys::getDefaultTargetTriple(), *diagnostics);
    driver->setTitle("OCLint");
    return driver;
}

static std::string compilationJobsToString(const clang::driver::JobList &jobs)
{
    clang::SmallString<256> errorMsg;
    llvm::raw_svector_ostream errorStream(errorMsg);
    jobs.Print(errorStream, "; ", true);
    return errorStream.str();
}

static const llvm::opt::ArgStringList *getCC1Arguments(clang::driver::Compilation *compilation)
{
    const clang::driver::JobList &jobList = compilation->getJobs();
    if (jobList.size() != 1 || !clang::isa<clang::driver::Command>(*jobList.begin()))
    {
        throw oclint::GenericException("one compiler command contains multiple jobs:\n" +
            compilationJobsToString(jobList) + "\n");
    }

    const clang::driver::Command &cmd = clang::cast<clang::driver::Command>(*jobList.begin());
    if (llvm::StringRef(cmd.getCreator().getName()) != "clang")
    {
        throw oclint::GenericException("expected a clang compiler command");
    }

    return &cmd.getArguments();
}

static clang::CompilerInvocation *newInvocation(clang::DiagnosticsEngine *diagnostics,
    const llvm::opt::ArgStringList &argStringList)
{
    assert(!argStringList.empty() && "Must at least contain the program name!");
    auto invocation = new clang::CompilerInvocation;
    clang::CompilerInvocation::CreateFromArgs(*invocation,
        argStringList.data() + 1, argStringList.data() + argStringList.size(), *diagnostics);
    invocation->getFrontendOpts().DisableFree = false;
    return invocation;
}

static void constructCompileCommands(
    CompileCommandPairs &compileCommands,
    const clang::tooling::CompilationDatabase &compilationDatabase,
    llvm::ArrayRef<std::string> sourcePaths)
{
    for (const auto &sourcePath : sourcePaths)
    {
        std::string filePath(clang::tooling::getAbsolutePath(sourcePath));
        std::vector<clang::tooling::CompileCommand> compileCmdsForFile =
            compilationDatabase.getCompileCommands(filePath);
        if (compileCmdsForFile.empty())
        {
            llvm::errs() << "Skipping " << filePath << ". Compile command not found.\n";
            continue;
        }
        for (auto &compileCommand : compileCmdsForFile)
        {
            compileCommands.push_back(std::make_pair(filePath, compileCommand));
        }
    }
}

static clang::CompilerInvocation *newCompilerInvocation(std::string &mainExecutable,
    std::vector<std::string> &commandLine, bool runClangChecker = false)
{
    assert(!commandLine.empty() && "Command line must not be empty!");
    commandLine[0] = mainExecutable;

    std::vector<const char*> argv;
    int start = 0, end = commandLine.size();
    if (runClangChecker)
    {
        argv.push_back(commandLine[0].c_str());
        argv.push_back("--analyze");
        start = 1;
        end -= 1;
    }
    for (int cmdIndex = start; cmdIndex != end; cmdIndex++)
    {
        if (commandLine[cmdIndex] != "-gmodules")
        {
            argv.push_back(commandLine[cmdIndex].c_str());
        }
    }
    argv.push_back("-D__OCLINT__");

    // create diagnostic engine
    llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagOpts =
        new clang::DiagnosticOptions();
    clang::DiagnosticsEngine diagnosticsEngine(
        llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()),
        &*diagOpts,
        new clang::DiagnosticConsumer());

    // create driver
    const char *const mainBinaryPath = argv[0];
    const std::unique_ptr<clang::driver::Driver> driver(
        newDriver(&diagnosticsEngine, mainBinaryPath));
    driver->setCheckInputsExist(false);

    // create compilation invocation
    const std::unique_ptr<clang::driver::Compilation> compilation(
        driver->BuildCompilation(llvm::makeArrayRef(argv)));
    const llvm::opt::ArgStringList *const cc1Args = getCC1Arguments(compilation.get());
    return newInvocation(&diagnosticsEngine, *cc1Args);
}

static clang::FileManager *newFileManager()
{
    clang::FileSystemOptions fileSystemOptions;
    return new clang::FileManager(fileSystemOptions);
}

static oclint::CompilerInstance *newCompilerInstance(clang::CompilerInvocation *compilerInvocation,
    clang::FileManager *fileManager, bool runClangChecker = false)
{
    auto compilerInstance = new oclint::CompilerInstance();
    compilerInstance->setInvocation(compilerInvocation);
    compilerInstance->setFileManager(fileManager);
    compilerInstance->createDiagnostics(new DiagnosticDispatcher(runClangChecker));
    if (!compilerInstance->hasDiagnostics())
    {
        throw oclint::GenericException("cannot create compiler diagnostics");
    }
    compilerInstance->createSourceManager(*fileManager);
    if (!compilerInstance->hasSourceManager())
    {
        throw oclint::GenericException("cannot create compiler source manager");
    }
    return compilerInstance;
}

#ifndef NDEBUG
static void printCompileCommandDebugInfo(
    std::pair<std::string, clang::tooling::CompileCommand> &compileCommand,
    std::vector<std::string> &commandLine)
{
    LOG_DEBUG_LINE("-----------------------");
    LOG_DEBUG("File: ");
    LOG_DEBUG_LINE(compileCommand.first.c_str());
    LOG_DEBUG("Directory: ");
    LOG_DEBUG_LINE(compileCommand.second.Directory.c_str());
    LOG_DEBUG("Command: ");
    for (auto& flag : compileCommand.second.CommandLine)
    {
        LOG_DEBUG(flag.c_str());
        LOG_DEBUG(" ");
    }
    LOG_DEBUG_LINE("");
    LOG_DEBUG("Adjusted Command: ");
    for (auto& cmd : commandLine)
    {
        LOG_DEBUG(cmd.c_str());
        LOG_DEBUG(" ");
    }
    LOG_DEBUG_LINE("");
}
#endif

static std::vector<std::string> adjustArguments(std::vector<std::string> &unadjustedCmdLine,
	const std::string& filename)
{
    clang::tooling::ArgumentsAdjuster argAdjuster =
        clang::tooling::combineAdjusters(
            clang::tooling::getClangStripOutputAdjuster(),
            clang::tooling::getClangSyntaxOnlyAdjuster());
    return argAdjuster(unadjustedCmdLine, filename);
}

static void constructCompilersAndFileManagers(std::vector<oclint::CompilerInstance *> &compilers,
    std::vector<clang::FileManager *> &fileManagers,
    CompileCommandPairs &compileCommands,
    std::string &mainExecutable)
{
    for (auto &compileCommand : compileCommands)
    {
        std::vector<std::string> adjustedCmdLine =
            adjustArguments(compileCommand.second.CommandLine, compileCommand.first);

#ifndef NDEBUG
        printCompileCommandDebugInfo(compileCommand, adjustedCmdLine);
#endif

        LOG_VERBOSE("Compiling ");
        LOG_VERBOSE(compileCommand.first.c_str());
        if (chdir(compileCommand.second.Directory.c_str()))
        {
            throw oclint::GenericException("Cannot change dictionary into \"" +
                compileCommand.second.Directory + "\", "
                "please make sure the directory exists and you have permission to access!");
        }
        clang::CompilerInvocation *compilerInvocation =
            newCompilerInvocation(mainExecutable, adjustedCmdLine);
        clang::FileManager *fileManager = newFileManager();
        oclint::CompilerInstance *compiler = newCompilerInstance(compilerInvocation, fileManager);

        compiler->start();
        if (!compiler->getDiagnostics().hasErrorOccurred() && compiler->hasASTContext())
        {
            LOG_VERBOSE(" - Success");
            compilers.push_back(compiler);
            fileManagers.push_back(fileManager);
        }
        else
        {
            LOG_VERBOSE(" - Failed");
        }
        LOG_VERBOSE_LINE("");
    }
}

static void invokeClangStaticAnalyzer(
    CompileCommandPairs &compileCommands,
    std::string &mainExecutable)
{
    for (auto &compileCommand : compileCommands)
    {
        LOG_VERBOSE("Clang Static Analyzer ");
        LOG_VERBOSE(compileCommand.first.c_str());
        if (chdir(compileCommand.second.Directory.c_str()))
        {
            throw oclint::GenericException("Cannot change dictionary into \"" +
                compileCommand.second.Directory + "\", "
                "please make sure the directory exists and you have permission to access!");
        }
        std::vector<std::string> adjustedArguments =
            adjustArguments(compileCommand.second.CommandLine, compileCommand.first);
        clang::CompilerInvocation *compilerInvocation =
            newCompilerInvocation(mainExecutable, adjustedArguments, true);
        clang::FileManager *fileManager = newFileManager();
        oclint::CompilerInstance *compiler = newCompilerInstance(compilerInvocation,
            fileManager, true);

        compiler->start();
        if (!compiler->getDiagnostics().hasErrorOccurred() && compiler->hasASTContext())
        {
            LOG_VERBOSE(" - Done");
        }
        else
        {
            LOG_VERBOSE(" - Finished with Failure");
        }
        compiler->end();
        compiler->resetAndLeakFileManager();
        fileManager->clearStatCaches();
        LOG_VERBOSE_LINE("");
    }
}

static void invoke(CompileCommandPairs &compileCommands,
    std::string &mainExecutable, oclint::Analyzer &analyzer)
{
    std::vector<oclint::CompilerInstance *> compilers;
    std::vector<clang::FileManager *> fileManagers;
    constructCompilersAndFileManagers(compilers, fileManagers, compileCommands, mainExecutable);

    // collect a collection of AST contexts
    std::vector<clang::ASTContext *> localContexts;
    for (auto compiler : compilers)
    {
        localContexts.push_back(&compiler->getASTContext());
    }

    // use the analyzer to do the actual analysis
    analyzer.preprocess(localContexts);
    analyzer.analyze(localContexts);
    analyzer.postprocess(localContexts);

    // send out the signals to release or simply leak resources
    for (size_t compilerIndex = 0; compilerIndex != compilers.size(); ++compilerIndex)
    {
        compilers.at(compilerIndex)->end();
        compilers.at(compilerIndex)->resetAndLeakFileManager();
        fileManagers.at(compilerIndex)->clearStatCaches();
        delete compilers.at(compilerIndex);
        delete fileManagers.at(compilerIndex);
    }
}

void Driver::run(const clang::tooling::CompilationDatabase &compilationDatabase,
    llvm::ArrayRef<std::string> sourcePaths, oclint::Analyzer &analyzer)
{
    CompileCommandPairs compileCommands;
    constructCompileCommands(compileCommands, compilationDatabase, sourcePaths);

    static int staticSymbol;
    std::string mainExecutable = llvm::sys::fs::getMainExecutable("oclint", &staticSymbol);

    if (option::enableGlobalAnalysis())
    {
        invoke(compileCommands, mainExecutable, analyzer);
    }
    else
    {
        for (auto &compileCommand : compileCommands)
        {
            CompileCommandPairs oneCompileCommand { compileCommand };
            invoke(oneCompileCommand, mainExecutable, analyzer);
        }
    }

    if (option::enableClangChecker())
    {
        invokeClangStaticAnalyzer(compileCommands, mainExecutable);
    }
}
