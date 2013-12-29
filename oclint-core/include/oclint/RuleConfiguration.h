#ifndef OCLINT_RULECONFIGURATION_H
#define OCLINT_RULECONFIGURATION_H

#include <string>

namespace oclint
{

class RuleConfiguration
{
public:
    static void addConfiguration(std::string key, std::string value);
    static bool hasKey(std::string key);
    static std::string valueForKey(std::string key);
    static void removeAll();

    static std::string stringForKey(std::string key, std::string defaultValue = "");
    static int intForKey(std::string key, int defaultValue = 0);
    static double doubleForKey(std::string key, double defaultValue = 0.0);
};

} // end namespace oclint

#endif
