#ifndef OCLINT_RESULTCOLLECTOR_H
#define OCLINT_RESULTCOLLECTOR_H

#include "oclint/Violation.h"

#include <vector>

namespace oclint
{

class ViolationSet;


class ResultCollector
{
private:
    static ResultCollector *_singleton;

protected:
    ResultCollector();
    ~ResultCollector();

public:
    static ResultCollector* getInstance();

private:
    std::vector<ViolationSet*> _collection;
    ViolationSet *_compilerErrorSet;
    ViolationSet *_compilerWarningSet;
    ViolationSet *_clangStaticCheckerBugSet;

public:
    void add(ViolationSet *violationSet);

    std::vector<ViolationSet*> getCollection() const;

    void addError(const Violation& violation);
    ViolationSet* getCompilerErrorSet() const;

    void addWarning(const Violation& violation);
    ViolationSet* getCompilerWarningSet() const;

    void addCheckerBug(const Violation& violation);
    ViolationSet* getClangStaticCheckerBugSet() const;
};

} // end namespace oclint

#endif
