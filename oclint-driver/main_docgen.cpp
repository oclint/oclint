#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <map>
#include <memory>

#include <clang/Tooling/CommonOptionsParser.h>

#include "oclint/Options.h"
#include "oclint/RuleBase.h"
#include "oclint/RuleSet.h"
#include "oclint/Version.h"

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

void writeIndexHeader(ofstream& out)
{
    out << "Rule Index" << endl
        << "==========" << endl
        << endl
        << "OCLint |release| includes " << allRules().size() << " rules." << endl
        << endl
        << ".. toctree::" << endl
        << "   :maxdepth: 2" << endl
        << endl;
}

void writeCategoryToIndex(ofstream& out, string category)
{
    out << "   " << category << endl;
}

void writeCategoryHeader(ofstream& out, string category)
{
    category[0] = toupper(category[0]);
    out << category << endl;
    for (int i = 0; i < category.size(); i++)
    {
        out << "=";
    }
    out << endl << endl;
}

void writeRuleToCategory(ofstream& out, oclint::RuleBase* rule)
{
    auto ruleIdentifier = rule->identifier();
    auto ruleCategory = rule->category();
    auto fileName = rule->fileName();

    out << ruleIdentifier << endl;
    for (int i = 0; i < ruleIdentifier.size(); i++)
    {
        out << "-";
    }
    out << endl << endl;

    out << "**Since: " << rule->since() << "**" << endl << endl;

    out << "**Name: " << rule->name() << "**" << endl << endl;

    out << rule->description() << endl << endl;

    out << "This rule is defined by the following class: "
        << "`oclint-rules/rules/" << ruleCategory << "/" << fileName << " "
        << "<https://github.com/oclint/oclint/blob/master/"
        << "oclint-rules/rules/" << ruleCategory << "/" << fileName << ">`_"
        << endl << endl;

    out << "**Example:**" << endl << endl;
    out << rule->example() << endl << endl;

    auto thresholdMapping = rule->thresholds();
    if (thresholdMapping.size() > 0)
    {
        out << "**Thresholds:**" << endl << endl;
        for (auto entry : thresholdMapping)
        {
            out << entry.first << endl
                << "    " << entry.second << endl;
        }
        out << endl;
    }

    if (rule->enableSuppress())
    {
        out << "**Suppress:**" << endl << endl;
        out << ".. code-block:: cpp" << endl << endl;
        out << "    __attribute__((annotate(\"oclint:suppress[" << rule->attributeName() << "]\")))"
            << endl << endl;
    }

    auto additionalDoc = rule->additionalDocument();
    if (additionalDoc.size() > 0)
    {
        out << additionalDoc << endl;
    }
}

void writeFooter(ofstream& out)
{
    time_t timeT = time(NULL);
    struct tm * currentTime = localtime(&timeT);
    out << endl;
    out << ".. Generated on " << asctime(currentTime);
    out << endl;
}

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

    vector<string> categories;
    map<string, vector<oclint::RuleBase *>> rulesMapping;

    for (auto rule : allRules())
    {
        auto category = rule->category();
        if (std::find(categories.begin(), categories.end(), category) == categories.end())
        {
            vector<oclint::RuleBase *> rulesForCategory;
            rulesForCategory.push_back(rule);
            rulesMapping[category] = rulesForCategory;
            categories.push_back(category);
        }
        else
        {
            auto rulesForCategory = rulesMapping[category];
            rulesForCategory.push_back(rule);
            rulesMapping[category] = rulesForCategory;
        }
    }
    sort(categories.begin(), categories.end());

    string docRulePath = "../build/oclint-docs/rules/";
    string indexPath = docRulePath + "index.rst";
    ofstream *indexOut = new ofstream(indexPath.c_str());

    writeIndexHeader(*indexOut);

    for (auto category : categories)
    {
        writeCategoryToIndex(*indexOut, category);

        string categoryPath = docRulePath + category + ".rst";
        ofstream *categoryOut = new ofstream(categoryPath.c_str());

        writeCategoryHeader(*categoryOut, category);

        auto rulesForCategory = rulesMapping[category];
        for (auto rule : rulesForCategory)
        {
            writeRuleToCategory(*categoryOut, rule);
        }

        writeFooter(*categoryOut);
        categoryOut->close();
    }

    writeFooter(*indexOut);
    indexOut->close();

    return SUCCESS;
}
