#ifndef OCLINT_RULECONFIGURATION_H
#define OCLINT_RULECONFIGURATION_H

#include <string>
#include <map>

using namespace std;

class RuleConfiguration
{
private:
    static map<string, string>* _configurations;

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
