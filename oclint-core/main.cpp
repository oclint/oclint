#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include <llvm/Support/raw_ostream.h>
#include <clang/AST/AST.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/RecordLayout.h>
#include <clang/Tooling/Tooling.h>
#include <llvm/Support/Path.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/Program.h>
#include <llvm/Support/FileSystem.h>

#include "oclint/CommandLineOptions.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include "oclint/Results.h"
#include "oclint/ViolationSet.h"
#include "oclint/Violation.h"
#include "oclint/RuleBase.h"
#include "oclint/Reporter.h"
#include "oclint/Processor.h"

using namespace std;
using namespace llvm;
using namespace llvm::sys;
using namespace clang;

class HTMLReporter : public Reporter
{
public:
    virtual void report(Results *results, ostream &out)
    {
        out << "<!DOCTYPE html>";
        out << "<html>";
        out << "<head>";
        out << "<title>OCLint Report</title>";
        out << "<style type='text/css'>"
            << "                            \
.priority1, .priority2, .priority3 {        \
    font-weight: bold;                      \
    text-align: center;                     \
    color: #B40527;                         \
}                                           \
.priority1 { background-color: #FFC200; }   \
.priority2 { background-color: #FFD3A6; }   \
.priority3 { background-color: #FFEEB5; }   \
table {                                     \
    border: 2px solid gray;                 \
    border-collapse: collapse;              \
    -moz-box-shadow: 3px 3px 4px #AAA;      \
    -webkit-box-shadow: 3px 3px 4px #AAA;   \
    box-shadow: 3px 3px 4px #AAA;           \
}                                           \
td, th {                                    \
    border: 1px solid #D3D3D3;              \
    padding: 4px 20px 4px 20px;             \
}                                           \
th {                                        \
    text-shadow: 2px 2px 2px white;         \
    border-bottom: 1px solid gray;          \
    background-color: #E9F4FF;              \
}"
            << "</style>";
        out << "</head>";
        out << "<body>";
        out << "<h1>OCLint Report</h1>";
        out << "<hr />";
        out << "<h2>Summary</h2>";
        out << "<table><thead><tr><th>Total Files</th><th>Files with Violations</th>"
            << "<th>Priority 1</th><th>Priority 2</th><th>Priority 3</th></tr></thead>";
        out << "<tbody><tr><td>" << results->numberOfFiles() << "</td><td>"
            << results->numberOfFilesWithViolations() << "</td><td class='priority1'>"
            << results->numberOfViolationsWithPriority(1) << "</td><td class='priority2'>"
            << results->numberOfViolationsWithPriority(2) << "</td><td class='priority3'>"
            << results->numberOfViolationsWithPriority(3) << "</td></tr></tbody></table>";
        out << "<hr />";
        out << "<table><thead><tr><th>File</th><th>Location</th><th>Rule Name</th>"
            << "<th>Priority</th><th>Message</th></tr></thead><tbody>";
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            Violation violation = violationSet.at(index);
            out << "<tr><td>" << violation.path << "</td><td>" << violation.startLine
                << ":" << violation.startColumn << "</td>";
            const RuleBase *rule = violation.rule;
            out << "<td>" << rule->name() << "</td><td class='priority" << rule->priority() << "'>"
                << rule->priority() << "</td><td>" << violation.message << "</td></tr>";
        }
        out << "</tbody></table>";
        out << "<hr />";
        time_t now = time(0);
        out << ctime(&now)
            << "| Generated with <a href='http://oclint.org'>OCLint v0.6</a>.</p>";
        out << "</body>";
        out << "</html>" << endl;
    }
};

class PlainTextReporter : public Reporter
{
public:
    virtual void report(Results *results, ostream &out)
    {
        out << "OCLint Report" << endl << endl;
        out << "Summary: TotalFiles=" << results->numberOfFiles() << " ";
        out << "FilesWithViolations=" << results->numberOfFilesWithViolations() << " ";
        out << "P1=" << results->numberOfViolationsWithPriority(1) << " ";
        out << "P2=" << results->numberOfViolationsWithPriority(2) << " ";
        out << "P3=" << results->numberOfViolationsWithPriority(3) << " ";
        out << endl << endl;
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            Violation violation = violationSet.at(index);
            out << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
            const RuleBase *rule = violation.rule;
            out << ": " << rule->name()
                << " P" << rule->priority() << " " << violation.message << endl;
        }
        out << endl << "[OCLint (http://oclint.org) v0.6]" << endl;
    }
};

