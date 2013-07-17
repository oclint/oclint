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
    std::vector<ViolationSet*>* _collection;
    ViolationSet *_compilerErrorSet;
    ViolationSet *_compilerWarningSet;
    ViolationSet *_clangStaticCheckerBugSet;

public:
    void add(ViolationSet *violationSet);

    std::vector<Violation> allViolations();

    int numberOfViolations();
    int numberOfViolationsWithPriority(int priority);
    int numberOfFiles();
    int numberOfFilesWithViolations();

    void addError(Violation& violation);
    int numberOfErrors();
    bool hasErrors();
    std::vector<Violation> allErrors();

    void addWarning(Violation& violation);
    int numberOfWarnings();
    bool hasWarnings();
    std::vector<Violation> allWarnings();

    void addCheckerBug(Violation& violation);
    int numberOfCheckerBugs();
    bool hasCheckerBugs();
    std::vector<Violation> allCheckerBugs();
};

} // end namespace oclint

#endif
