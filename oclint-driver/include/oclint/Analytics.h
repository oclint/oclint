#ifndef OCLINT_ANALYTICS_H
#define OCLINT_ANALYTICS_H

#include <string>

namespace oclint
{

class Analytics
{
public:
  static void send();

public:
  static void ruleConfiguration(std::string key, std::string value);
};

} // end namespace oclint

#endif
