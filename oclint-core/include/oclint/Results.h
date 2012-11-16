#ifndef OCLINT_RESULTS_H
#define OCLINT_RESULTS_H

#include <vector>

class Violation;
class ViolationSet;

using namespace std;

class Results
{
protected:
    static Results *_singleton;
    Results();

public:
    static Results* getInstance();

protected:
    vector<ViolationSet*>* _collection;

public:
    void add(ViolationSet *violationSet);

    vector<Violation> allViolations();

    int numberOfViolations();
    int numberOfViolationsWithPriority(int priority);
    int numberOfFiles();
    int numberOfFilesWithViolations();
};

#endif
