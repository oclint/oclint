#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <memory>

#include <clang/Tooling/CommonOptionsParser.h>

#include "oclint/Analytics.h"
#include "oclint/Analyzer.h"
#include "oclint/CompilerInstance.h"
#include "oclint/Driver.h"
#include "oclint/ExitCode.h"
#include "oclint/GenericException.h"
#include "oclint/Options.h"
#include "oclint/RawResults.h"
#include "oclint/Reporter.h"
#include "oclint/ResultCollector.h"
#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"
#include "oclint/RulesetFilter.h"
#include "oclint/RulesetBasedAnalyzer.h"
#include "oclint/UniqueResults.h"
#include "oclint/Version.h"
#include "oclint/ViolationSet.h"
#include "oclint/Violation.h"

#include "reporters.h"
#include "rules.h"

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;

void consumeArgRulesPath()
{
    for (const auto& rulePath : oclint::option::rulesPath())
    {
        dynamicLoadRules(rulePath);
    }
}

bool numberOfViolationsExceedThreshold(oclint::Results *results)
{
    return results->numberOfViolationsWithPriority(1) > oclint::option::maxP1() ||
        results->numberOfViolationsWithPriority(2) > oclint::option::maxP2() ||
        results->numberOfViolationsWithPriority(3) > oclint::option::maxP3();
}

ostream* outStream()
{
    if (!oclint::option::hasOutputPath())
    {
        return &cout;
    }
    string output = oclint::option::outputPath();
    auto out = new ofstream(output.c_str());
    if (!out->is_open())
    {
        throw oclint::GenericException("cannot open report output file " + output);
    }
    return out;
}

void disposeOutStream(ostream* out)
{
    if (out && oclint::option::hasOutputPath())
    {
        ofstream *fout = (ofstream *)out;
        fout->close();
    }
}

void listRules()
{
    cerr << "Enabled rules:\n";
    for (const std::string &ruleName : oclint::option::rulesetFilter().filteredRuleNames())
    {
        cerr << "- " << ruleName << "\n";
    }
}

void printErrorLine(const char *errorMessage)
{
    cerr << endl << "oclint: error: " << errorMessage << endl;
}

void printViolationsExceedThresholdError(const oclint::Results *results)
{
    printErrorLine("violations exceed threshold");
    cerr << "P1=" << results->numberOfViolationsWithPriority(1)
        << "[" << oclint::option::maxP1() << "] ";
    cerr << "P2=" << results->numberOfViolationsWithPriority(2)
        << "[" << oclint::option::maxP2() << "] ";
    cerr << "P3=" << results->numberOfViolationsWithPriority(3)
        << "[" << oclint::option::maxP3() << "] " <<endl;
}

std::unique_ptr<oclint::Results> getResults()
{
    std::unique_ptr<oclint::Results> results;
    if (oclint::option::allowDuplicatedViolations())
    {
        results.reset(new oclint::RawResults(*oclint::ResultCollector::getInstance()));
    }
    else
    {
        results.reset(new oclint::UniqueResults(*oclint::ResultCollector::getInstance()));
    }
    return results;
}

int prepare()
{
    try
    {
        consumeArgRulesPath();
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return RULE_NOT_FOUND;
    }
    if (oclint::RuleSet::numberOfRules() <= 0)
    {
        printErrorLine("no rule loaded");
        return RULE_NOT_FOUND;
    }
    try
    {
        loadReporter();
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return REPORTER_NOT_FOUND;
    }

    return SUCCESS;
}

static int sendAnalyticsAndExit(int exitCode)
{
  if (!oclint::option::disableAnalytics())
  {
    oclint::Analytics::send(exitCode);
  }
  return exitCode;
}

static void oclintVersionPrinter()
{
    outs() << "OCLint (http://oclint.org/):\n";
    outs() << "  OCLint version " << oclint::Version::identifier() << ".\n";
    outs() << "  Built " << __DATE__ << " (" << __TIME__ << ").\n";
}

extern llvm::cl::OptionCategory OCLintOptionCategory;

int main(int argc, const char **argv)
{
    llvm::cl::AddExtraVersionPrinter(&oclintVersionPrinter);
    CommonOptionsParser optionsParser(argc, argv, OCLintOptionCategory);
    oclint::option::process(argv[0]);

    int prepareStatus = prepare();
    if (prepareStatus)
    {
        return sendAnalyticsAndExit(prepareStatus);
    }

    if (oclint::option::showEnabledRules())
    {
        listRules();
    }

    oclint::RulesetBasedAnalyzer analyzer(oclint::option::rulesetFilter().filteredRules());
    oclint::Driver driver;
    try
    {
        driver.run(optionsParser.getCompilations(), optionsParser.getSourcePathList(), analyzer);
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return sendAnalyticsAndExit(ERROR_WHILE_PROCESSING);
    }

    std::unique_ptr<oclint::Results> results(std::move(getResults()));

    try
    {
        ostream *out = outStream();
        reporter()->report(results.get(), *out);
        disposeOutStream(out);
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return sendAnalyticsAndExit(ERROR_WHILE_REPORTING);
    }

    if (numberOfViolationsExceedThreshold(results.get()))
    {
        printViolationsExceedThresholdError(results.get());
        return sendAnalyticsAndExit(VIOLATIONS_EXCEED_THRESHOLD);
    }

    return sendAnalyticsAndExit(SUCCESS);
}
