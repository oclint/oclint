#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
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
using namespace clang;

class PlainTextReporter : public Reporter
{
public:
    virtual void report(Results *results)
    {
        cout << "OCLint Report" << endl << endl;
        cout << "Summary: TotalFiles=" << results->numberOfFiles() << " ";
        cout << "FilesWithViolations=" << results->numberOfFilesWithViolations() << " ";
        cout << "P1=" << results->numberOfViolationsWithPriority(1) << " ";
        cout << "P2=" << results->numberOfViolationsWithPriority(2) << " ";
        cout << "P3=" << results->numberOfViolationsWithPriority(3) << " ";
        cout << endl << endl;
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            Violation violation = violationSet.at(index);
            cout << violation.path << ":" << violation.startLine << ":" << violation.startColumn;
            const RuleBase *rule = violation.rule;
            cout << ": " << rule->name() << " P" << rule->priority() << " " << violation.message << endl;
        }

        cout << endl << "[OCLint (http://oclint.org) v0.6]" << endl;
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

string getExecutablePath(const char *argv) {
  llvm::SmallString<128> installedPath(argv);
  if (llvm::sys::path::filename(installedPath) == installedPath) {
    llvm::sys::Path intermediatePath = llvm::sys::Program::FindProgramByName(
      llvm::sys::path::filename(installedPath.str()));
    if (!intermediatePath.empty()) {
      installedPath = intermediatePath.str();
    }
  }
  llvm::sys::fs::make_absolute(installedPath);
  installedPath = llvm::sys::path::parent_path(installedPath);
  return string(installedPath.c_str());
}

int dynamicLoadRules(string ruleDirPath) {
  DIR *dp = opendir(ruleDirPath.c_str());
  if (dp != NULL) {
    struct dirent *dirp;
    while ((dirp = readdir(dp))) {
      if (dirp->d_name[0] == '.') {
        continue;
      }
      string rulePath = ruleDirPath + "/" + string(dirp->d_name);
      if (dlopen(rulePath.c_str(), RTLD_NOW) == NULL){
        cerr << dlerror() << endl;
        closedir(dp);
        return 3;
      }
    }
    closedir(dp);
  }
  return 0;
}

int consumeArgRulesPath(const char* executablePath) {
  if (argRulesPath.size() == 0) {
    string exeStrPath = getExecutablePath(executablePath);
    string defaultRulePath = exeStrPath + "/../lib/oclint/rules";
    return dynamicLoadRules(defaultRulePath);
  }
  int returnFlag = 0;
  for (unsigned i = 0; i < argRulesPath.size() && returnFlag == 0; ++i) {
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
        string value = configuration.substr(indexOfSeparator + 1, configuration.size() - indexOfSeparator - 1);
        RuleConfiguration::addConfiguration(key, value);
    }
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
    CommonOptionsParser OptionsParser(argc, argv);
    if (consumeArgRulesPath(argv[0]) == 0 && RuleSet::numberOfRules() > 0)
    {
        consumeRuleConfigurations();

        ClangTool clangTool(OptionsParser.GetCompilations(), OptionsParser.GetSourcePathList());

        ProcessorActionFactory actionFactory;
        if (clangTool.run(newFrontendActionFactory(&actionFactory)) == 0)
        {
            Reporter *reporter = new PlainTextReporter();
            reporter->report(Results::getInstance());
            return SUCCESS;
        }
        return ERROR_WHILE_PROCESSING;
    }
    return RULE_NOT_FOUND;
}
