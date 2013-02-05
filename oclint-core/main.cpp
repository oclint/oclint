#include <dlfcn.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#include <libxml/parser.h>
#include <libxml/xpath.h>

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

class ProcessorActionFactory
{
public:
    ASTConsumer *newASTConsumer()
    {
      return new Processor();
    }
};

static string absoluteWorkingPath("");
static Reporter *selectedReporter = NULL;

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

void consumeRulesConfigurationFile()
{
    cout << argRulesConfigurationFile << endl;
    string xmlFile = argRulesConfigurationFile;
    if (!xmlFile.empty())
    {
        xmlDocPtr doc = xmlParseFile(xmlFile.c_str());
        if (doc == NULL)
        {
        }
        xmlNodePtr root = xmlDocGetRootElement(doc);

        
        xmlXPathInit();
        xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
        if (ctxt == NULL) {
            cerr << "Error on XPath context" << endl;
            exit(-1);
        }
        // Evaluation de l'expression XPath
        xmlXPathObjectPtr xpathRes = xmlXPathEvalExpression(BAD_CAST "/rules/rule", ctxt);
        if (xpathRes == NULL) {
            cerr << "Error on XPath expression" << endl;
            exit(-1);
        }

        if (xpathRes->type == XPATH_NODESET)
        {
            for (int i = 0; i < xpathRes->nodesetval->nodeNr; i++)
                {
                xmlNodePtr n = xpathRes->nodesetval->nodeTab[i];
                cout << "Node rule found with name: " << n->name << endl;
                
                xmlNodePtr child = n->children;
                string key;
                string value;
                for (; child != NULL; child = child->next)
                {
                    if (child->type == XML_ELEMENT_NODE)
                    {
                        static const string RULE_NODE_KEY = "name";
                        static const string RULE_NODE_VALUE = "value";

                        cout << "Node rule value: " << child->name << " / " << child->children->content << endl;
                        string nodeName = reinterpret_cast<const char*>(child->name);
                        if (nodeName == RULE_NODE_KEY)
                        {
                            key = reinterpret_cast<const char*>(child->children->content);
                        }
                        else if (nodeName == RULE_NODE_VALUE)
                        {
                            value = reinterpret_cast<const char*>(child->children->content);
                        }
                    }
                }
                
                if ( !key.empty() && !value.empty() )
                {
                    cout << "Adding rule conf via xml file: " << key << " / " << value << endl;
                    RuleConfiguration::addConfiguration(key, value);
                }
            }
        }

        xmlXPathFreeObject(xpathRes);
        xmlXPathFreeContext(ctxt);
        xmlFreeDoc(doc);    
    }
}

int loadReporter(const char* executablePath)
{
    selectedReporter = NULL;
    string exeStrPath = getExecutablePath(executablePath);
    string defaultReportersPath = exeStrPath + "/../lib/oclint/reporters";
    DIR *dp = opendir(defaultReportersPath.c_str());
    if (dp != NULL)
    {
        struct dirent *dirp;
        while ((dirp = readdir(dp)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            string rulePath = defaultReportersPath + "/" + string(dirp->d_name);
            void *reporterHandle = dlopen(rulePath.c_str(), RTLD_NOW);
            if (reporterHandle == NULL)
            {
                cerr << dlerror() << endl;
                closedir(dp);
            }
            Reporter* (*createMethodPointer)();
            createMethodPointer = (Reporter* (*)())dlsym(reporterHandle, "create");
            Reporter* reporter = (Reporter*)createMethodPointer();
            if (reporter->name() == argReportType)
            {
                selectedReporter = reporter;
                break;
            }
        }
        closedir(dp);
    } // TODO: Remove the duplication pf loading rules and reporters
    return selectedReporter == NULL ? 1 : 0;
}

void readRuleFromXPath(xmlNodePtr node)
{
    string key;
    string value;
    
    for (xmlNodePtr child; child != NULL; child = node->next)
    {
        if (child->type == XML_ELEMENT_NODE)
        {
            static const string RULE_NODE_KEY = "name";
            static const string RULE_NODE_VALUE = "value";

            cout << "Node rule value: " << child->name;
            cout << " / " << child->children->content << endl;
            string nodeName = reinterpret_cast<const char*>(child->name);
            if (nodeName == RULE_NODE_KEY)
            {
                key = reinterpret_cast<const char*>(child->children->content);
            }
            else if (nodeName == RULE_NODE_VALUE)
            {
                value = reinterpret_cast<const char*>(child->children->content);
            }
        }
    }
    
    if ( !key.empty() && !value.empty() )
    {
        cout << "Adding rule conf via xml file: " << key << " / " << value << endl;
        RuleConfiguration::addConfiguration(key, value);
    }
}

/**
 * Read and consume XML configuration file
 */
void consumeRulesConfigurationFile()
{
    string xmlFile = argRulesConfigurationFile;
    if (!xmlFile.empty())
    {
        xmlDocPtr doc = xmlParseFile(xmlFile.c_str());
        
        xmlXPathInit();
        xmlXPathContextPtr ctxt = xmlXPathNewContext(doc);
        if (ctxt == NULL)
        {
            cerr << "Error on XPath context" << endl;
            exit(-1);
        }
        
        // Evaluate XPath expression
        static string const *XML_RULES_PATH = new string("rules/rule");
        //xmlXPathObjectPtr xpathRes = xmlXPathEvalExpression(BAD_CAST "rules/rule", ctxt);
        xmlXPathObjectPtr xpathRes = xmlXPathEvalExpression(BAD_CAST XML_RULES_PATH, ctxt);
        if (xpathRes == NULL)
        {
            cerr << "Error on XPath expression" << endl;
            exit(-1);
        }

        if (xpathRes->type == XPATH_NODESET)
        {
            for (int i = 0; i < xpathRes->nodesetval->nodeNr; i++)
            {
                xmlNodePtr n = xpathRes->nodesetval->nodeTab[i];
                cout << "Node rule found with name: " << n->name << endl;
                
                xmlNodePtr child = n->children;
                readRuleFromXPath(child);
            }
        }

        xmlXPathFreeObject(xpathRes);
        xmlXPathFreeContext(ctxt);
        xmlFreeDoc(doc);    
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
    return selectedReporter;
}

enum ExitCode
{
    SUCCESS,
    RULE_NOT_FOUND,
    REPORTER_NOT_FOUND,
    ERROR_WHILE_PROCESSING,
    VIOLATIONS_EXCEED_THRESHOLD
};

int main(int argc, const char **argv)
{
    CommonOptionsParser optionsParser(argc, argv);
    if (consumeArgRulesPath(argv[0]) || RuleSet::numberOfRules() <= 0)
    {
        return RULE_NOT_FOUND;
    }
    if (loadReporter(argv[0]))
    {
        return REPORTER_NOT_FOUND;
    }
    consumeRuleConfigurations();
    consumeRulesConfigurationFile();
    preserveWorkingPath();

    ClangTool clangTool(optionsParser.getCompilations(), optionsParser.getSourcePathList());
    ProcessorActionFactory actionFactory;
    if (clangTool.run(newFrontendActionFactory(&actionFactory)))
    {
        return ERROR_WHILE_PROCESSING;
    }
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
