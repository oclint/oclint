/*-
 * This code is derived from
 * http://llvm.org/svn/llvm-project/cfe/trunk/lib/Frontend/CompilerInstance.cpp
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
#include "oclint/CompilerInstance.h"

#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/FrontendActions.h>
#include "clang/Frontend/PCHContainerOperations.h"
#include <clang/StaticAnalyzer/Frontend/FrontendActions.h>

#include "oclint/Options.h"
#include "oclint/ExitCode.h"
#include "oclint/RuleSet.h"

using namespace oclint;

#include <iostream>
#include <unordered_set>
#include <regex>

#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/SourceLocation.h"

class MacroLocationRecorder : public clang::PPCallbacks {
  public:
    explicit MacroLocationRecorder(const std::string &sourcePath, const clang::SourceManager &SM, const std::set<std::string> &macrosToFilterStr, const std::vector<std::string> &macrosToFilterRegex, std::unordered_set<unsigned int> &macroFilterPresumedLOC)
        : _sourcePath(sourcePath),
          _SourceMgr(SM),
          _macrosToFilterStr(macrosToFilterStr),
          _macrosToFilterRegex(macrosToFilterRegex),
          _macroFilterPresumedLOC(macroFilterPresumedLOC)
    {}

    void MacroExpands(const clang::Token &MacroNameTok, const clang::MacroDefinition &MD, clang::SourceRange Range, const clang::MacroArgs *Args) override {
        std::string macroName = MacroNameTok.getIdentifierInfo()->getName().str();
        clang::SourceLocation Loc = MacroNameTok.getLocation();

        if (Loc.isValid()) {
            clang::PresumedLoc PLoc = _SourceMgr.getPresumedLoc(Loc);
            // Process macros in source file only exculding headers. When running in distcc pump
            // mode the location of source file is changed with the "#line"
            // directive(https://gcc.gnu.org/onlinedocs/cpp/Line-Control.html) by the pump
            // script. To handle this do substring match instead of string comparision.
            if (PLoc.isValid() && endsWith(_sourcePath, PLoc.getFilename())) {
                // Perform string match first, this avoid costly regex if it succeeds
                if (_macrosToFilterStr.find(macroName) != _macrosToFilterStr.end()) {
                    _macroFilterPresumedLOC.insert(PLoc.getLine());
                } else { // check if regex match
                    for (const auto& regexStr : _macrosToFilterRegex) {
                        try {
                            std::regex pattern(regexStr);
                            if (std::regex_search(macroName, pattern)) {
                                _macroFilterPresumedLOC.insert(PLoc.getLine());
                            }
                        } catch (const std::regex_error& e) {
                            std::cerr << "Invalid macro suppression regex : " << regexStr << " (" << e.what() << ")" << std::endl;
                        }
                    }
                }
            }
        }
    }

  private:
    bool endsWith(std::string const & value, std::string const & ending)
    {
        if (ending.size() > value.size()) {
            return false;
        }

        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }

  private:
    const std::string &_sourcePath;
    const clang::SourceManager &_SourceMgr;
    const std::set<std::string> &_macrosToFilterStr;
    const std::vector<std::string> &_macrosToFilterRegex;
    std::unordered_set<unsigned int> &_macroFilterPresumedLOC;
};

static clang::FrontendAction *getFrontendAction() {
    if (option::enableClangChecker()) {
        return new clang::ento::AnalysisAction();
    }
    return new clang::SyntaxOnlyAction();
}

CompilerInstance::CompilerInstance(clang::CompilerInvocation *compilerInvocation)
    : clang::CompilerInstance(
      std::shared_ptr<clang::CompilerInvocation>(compilerInvocation),
      std::make_shared<clang::PCHContainerOperations>(),
      nullptr) {
}

void CompilerInstance::setupTarget() {
    // OpenMPIsDevice has been removed in newer LLVM versions
    // CUDA/OpenMP offloading is handled differently now
    if (getLangOpts().CUDA &&
        !getFrontendOpts().AuxTriple.empty())
    {
        clang::TargetOptions targetOptions;
        targetOptions.Triple = llvm::Triple::normalize(getFrontendOpts().AuxTriple);
        targetOptions.HostTriple = getTarget().getTriple().str();
        setAuxTarget(clang::TargetInfo::CreateTargetInfo(getDiagnostics(), targetOptions));
    }

    getTarget().adjust(getDiagnostics(), getLangOpts(), /*Target=*/nullptr);

    if (auto *auxTarget = getAuxTarget()) {
        getTarget().setAuxTarget(auxTarget);
    }
}

void CompilerInstance::start(std::string sourcePath) {
    assert(hasDiagnostics() && "Diagnostics engine is not initialized!");
    assert(!getFrontendOpts().ShowHelp && "Client must handle '-help'!");
    assert(!getFrontendOpts().ShowVersion && "Client must handle '-version'!");

    setTarget(clang::TargetInfo::CreateTargetInfo(getDiagnostics(), getInvocation().getTargetOpts()));
    if (!hasTarget()) { return; }
    setupTarget();

    for (const auto& input : getFrontendOpts().Inputs) {
        if (hasSourceManager()) {
            getSourceManager().clearIDTables();
        }

        clang::FrontendAction *frontendAction = getFrontendAction();
        if(frontendAction->BeginSourceFile(*this, input)) {
            // Retrive the macros to skip during cyclomatic complexity from command line arguments
            // and send it to the preprocessor callback for gathering the macro source file
            // location. This is processed during visiting the AST in
            // "CyclomaticComplexityMetric::isFilteredMacro()" method
            std::vector<std::string> suppresionMacro = option::getSuppressionMacroStr();
            std::set<std::string> macrosToFilterStr(suppresionMacro.begin(), suppresionMacro.end());
            std::vector<std::string> macrosToFilterRegex = option::getSuppressionMacroRegex();

            // Preprocessor is set during the exectution of "frontendAction->BeginSourceFile()"
            assert(hasPreprocessor() && "Does not have preprocessor");
            clang::Preprocessor &PP = getPreprocessor();

            std::unordered_set<unsigned int>& macroFilterPresumedLOC = RuleSet::getMacroFilterPresumedLOC();
            macroFilterPresumedLOC.clear(); // reset the container before capturing

            PP.addPPCallbacks(std::make_unique<MacroLocationRecorder>(sourcePath, getSourceManager(), macrosToFilterStr, macrosToFilterRegex, macroFilterPresumedLOC));

            if (llvm::Error err = frontendAction->Execute()) {
                llvm::errs() << "Error: " << llvm::toString(std::move(err)) << "\n";
                std::exit(ERROR_WHILE_PROCESSING);
            }

            _actions.emplace_back(frontendAction);
        }
    }
}

void CompilerInstance::end() {
    for (const auto& action : _actions) {
        action->EndSourceFile();
    }

    getDiagnostics().getClient()->finish();
}
