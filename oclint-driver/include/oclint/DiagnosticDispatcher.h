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
    explicit DiagnosticDispatcher(bool runClangChecker);
    void HandleDiagnostic(clang::DiagnosticsEngine::Level diagnosticLevel,
                          const clang::Diagnostic& diagnosticInfo) override;
};

} // end namespace oclint

#endif
