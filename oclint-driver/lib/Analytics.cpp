#include "oclint/Analytics.h"

#ifndef COUNTLY_ANALYTICS

void oclint::Analytics::send() {}

#else

#include <map>
#include <string>
#include <sstream>

#include <countly/Countly.h>
#include <countly/Device.h>
#include <countly/Metrics.h>

#include "oclint/Options.h"
#include "oclint/Version.h"

std::string hashedWorkingPath()
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

std::string ruleNames()
{
  std::ostringstream names;
  for (const std::string &ruleName : oclint::option::rulesetFilter().filteredRuleNames())
  {
    names << ruleName << ",";
  }
  return names.str().substr(0, names.str().size()-1);
}

void oclint::Analytics::send()
{
  std::map<std::string, std::string> segmentation;
  segmentation["oclint_version"] = oclint::Version::identifier();
  segmentation["oclint_bin_path"] = oclint::option::binPath();
  segmentation["host_os"] = countly::Metrics::os();
  segmentation["host_os_version"] = countly::Metrics::osVersion();

  // anonymous identifiers to deduplicate events
  segmentation["device_id"] = countly::Device::id();
    // https://github.com/ryuichis/countly-cpp/blob/master/lib/Device.cpp
    // this is a hashed string of this network card's mac address
  segmentation["project_id"] = hashedWorkingPath();

  segmentation["report_type"] = oclint::option::reportType();
  segmentation["enable_global_analysis"] =
    oclint::option::enableGlobalAnalysis() ? "1" : "0";
  segmentation["enable_clang_checker"] =
    oclint::option::enableClangChecker() ? "1" : "0";
  segmentation["allow_duplicated_violations"] =
    oclint::option::allowDuplicatedViolations() ? "1" : "0";
  segmentation["max_allowed_violations"] =
    std::to_string(oclint::option::maxP1()) + "," +
    std::to_string(oclint::option::maxP2()) + "," +
    std::to_string(oclint::option::maxP3());
  // segmentation["enabled_rules"] = ruleNames();

  countly::Countly cntly;
  cntly.start("countly.ryuichisaito.com",
    "873c792b2ead515f27f0ccba01a976ae9a4cc425");
  cntly.recordEvent("DevEvent", segmentation);
  cntly.suspend();
}

#endif
