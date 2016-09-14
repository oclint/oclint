#ifndef OCLINT_ANALYTICS_H
#define OCLINT_ANALYTICS_H

#include <string>

#include <clang/Basic/LangOptions.h>

namespace oclint
{

class Analytics
{
public:
  static void send(int exitCode);

public:
  static void languageOption(clang::LangOptions langOptions);
  static void ruleConfiguration(std::string key, std::string value);
};

} // end namespace oclint

#endif
