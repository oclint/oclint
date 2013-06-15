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

#include <unistd.h>

#include <sstream>

#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Option/ArgList.h>
#include <llvm/Support/Debug.h>
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

#include "oclint/CarrierDiagnosticConsumer.h"
#include "oclint/CompilerInstance.h"
#include "oclint/Driver.h"
#include "oclint/GenericException.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

static clang::driver::Driver *newDriver(clang::DiagnosticsEngine *diagnostics,
    const char *binaryName)
{
    const std::string defaultOutputName = "a.out";
    clang::driver::Driver *driver = new clang::driver::Driver(binaryName,
        llvm::sys::getDefaultTargetTriple(), defaultOutputName, *diagnostics);
    driver->setTitle("OCLint");
    return driver;
}


static std::string compilationJobsToString(const clang::driver::Job &job)
{
    std::stringstream buffer;
    if (const clang::driver::Command *cmd = clang::dyn_cast<clang::driver::Command>(&job))
    {
        buffer << cmd->getExecutable();
        for (llvm::opt::ArgStringList::const_iterator argIdx = cmd->getArguments().begin(),
            argEnd = cmd->getArguments().end(); argIdx != argEnd; ++argIdx)
        {
            buffer << " ";
            for (const char *s = *argIdx; *s; ++s)
            {
                if (*s == '"' || *s == '\\' || *s == '$')
                {
                    buffer << '\\';
                }
                buffer << *s;
            }
        }
        buffer << "\n";
    }
    else
    {
        const clang::driver::JobList *jobs = clang::cast<clang::driver::JobList>(&job);
        for (clang::driver::JobList::const_iterator jobIdx = jobs->begin(), jobEnd = jobs->end();
            jobIdx != jobEnd; ++jobIdx)
        {
            buffer << compilationJobsToString(**jobIdx);
        }
    }
    return buffer.str();
}

static const llvm::opt::ArgStringList *getCC1Arguments(clang::driver::Compilation *compilation)
{
    const clang::driver::JobList &jobList = compilation->getJobs();
    if (jobList.size() != 1 || !clang::isa<clang::driver::Command>(*jobList.begin()))
    {
        throw oclint::GenericException("one compiler command contains multiple jobs:\n" +
            compilationJobsToString(compilation->getJobs()) + "\n");
    }

    const clang::driver::Command *cmd = clang::cast<clang::driver::Command>(*jobList.begin());
    if (llvm::StringRef(cmd->getCreator().getName()) != "clang")
    {
        throw oclint::GenericException("expected a clang compiler command");
    }

    return &cmd->getArguments();
}

static clang::CompilerInvocation *newInvocation(
    clang::DiagnosticsEngine *diagnostics,
    const llvm::opt::ArgStringList &argStringList)
{
    assert(!argStringList.empty() && "Must at least contain the program name!");
    clang::CompilerInvocation *invocation = new clang::CompilerInvocation;
    clang::CompilerInvocation::CreateFromArgs(*invocation,
        argStringList.data() + 1, argStringList.data() + argStringList.size(), *diagnostics);
    invocation->getFrontendOpts().DisableFree = false;
    return invocation;
}

static void constructCompileCommands(
    std::vector<std::pair<std::string, clang::tooling::CompileCommand> > &compileCommands,
    const clang::tooling::CompilationDatabase &compilationDatabase,
    llvm::ArrayRef<std::string> sourcePaths)
{
    for (unsigned pathIndex = 0, pathEnd = sourcePaths.size(); pathIndex != pathEnd; pathIndex++)
    {
        llvm::SmallString<1024> filePath(clang::tooling::getAbsolutePath(sourcePaths[pathIndex]));

        std::vector<clang::tooling::CompileCommand> compileCmdsForFile =
            compilationDatabase.getCompileCommands(filePath.str());
        if (!compileCmdsForFile.empty())
        {
            for (int commandsIndex = 0, commandsEnd = compileCmdsForFile.size();
                commandsIndex != commandsEnd; commandsIndex++)
            {
                compileCommands.push_back(
                    std::make_pair(filePath.str(), compileCmdsForFile[commandsIndex]));
            }
        }
        /* - this needs to be collected and the gathered information will be printed eventually
           - not here
        else
        {
            DEBUG({
                llvm::dbgs() << "Skipping " << filePath << ". Command line not found.\n";
            });
        }
        */
    }
}

