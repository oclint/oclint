#ifndef OCLINT_RULECONFIGURATION_H
#define OCLINT_RULECONFIGURATION_H

#include <vector>
#include <utility>
#include <string>

using namespace std;

class RuleConfiguration
{
private:
    static vector<pair<string, string> >* _configurations;
    static int indexOfKey(string key);
    static int numberOfConfigurations();

public:
    static void addConfiguration(string key, string value);
    static bool hasKey(string key);
    static string valueForKey(string key);
    static void removeAll();

    static string stringForKey(string key, string defaultValue = "");
    static int intForKey(string key, int defaultValue = 0);
    static double doubleForKey(string key, double defaultValue = 0.0);
};

#endif
