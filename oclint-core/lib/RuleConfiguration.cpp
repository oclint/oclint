#include <cstdlib>

#include "oclint/RuleConfiguration.h"

using namespace oclint;

std::map<std::string, std::string>* RuleConfiguration::_configurations = NULL;

void RuleConfiguration::addConfiguration(std::string key, std::string value)
{
    if (_configurations == NULL)
    {
        _configurations = new std::map<std::string, std::string>();
    }

    _configurations->operator[](key) = value;
}

bool RuleConfiguration::hasKey(std::string key)
{
    return _configurations != NULL && _configurations->find(key) != _configurations->end();
}

std::string RuleConfiguration::valueForKey(std::string key)
{
    return _configurations->operator[](key);
}

void RuleConfiguration::removeAll()
{
    if (_configurations != NULL)
    {
        _configurations = NULL;
    }
}

std::string RuleConfiguration::stringForKey(std::string key, std::string defaultValue)
{
    return hasKey(key) ? valueForKey(key) : defaultValue;
}

int RuleConfiguration::intForKey(std::string key, int defaultValue)
{
    return hasKey(key) ? atoi(valueForKey(key).c_str()) : defaultValue;
}

double RuleConfiguration::doubleForKey(std::string key, double defaultValue)
{
    return hasKey(key) ? atof(valueForKey(key).c_str()) : defaultValue;
}

string RuleConfiguration::stringForKey(string key, string defaultValue)
{
    return hasKey(key) ? valueForKey(key) : defaultValue;
}

int RuleConfiguration::intForKey(string key, int defaultValue)
{
    return hasKey(key) ? atoi(valueForKey(key).c_str()) : defaultValue;
}

double RuleConfiguration::doubleForKey(string key, double defaultValue)
{
    return hasKey(key) ? atof(valueForKey(key).c_str()) : defaultValue;
}
