#ifndef OCLINT_RULEBASE_H
#define OCLINT_RULEBASE_H

#include <string>
#include <thread>

#include "oclint/RuleCarrier.h"

namespace oclint
{

class RuleBase
{
private:
    std::mutex _mutex;

protected:
    RuleCarrier *_carrier;

    virtual void apply() = 0;

public:
    void takeoff(RuleCarrier *carrier)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _carrier = carrier;
        apply();
    }

    virtual ~RuleBase() {}
    virtual const std::string name() const = 0;
    virtual int priority() const = 0;
};

} // end namespace oclint

#endif
