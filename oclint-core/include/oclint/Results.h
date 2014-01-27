#ifndef OCLINT_RESULTS_H
#define OCLINT_RESULTS_H

#include <vector>

#include "oclint/ReportableResults.h"

namespace oclint
{

class Violation;
class ViolationSet;

class Results :public ReportableResults
{
protected:
    static Results *_singleton;
    Results();
    ~Results();

public:
    static Results* getInstance();

protected:
    std::vector<ViolationSet*> _collection;
    ViolationSet *_compilerErrorSet;
    ViolationSet *_compilerWarningSet;
    ViolationSet *_clangStaticCheckerBugSet;

public:
    void add(ViolationSet *violationSet);

    std::vector<Violation> allViolations() const override;

    int numberOfViolations() const override;
    int numberOfViolationsWithPriority(int priority) const override;
    int numberOfFiles() const override;
    int numberOfFilesWithViolations() const override;

    void addError(const Violation& violation);
    int numberOfErrors() const override;
    bool hasErrors() const override;
    const std::vector<Violation>& allErrors() const override;

    void addWarning(const Violation& violation);
    int numberOfWarnings() const override;
    bool hasWarnings() const override;
    const std::vector<Violation>& allWarnings() const override;

    void addCheckerBug(const Violation& violation);
    int numberOfCheckerBugs() const override;
    bool hasCheckerBugs() const override;
    const std::vector<Violation>& allCheckerBugs() const override;
};

} // end namespace oclint

#endif
