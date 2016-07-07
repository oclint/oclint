#ifndef OCLINT_RULEBASE_H
#define OCLINT_RULEBASE_H

#include <string>
#include <algorithm>

#ifdef DOCGEN
#include <map>
#endif

#include "oclint/RuleCarrier.h"

namespace oclint
{

class RuleBase
{
protected:
    RuleCarrier *_carrier;

public:
    void takeoff(RuleCarrier *carrier);

    virtual ~RuleBase() {}
    virtual void apply() = 0;
    virtual const std::string name() const = 0;
    virtual const std::string attributeName() const;
    virtual const std::string identifier() const;
    virtual const std::string category() const = 0;
    virtual int priority() const = 0;

#ifdef DOCGEN
    virtual const std::string since() const = 0;
    virtual const std::string description() const = 0;
    virtual const std::string example() const = 0;
    virtual const std::string fileName() const;
    virtual bool enableSuppress() const;
    virtual const std::map<std::string, std::string> thresholds() const;
    virtual const std::string additionalDocument() const;
#endif
};

} // end namespace oclint

#endif
