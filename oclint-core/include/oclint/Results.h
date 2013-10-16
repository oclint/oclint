#ifndef OCLINT_RESULTS_H
#define OCLINT_RESULTS_H

#include <vector>

namespace oclint
{

class Violation;
class ViolationSet;

class Results
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

    std::vector<Violation> allViolations() const;

    int numberOfViolations() const;
    int numberOfViolationsWithPriority(int priority) const;
    int numberOfFiles() const;
    int numberOfFilesWithViolations() const;

    void addError(const Violation& violation);
    int numberOfErrors() const;
    bool hasErrors() const;
    const std::vector<Violation>& allErrors() const;

    void addWarning(const Violation& violation);
    int numberOfWarnings() const;
    bool hasWarnings() const;
    const std::vector<Violation>& allWarnings() const;

    void addCheckerBug(const Violation& violation);
    int numberOfCheckerBugs() const;
    bool hasCheckerBugs() const;
    const std::vector<Violation>& allCheckerBugs() const;
};

} // end namespace oclint

#endif
