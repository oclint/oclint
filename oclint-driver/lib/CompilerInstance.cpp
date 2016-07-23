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
#include <clang/StaticAnalyzer/Frontend/FrontendActions.h>

#include "oclint/Options.h"

using namespace oclint;

static clang::FrontendAction *getFrontendAction()
{
    if (option::enableClangChecker())
    {
        return new clang::ento::AnalysisAction();
    }
    return new clang::SyntaxOnlyAction();
}

void CompilerInstance::start()
{
    assert(hasDiagnostics() && "Diagnostics engine is not initialized!");
    assert(!getFrontendOpts().ShowHelp && "Client must handle '-help'!");
    assert(!getFrontendOpts().ShowVersion && "Client must handle '-version'!");

    setTarget(clang::TargetInfo::CreateTargetInfo(getDiagnostics(), getInvocation().TargetOpts));
    if (!hasTarget())
    {
        return;// false;
    }

    getTarget().adjust(getLangOpts());

    for (const auto& input : getFrontendOpts().Inputs)
    {
        if (hasSourceManager())
        {
            getSourceManager().clearIDTables();
        }

        clang::FrontendAction *frontendAction = getFrontendAction();
        if(frontendAction->BeginSourceFile(*this, input))
        {
            frontendAction->Execute();
            _actions.emplace_back(frontendAction);
        }
    }
}

void CompilerInstance::end()
{
    for (const auto& action : _actions)
    {
        action->EndSourceFile();
    }

    getDiagnostics().getClient()->finish();
}
