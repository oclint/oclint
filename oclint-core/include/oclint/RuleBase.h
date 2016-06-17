#ifndef OCLINT_RULEBASE_H
#define OCLINT_RULEBASE_H

#include <string>
#include <algorithm>

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
    virtual const std::string attributeName() const
    {
        return name();
    }
    virtual const std::string identifier() const
    {
        std::string copy = name();
        if (copy.empty())
        {
            return copy;
        }
        copy[0] = toupper(copy[0]);

        for (std::string::iterator it = copy.begin() + 1; it != copy.end(); ++it)
        {
            if (!isalpha(*(it - 1)) && islower(*it))
            {
                *it = toupper(*it);
            }
        }
        copy.erase(std::remove_if(copy.begin(), copy.end(),
                   [](char c){return !isalpha(c);}), copy.end());
        return copy;
    }
    virtual const std::string category() const = 0;
    virtual int priority() const = 0;
};

} // end namespace oclint

#endif
