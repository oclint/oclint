#ifndef OCLINT_COMMANDLINEOPTIONS_H
#define OCLINT_COMMANDLINEOPTIONS_H

#include <string>

#include <llvm/Support/CommandLine.h>
#include <clang/Driver/OptTable.h>
#include <clang/Driver/Options.h>
#include <clang/Tooling/CommonOptionsParser.h>

using namespace std;
using namespace llvm;
using namespace clang::driver;
using namespace clang::tooling;

/* ----------------
   input and output
   ---------------- */

cl::opt<string> argOutput("o", cl::desc("Write output to <path>"), cl::value_desc("path"), cl::init("-"));

enum ReportType
{
  text, html
};
cl::opt<ReportType> argReportType(cl::desc("Choose report type:"), cl::values(clEnumVal(text , "Plain text report"), clEnumVal(html, "HTML formatted report"), clEnumValEnd));

/* --------------------
   oclint configuration
   -------------------- */

cl::list<string> argRulesPath("R", cl::Prefix, cl::desc("Add directory to rule loading path"), cl::value_desc("directory"), cl::ZeroOrMore);
cl::list<string> argRuleConfiguration("rc", cl::desc("Override the default baheviour of rules"), cl::value_desc("paramemter>=<value"), cl::ZeroOrMore);
cl::opt<int> argMaxP1("max-priority-1", cl::desc("The max allowed number of priority 1 violations"), cl::value_desc("threshold"), cl::init(0));
cl::opt<int> argMaxP2("max-priority-2", cl::desc("The max allowed number of priority 2 violations"), cl::value_desc("threshold"), cl::init(10));
cl::opt<int> argMaxP3("max-priority-3", cl::desc("The max allowed number of priority 3 violations"), cl::value_desc("threshold"), cl::init(20));

/* -------------
   libTooling cl
   ------------- */

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp(
    "For more information, please visit http://oclint.org\n"
);
static OwningPtr<OptTable> Options(createDriverOptTable());

#endif