void __attribute__((annotate("oclint:suppress")))
    /* this method breaks every law as it could, high refactoring is necessary */
    Driver::run(const clang::tooling::CompilationDatabase &compilationDatabase,
        llvm::ArrayRef<std::string> sourcePaths, oclint::Analyzer &analyzer)
{
    std::vector<std::pair<std::string, clang::tooling::CompileCommand> > compileCommands;
    constructCompileCommands(compileCommands, compilationDatabase, sourcePaths);

    std::vector<oclint::CompilerInstance *> compilers;
    std::vector<clang::FileManager *> fileManagers;

    static int staticSymbol;
    std::string mainExecutable = llvm::sys::Path::GetMainExecutable("oclint", &staticSymbol).str();

    // compiling process
    DEBUG({
        llvm::dbgs() << "\nStart compiling:\n";
    });
    for (unsigned compileCmdIdx = 0, numCmds = compileCommands.size();
        compileCmdIdx < numCmds; compileCmdIdx++)
    {
        // Prepare for command lines, and convert to old-school argv
        if (chdir(compileCommands[compileCmdIdx].second.Directory.c_str()))
        {
            throw oclint::GenericException("Cannot change dictionary into \"" +
                compileCommands[compileCmdIdx].second.Directory + "\", "
                "please make sure the directory exists and you have permission to access!");
        }
        llvm::OwningPtr<clang::tooling::ArgumentsAdjuster> argumentsAdjusterPtr(
            new clang::tooling::ClangSyntaxOnlyAdjuster());
        std::vector<std::string> commandLine = argumentsAdjusterPtr->Adjust(
            compileCommands[compileCmdIdx].second.CommandLine);
        assert(!commandLine.empty());
        commandLine[0] = mainExecutable;

        std::vector<const char*> argv;
        for (int commandLineIndex = 0, commandLineEnd = commandLine.size();
            commandLineIndex != commandLineEnd; commandLineIndex++)
        {
            argv.push_back(commandLine[commandLineIndex].c_str());
        }

        // create diagnostic engine
        llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagOpts =
            new clang::DiagnosticOptions();
        clang::DiagnosticsEngine diagnosticsEngine(
            llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs>(new clang::DiagnosticIDs()),
            &*diagOpts,
            new clang::DiagnosticConsumer(),
            false);

        // create driver
        const char *const mainBinaryPath = argv[0];
        const llvm::OwningPtr<clang::driver::Driver> driver(
            newDriver(&diagnosticsEngine, mainBinaryPath));
        driver->setCheckInputsExist(false);

        // create compilation invocation
        const llvm::OwningPtr<clang::driver::Compilation> compilation(
            driver->BuildCompilation(llvm::makeArrayRef(argv)));
        const llvm::opt::ArgStringList *const cc1Args = getCC1Arguments(compilation.get());
        clang::CompilerInvocation *compilerInvocation = newInvocation(&diagnosticsEngine, *cc1Args);

        // create file manager
        clang::FileSystemOptions fileSystemOptions;
        clang::FileManager *fileManager = new clang::FileManager(fileSystemOptions);

        // finally, create a compiler instance to handle the actual compilation work
        oclint::CompilerInstance *compiler = new oclint::CompilerInstance();
        compiler->setInvocation(compilerInvocation);
        compiler->setFileManager(fileManager);
        compiler->createDiagnostics(new CarrierDiagnosticConsumer());
        if (!compiler->hasDiagnostics())
        {
            throw oclint::GenericException("cannot create compiler diagnostics");
        }
        compiler->createSourceManager(*fileManager);
        if (!compiler->hasSourceManager())
        {
            throw oclint::GenericException("cannot create compiler source manager");
        }

        // start compilation to get the abstract syntax tree (AST) for all source code
        compiler->start();
        if (!compiler->getDiagnostics().hasErrorOccurred() && compiler->hasASTContext())
        {
            DEBUG({
                llvm::dbgs() << ".";
            });
            compilers.push_back(compiler);
            fileManagers.push_back(fileManager);
        }
        else
        {
            DEBUG({
                llvm::dbgs() << "X";
            });
        }
    }
    DEBUG({
        llvm::dbgs() << "\n";
    });

    // collect a collection of AST contexts
    std::vector<clang::ASTContext *> localContexts;
    for (int compilerIndex = 0; compilerIndex < compilers.size(); compilerIndex++)
    {
        localContexts.push_back(&compilers.at(compilerIndex)->getASTContext());
    }

    // use the analyzer to do the actual analysis
    analyzer.preprocess(localContexts);
    analyzer.analyze(localContexts);
    analyzer.postprocess(localContexts);

    // send out the signals to release or simply leak resources
    for (int compilerIndex = 0; compilerIndex < compilers.size(); compilerIndex++)
    {
        compilers.at(compilerIndex)->end();
        compilers.at(compilerIndex)->resetAndLeakFileManager();
        fileManagers.at(compilerIndex)->clearStatCaches();
    }
}
