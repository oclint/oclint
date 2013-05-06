#ifndef OCLINT_RULEBASE_H
#define OCLINT_RULEBASE_H

#include <string>

#include "oclint/RuleCarrier.h"

using namespace std;

class RuleBase
{
protected:
    RuleCarrier *_carrier;

public:
    void takeoff(RuleCarrier *carrier)
    {
        _carrier = carrier;
        apply();
    }

    virtual ~RuleBase() {}
    virtual void apply() = 0;
    virtual const string name() const = 0;
    virtual int priority() const = 0;
};

#endif
