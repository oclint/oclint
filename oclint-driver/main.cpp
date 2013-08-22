#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include <clang/Tooling/CommonOptionsParser.h>

#include "oclint/Analyzer.h"
#include "oclint/CompilerInstance.h"
#include "oclint/Driver.h"
#include "oclint/GenericException.h"
#include "oclint/Options.h"
#include "oclint/Reporter.h"
#include "oclint/Results.h"
#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"
#include "oclint/RulesetFilter.h"
#include "oclint/RulesetBasedAnalyzer.h"
#include "oclint/ViolationSet.h"
#include "oclint/Violation.h"

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;

static oclint::Reporter *selectedReporter = NULL;

void dynamicLoadRules(string ruleDirPath)
{
    DIR *pDir = opendir(ruleDirPath.c_str());
    if (pDir != NULL)
    {
        struct dirent *dirp;
        while ((dirp = readdir(pDir)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            string rulePath = ruleDirPath + "/" + string(dirp->d_name);
            if (dlopen(rulePath.c_str(), RTLD_LAZY) == NULL)
            {
                cerr << dlerror() << endl;
                closedir(pDir);
                throw oclint::GenericException("cannot open dynamic library: " + rulePath);
            }
        }
        closedir(pDir);
    }
}

void consumeArgRulesPath()
{
    for (const auto& rulePath : oclint::option::rulesPath())
    {
        dynamicLoadRules(rulePath);
    }
}

void loadReporter()
{
    selectedReporter = NULL;
    string defaultReportersPath = oclint::option::reporterPath();
    DIR *pDir = opendir(defaultReportersPath.c_str());
    if (pDir != NULL)
    {
        struct dirent *dirp;
        while ((dirp = readdir(pDir)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            string reporterPath = defaultReportersPath + "/" + string(dirp->d_name);
            void *reporterHandle = dlopen(reporterPath.c_str(), RTLD_LAZY);
            if (reporterHandle == NULL)
            {
                cerr << dlerror() << endl;
                closedir(pDir);
                throw oclint::GenericException("cannot open dynamic library: " + reporterPath);
            }
            oclint::Reporter* (*createMethodPointer)();
            createMethodPointer = (oclint::Reporter* (*)())dlsym(reporterHandle, "create");
            oclint::Reporter* reporter = (oclint::Reporter*)createMethodPointer();
            if (reporter->name() == oclint::option::reportType())
            {
                selectedReporter = reporter;
                break;
            }
        }
        closedir(pDir);
    }
    if (selectedReporter == NULL)
    {
        throw oclint::GenericException(
            "cannot find dynamic library for report type: " + oclint::option::reportType());
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
    ofstream *out = new ofstream(output.c_str());
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

oclint::Reporter* reporter()
{
    return selectedReporter;
}

void printErrorLine(const char *errorMessage)
{
    cerr << endl << "oclint: error: " << errorMessage << endl;
}

enum ExitCode
{
    SUCCESS,
    RULE_NOT_FOUND,
    REPORTER_NOT_FOUND,
    ERROR_WHILE_PROCESSING,
    ERROR_WHILE_REPORTING,
    VIOLATIONS_EXCEED_THRESHOLD
};

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

int main(int argc, const char **argv)
{
    CommonOptionsParser optionsParser(argc, argv);
    oclint::option::process(argv[0]);

    int prepareStatus = prepare();
    if (prepareStatus)
    {
        return prepareStatus;
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
        return ERROR_WHILE_PROCESSING;
    }
    oclint::Results *results = oclint::Results::getInstance();

    try
    {
        ostream *out = outStream();
        reporter()->report(results, *out);
        disposeOutStream(out);
    }
    catch (const exception& e)
    {
        printErrorLine(e.what());
        return ERROR_WHILE_REPORTING;
    }

    if (numberOfViolationsExceedThreshold(results))
    {
        return VIOLATIONS_EXCEED_THRESHOLD;
    }
    return SUCCESS;
}
