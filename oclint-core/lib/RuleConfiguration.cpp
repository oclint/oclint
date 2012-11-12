#include "oclint/RuleConfiguration.h"

vector<pair<string, string> >* RuleConfiguration::_configurations = NULL;

void RuleConfiguration::addConfiguration(string key, string value)
{
    if (_configurations == NULL)
    {
        _configurations = new vector<pair<string, string> >();
    }

    _configurations->push_back(make_pair(key, value));
}

int RuleConfiguration::numberOfConfigurations()
{
    return (_configurations == NULL) ? 0 : _configurations->size();
}

int RuleConfiguration::indexOfKey(string key)
{
    for (int index = 0; index < numberOfConfigurations(); index++)
    {
        if (_configurations->at(index).first == key)
        {
            return index;
        }
    }
    return -1;
}

bool RuleConfiguration::hasKey(string key)
{
    return indexOfKey(key) != -1;
}

string RuleConfiguration::valueForKey(string key)
{
    return _configurations->at(indexOfKey(key)).second;
}

void RuleConfiguration::removeAll()
{
    if (_configurations != NULL)
    {
        _configurations = NULL;
    }
}
