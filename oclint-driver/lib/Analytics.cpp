#include "oclint/Analytics.h"

#ifndef COUNTLY_ANALYTICS

void oclint::Analytics::send(int) {}
void oclint::Analytics::ruleConfiguration(std::string key, std::string value) {}
void oclint::Analytics::languageOption(clang::LangOptions langOptions) {}

#else

#include <map>
#include <sstream>

#include <countly/Countly.h>
#include <countly/Device.h>
#include <countly/Metrics.h>

#include "oclint/ExitCode.h"
#include "oclint/Options.h"
#include "oclint/RuleSet.h"
#include "oclint/Version.h"

static std::map<std::string, std::string>* _ruleConfigurations = nullptr;
static std::map<std::string, int>* _languageCounter = nullptr;

static std::string hashedWorkingPath()
{
  std::string workingPath = oclint::option::workingPath();
  if (workingPath == "")
  {
    return "unknown-project-id";
  }

  std::ostringstream hashed;
  hashed << std::hash<std::string>{}(workingPath);
  return hashed.str();
}

static std::string toolchain()
{
  std::shared_ptr<FILE> pipe(popen("cc --version", "r"), pclose);
  if (pipe)
  {
    char buffer[128];
    if (fgets(buffer, 128, pipe.get()) != NULL)
    {
      std::string toolchain(buffer);
      return toolchain.erase(toolchain.find_last_not_of(" \n\r\t")+1);
    }
  }
  return "unknown-toolchain";
}

static std::string exitCodeToString(int exitCode)
{
  switch (exitCode)
  {
    case SUCCESS: return "success";
    case RULE_NOT_FOUND: return "rule_not_found";
    case REPORTER_NOT_FOUND: return "reporter_not_found";
    case ERROR_WHILE_PROCESSING: return "error_while_processing";
    case ERROR_WHILE_REPORTING: return "error_while_reporting";
    case VIOLATIONS_EXCEED_THRESHOLD: return "violations_exceed_threshold";
    default: return "unknown_exit_status";
  }
}

static void sendEnvironment(countly::Countly &cntly)
{
  std::map<std::string, std::string> segments;
  // anonymous identifiers to deduplicate events
  segments["device_id"] = countly::Device::id();
    // https://github.com/ryuichis/countly-cpp/blob/master/lib/Device.cpp
    // this is a hashed string of this network card's mac address
  segments["project_id"] = hashedWorkingPath();

  segments["version"] = oclint::Version::identifier();
  segments["bin_path"] = oclint::option::binPath();
  segments["os"] = countly::Metrics::os() + "/" + countly::Metrics::osVersion();
  segments["toolchain"] = toolchain();

  cntly.recordEvent("Environment", segments);
}

static void sendConfiguration(countly::Countly &cntly)
{
  std::map<std::string, std::string> segments;
  segments["report_type"] = oclint::option::reportType();
  segments["global_analysis"] = oclint::option::enableGlobalAnalysis() ? "1" : "0";
  segments["clang_checker"] = oclint::option::enableClangChecker() ? "1" : "0";
  segments["allow_duplications"] = oclint::option::allowDuplicatedViolations() ? "1" : "0";
  segments["p1_violations"] = std::to_string(oclint::option::maxP1());
  segments["p2_violations"] = std::to_string(oclint::option::maxP2());
  segments["p3_violations"] = std::to_string(oclint::option::maxP3());

  cntly.recordEvent("Configuration", segments);
}

static void sendLoadedRules(countly::Countly &cntly)
{
  std::map<std::string, std::string> segments;

  std::vector<std::string> rules = oclint::option::rulesetFilter().filteredRuleNames();
  for (int ruleIdx = 0, numRules = oclint::RuleSet::numberOfRules(); ruleIdx < numRules; ruleIdx++)
  {
    oclint::RuleBase *rule = oclint::RuleSet::getRuleAtIndex(ruleIdx);
    std::string ruleId = rule->identifier();
    bool enabled = std::find(rules.begin(), rules.end(), ruleId) != rules.end();
    segments[ruleId] = enabled ? "1" : "0";
  }

  cntly.recordEvent("LoadedRules", segments);
}

static void sendRuleConfiguration(countly::Countly &cntly)
{
  if (_ruleConfigurations && _ruleConfigurations->size() > 0)
  {
    cntly.recordEvent("RuleConfigurations", *_ruleConfigurations);
  }
}

static void sendLanguageCount(countly::Countly &cntly)
{
  if (_languageCounter)
  {
    std::map<std::string, std::string> segments;
    for (auto &lang : *_languageCounter)
    {
      if (lang.second > 0)
      {
        segments[lang.first] = std::to_string(lang.second);
      }
    }
    if (segments.size() > 0)
    {
      cntly.recordEvent("LanguageCount", segments);
    }
  }
}

static void sendExitStatus(countly::Countly &cntly, int exitCode)
{
  std::map<std::string, std::string> segments = {{"exit_code", exitCodeToString(exitCode)}};
  cntly.recordEvent("ExitStatus", segments);
}

void oclint::Analytics::send(int exitCode)
{
  countly::Countly cntly;
  cntly.start("countly.ryuichisaito.com", "873c792b2ead515f27f0ccba01a976ae9a4cc425");

  switch (exitCode) //!OCLINT
  {
    case SUCCESS:
    case ERROR_WHILE_REPORTING:
      sendLanguageCount(cntly);
    case ERROR_WHILE_PROCESSING:
    case REPORTER_NOT_FOUND:
      sendLoadedRules(cntly);
    default:
      sendRuleConfiguration(cntly);
      sendEnvironment(cntly);
      sendConfiguration(cntly);
      sendExitStatus(cntly, exitCode);
  }

  cntly.suspend();
}

void oclint::Analytics::ruleConfiguration(std::string key, std::string value)
{
  if (_ruleConfigurations == nullptr)
  {
      _ruleConfigurations = new std::map<std::string, std::string>();
  }

  _ruleConfigurations->operator[](key) = value;
}

void oclint::Analytics::languageOption(clang::LangOptions langOpts)
{
  if (_languageCounter == nullptr)
  {
      _languageCounter = new std::map<std::string, int>();
      _languageCounter->operator[]("c") = 0;
      _languageCounter->operator[]("cpp") = 0;
      _languageCounter->operator[]("objc") = 0;
  }
  if (langOpts.ObjC1 || langOpts.ObjC2)
  {
      int objc = _languageCounter->operator[]("objc");
      _languageCounter->operator[]("objc") = objc + 1;
  }
  if (langOpts.CPlusPlus)
  {
      int cpp = _languageCounter->operator[]("cpp");
      _languageCounter->operator[]("cpp") = cpp + 1;
  }
  if (langOpts.C99)
  {
      int c99 = _languageCounter->operator[]("c");
      _languageCounter->operator[]("c") = c99 + 1;
  }
}

#endif
