#ifndef OCLINT_RESULTS_H
#define OCLINT_RESULTS_H

#include <vector>

class Violation;
class ViolationSet;

using namespace std;

class Results
{
private:
    static Results *_singleton;
    Results();

    vector<ViolationSet*>* _collection;

public:
    static Results* getInstance();
    void add(ViolationSet *violationSet);

    vector<Violation> allViolations();

    int numberOfViolations();
    int numberOfViolationsWithPriority(int priority);
    int numberOfFiles();
    int numberOfFilesWithViolations();
};

#endif
