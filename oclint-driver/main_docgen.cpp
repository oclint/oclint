#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <memory>

#include <clang/Tooling/CommonOptionsParser.h>

#include "oclint/Analyzer.h"
#include "oclint/CompilerInstance.h"
#include "oclint/Driver.h"
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

vector<oclint::RuleBase *> allRules()
{
    std::vector<oclint::RuleBase *> allRules;

    for (int ruleIdx = 0, numRules = oclint::RuleSet::numberOfRules();
        ruleIdx < numRules; ruleIdx++)
    {
        oclint::RuleBase *rule = oclint::RuleSet::getRuleAtIndex(ruleIdx);
        allRules.push_back(rule);
    }

    return allRules;
}

void printErrorLine(const char *errorMessage)
{
    cerr << endl << "oclint: error: " << errorMessage << endl;
}

enum ExitCode
{
    SUCCESS,
    RULE_NOT_FOUND
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

    return SUCCESS;
}

static void oclintDocGenVersionPrinter()
{
    cout << "OCLint DocGen (http://oclint.org/docs/):\n";
    cout << "  OCLint DocGen version " << oclint::Version::identifier() << ".\n";
    cout << "  Built " << __DATE__ << " (" << __TIME__ << ").\n";
}

extern llvm::cl::OptionCategory OCLintOptionCategory;

int main(int argc, const char **argv)
{
    llvm::cl::AddExtraVersionPrinter(&oclintDocGenVersionPrinter);
    CommonOptionsParser optionsParser(argc, argv, OCLintOptionCategory);
    oclint::option::process(argv[0]);

    int prepareStatus = prepare();
    if (prepareStatus)
    {
        return prepareStatus;
    }

    for (auto rule : allRules())
    {
        cout << rule->name() << ": " << rule->since() << endl;
    }

    return SUCCESS;
}