class ProcessorActionFactory
{
public:
    ASTConsumer *newASTConsumer()
    {
      return new Processor();
    }
};

static string absoluteWorkingPath("");

void preserveWorkingPath()
{
    char path[300];
    if (getcwd(path, 300))
    {
        absoluteWorkingPath = string(path);
    }
}

string getExecutablePath(const char *argv)
{
    llvm::SmallString<128> installedPath(argv);
    if (path::filename(installedPath) == installedPath)
    {
        Path intermediatePath = Program::FindProgramByName(path::filename(installedPath.str()));
        if (!intermediatePath.empty())
        {
            installedPath = intermediatePath.str();
        }
    }
    fs::make_absolute(installedPath);
    installedPath = path::parent_path(installedPath);
    return string(installedPath.c_str());
}

int dynamicLoadRules(string ruleDirPath)
{
    DIR *dp = opendir(ruleDirPath.c_str());
    if (dp != NULL)
    {
        struct dirent *dirp;
        while ((dirp = readdir(dp)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            string rulePath = ruleDirPath + "/" + string(dirp->d_name);
            if (dlopen(rulePath.c_str(), RTLD_NOW) == NULL)
            {
                cerr << dlerror() << endl;
                closedir(dp);
                return 3;
            }
        }
        closedir(dp);
    }
    return 0;
}

int consumeArgRulesPath(const char* executablePath)
{
    if (argRulesPath.size() == 0)
    {
        string exeStrPath = getExecutablePath(executablePath);
        string defaultRulePath = exeStrPath + "/../lib/oclint/rules";
        return dynamicLoadRules(defaultRulePath);
    }
    int returnFlag = 0;
    for (unsigned i = 0; i < argRulesPath.size() && returnFlag == 0; ++i)
    {
        returnFlag = dynamicLoadRules(argRulesPath[i]);
    }
    return returnFlag;
}

void consumeRuleConfigurations()
{
    for (unsigned i = 0; i < argRuleConfiguration.size(); ++i)
    {
        string configuration = argRuleConfiguration[i];
        int indexOfSeparator = configuration.find_last_of("=");
        string key = configuration.substr(0, indexOfSeparator);
        string value = configuration.substr(indexOfSeparator + 1,
            configuration.size() - indexOfSeparator - 1);
        RuleConfiguration::addConfiguration(key, value);
    }
}

bool numberOfViolationsExceedThreshold(Results *results)
{
    return results->numberOfViolationsWithPriority(1) > argMaxP1 ||
        results->numberOfViolationsWithPriority(2) > argMaxP2 ||
        results->numberOfViolationsWithPriority(3) > argMaxP3;
}

ostream* outStream()
{
    if (argOutput == "-")
    {
        return &cout;
    }
    string absoluteOutputPath = argOutput.at(0) == '/' ?
        argOutput : absoluteWorkingPath + "/" + argOutput;
    ofstream *out = new ofstream(absoluteOutputPath.c_str());
    if (!out->is_open())
    {
        throw string("Cannot open file " + argOutput);
    }
    return out;
}

void disposeOutStream(ostream* out)
{
    if (out && argOutput != "-")
    {
        ofstream *fout = (ofstream *)out;
        fout->close();
    }
}

Reporter* reporter()
{
    if (argReportType == html)
    {
        return new HTMLReporter();
    }
    return new PlainTextReporter();
}

enum ExitCode
{
    SUCCESS,
    RULE_NOT_FOUND,
    ERROR_WHILE_PROCESSING,
    VIOLATIONS_EXCEED_THRESHOLD
};

int main(int argc, const char **argv)
{
    CommonOptionsParser optionsParser(argc, argv);
    if (consumeArgRulesPath(argv[0]) == 0 && RuleSet::numberOfRules() > 0)
    {
        consumeRuleConfigurations();
        preserveWorkingPath();

        ClangTool clangTool(optionsParser.getCompilations(), optionsParser.getSourcePathList());
        ProcessorActionFactory actionFactory;
        if (clangTool.run(newFrontendActionFactory(&actionFactory)) == 0)
        {
            ostream *out = outStream();
            Results *results = Results::getInstance();
            reporter()->report(results, *out);
            disposeOutStream(out);
            if (numberOfViolationsExceedThreshold(results))
            {
                return VIOLATIONS_EXCEED_THRESHOLD;
            }
            return SUCCESS;
        }
        return ERROR_WHILE_PROCESSING;
    }
    return RULE_NOT_FOUND;
}
