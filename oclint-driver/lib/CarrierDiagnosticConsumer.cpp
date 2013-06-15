#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringRef.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

#include "oclint/CarrierDiagnosticConsumer.h"
#include "oclint/Results.h"
#include "oclint/Violation.h"

using namespace oclint;

void CarrierDiagnosticConsumer::HandleDiagnostic(clang::DiagnosticsEngine::Level diagnosticLevel,
    const clang::Diagnostic &diagnosticInfo)
{
    clang::DiagnosticConsumer::HandleDiagnostic(diagnosticLevel, diagnosticInfo);

    clang::SourceLocation location = diagnosticInfo.getLocation();
    clang::SourceManager *sourceManager = &diagnosticInfo.getSourceManager();
    llvm::StringRef filename = sourceManager->getFilename(location);
    int line = sourceManager->getPresumedLineNumber(location);
    int column = sourceManager->getPresumedColumnNumber(location);

    clang::SmallString<100> diagnosticMessage;
    diagnosticInfo.FormatDiagnostic(diagnosticMessage);

    Violation violation(0, filename.str(), line, column, 0, 0, diagnosticMessage.str().str());

    Results *results = Results::getInstance();
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
