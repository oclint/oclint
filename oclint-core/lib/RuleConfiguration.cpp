#include <cstdlib>

#include "oclint/RuleConfiguration.h"

map<string, string>* RuleConfiguration::_configurations = NULL;

void RuleConfiguration::addConfiguration(string key, string value)
{
    if (_configurations == NULL)
    {
        _configurations = new map<string, string>();
    }

    _configurations->operator[](key) = value;
}

bool RuleConfiguration::hasKey(string key)
{
    return _configurations != NULL && _configurations->find(key) != _configurations->end();
}

string RuleConfiguration::valueForKey(string key)
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
