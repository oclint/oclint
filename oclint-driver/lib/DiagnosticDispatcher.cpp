#include "oclint/DiagnosticDispatcher.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

#include "oclint/ResultCollector.h"
#include "oclint/Violation.h"

using namespace oclint;

DiagnosticDispatcher::DiagnosticDispatcher(bool runClangChecker)
{
    _isCheckerCustomer = runClangChecker;
}

void DiagnosticDispatcher::HandleDiagnostic(clang::DiagnosticsEngine::Level diagnosticLevel,
    const clang::Diagnostic &diagnosticInfo)
{
    int line = 0;
    int column = 0;
    std::string filename = "";

    clang::DiagnosticConsumer::HandleDiagnostic(diagnosticLevel, diagnosticInfo);

    clang::SourceLocation location = diagnosticInfo.getLocation();

    clang::SmallString<100> diagnosticMessage;
    diagnosticInfo.FormatDiagnostic(diagnosticMessage);

    if (diagnosticInfo.hasSourceManager()) {
        clang::SourceManager *sourceManager = &diagnosticInfo.getSourceManager();
        llvm::StringRef filename = sourceManager->getFilename(location);
        line = sourceManager->getPresumedLineNumber(location);
        column = sourceManager->getPresumedColumnNumber(location);
        filename = filename.str();
    }

    Violation violation(nullptr, filename, line, column, 0, 0,
                        diagnosticMessage.str().str());


    ResultCollector *results = ResultCollector::getInstance();
    if (_isCheckerCustomer)
    {
        results->addCheckerBug(violation);
    }
    else
    {
        if (diagnosticLevel == clang::DiagnosticsEngine::Warning)
        {
            results->addWarning(violation);
        }
        if (diagnosticLevel == clang::DiagnosticsEngine::Error ||
            diagnosticLevel == clang::DiagnosticsEngine::Fatal)
        {
            results->addError(violation);
        }
    }
}
