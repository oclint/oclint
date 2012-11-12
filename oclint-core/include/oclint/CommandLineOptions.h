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

/* -------------
   libTooling cl
   ------------- */

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp(
    "For more information, please visit http://oclint.org\n"
);
static OwningPtr<OptTable> Options(createDriverOptTable());

#endif
