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

public:
    void add(ViolationSet *violationSet);

    std::vector<Violation> allViolations();

    int numberOfViolations();
    int numberOfViolationsWithPriority(int priority);
    int numberOfFiles();
    int numberOfFilesWithViolations();
};

} // end namespace oclint

#endif
