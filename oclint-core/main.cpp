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
    virtual void report(ASTContext &astContext, ViolationSet *violationSet)
    {
        SourceManager *sourceManager = &astContext.getSourceManager();
        for (int index = 0, numberOfViolations = violationSet->numberOfViolations();
            index < numberOfViolations; index++)
        {
            Violation violation = violationSet->getViolations().at(index);
            const ViolationNode *violationNode = violation.node;
            if (violationNode->type == DECL)
            {
                Decl *node = (Decl *)violationNode->node;
                SourceLocation startLocation = node->getLocStart();
                startLocation.print(outs(), *sourceManager);
            }
            if (violationNode->type == STMT)
            {
                Stmt *node = (Stmt *)violationNode->node;
                SourceLocation startLocation = node->getLocStart();
                startLocation.print(outs(), *sourceManager);
            }
            const RuleBase *rule = violation.rule;
            cout << ": oclint: " << rule->name() << " (" << rule->priority() << ") " << violation.description << endl;
        }
    }
};

class ActionFactory
{
public:
    ASTConsumer *newASTConsumer()
    {
      return new Processor(new PlainTextReporter());
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

int main(int argc, const char **argv) {
  ActionFactory Factory;
  CommonOptionsParser OptionsParser(argc, argv);

  if (consumeArgRulesPath(argv[0]) == 0 && RuleSet::numberOfRules() > 0) {
    consumeRuleConfigurations();

    for (int i = 0; i < OptionsParser.GetSourcePathList().size(); i++) {
      string sourcePath = OptionsParser.GetSourcePathList().at(i);
      cout << i << " : " << sourcePath << endl;
    }

    ClangTool Tool(OptionsParser.GetCompilations(),
                   OptionsParser.GetSourcePathList());

    return Tool.run(newFrontendActionFactory(&Factory));
  }

  return 0;
}
