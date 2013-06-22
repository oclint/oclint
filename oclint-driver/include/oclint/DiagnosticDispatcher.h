#ifndef OCLINT_DIAGNOSTICDISPATCHER_H
#define OCLINT_DIAGNOSTICDISPATCHER_H

#include <clang/Basic/Diagnostic.h>

namespace oclint
{

class DiagnosticDispatcher : public clang::DiagnosticConsumer
{
private:
    bool _isCheckerCustomer;

public:
    DiagnosticDispatcher(bool runClangChecker);
    void HandleDiagnostic(clang::DiagnosticsEngine::Level diagnosticLevel,
        const clang::Diagnostic &diagnosticInfo);
};

} // end namespace oclint

#endif
