#ifndef OCLINT_RULEBASE_H
#define OCLINT_RULEBASE_H

#include <string>

#include "oclint/RuleCarrier.h"

namespace oclint
{

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
    virtual const std::string name() const = 0;
    virtual const std::string attributeName() const {
        return name();
    }
    virtual const std::string category() const = 0;
    virtual int priority() const = 0;
};

} // end namespace oclint

#endif
