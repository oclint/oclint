#include "oclint/RuleBase.h"

using namespace oclint;

void RuleBase::takeoff(RuleCarrier *carrier)
{
    _carrier = carrier;
    apply();
}

const std::string RuleBase::attributeName() const
{
    return name();
}

const std::string RuleBase::identifier() const
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
               [](char eachChar){return !isalpha(eachChar);}), copy.end());
    return copy;
}

#ifdef DOCGEN
const std::string RuleBase::fileName() const
{
    return identifier() + "Rule.cpp";
}

bool RuleBase::enableSuppress() const
{
    return false;
}

const std::map<std::string, std::string> RuleBase::thresholds() const
{
    std::map<std::string, std::string> emptyMap;
    return emptyMap;
}

const std::string RuleBase::additionalDocument() const
{
    return "";
}
#endif

