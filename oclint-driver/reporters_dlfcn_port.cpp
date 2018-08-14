#include <dirent.h>
#include <dlfcn.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_set>

#include "oclint/GenericException.h"
#include "oclint/Options.h"
#include "llvm/ADT/StringRef.h"

#include "reporters.h"

static std::vector<oclint::Reporter*> selectedReporterVector;

void loadReporter()
{
    selectedReporterVector.clear();

    // Create a set that contains all the report types
    std::unordered_set<std::string> reportTypeSet;
    std::stringstream ss(oclint::option::reportType());
    
    while(ss.good())
    {
        std::string reportType;
        std::getline(ss, reportType, ',');
        reportTypeSet.insert(reportType);
    }

    // Load the specified reporters
    std::string reportDirPath = oclint::option::reporterPath();
    DIR *pDir = opendir(reportDirPath.c_str());
    if (pDir != nullptr)
    {
        struct dirent *dirp;
        while ((dirp = readdir(pDir)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            std::string reporterPath = reportDirPath + "/" + std::string(dirp->d_name);
            void *reporterHandle = dlopen(reporterPath.c_str(), RTLD_LAZY);
            if (reporterHandle == nullptr)
            {
                std::cerr << dlerror() << std::endl;
                closedir(pDir);
                throw oclint::GenericException("cannot open dynamic library: " + reporterPath);
            }
            oclint::Reporter* (*createMethodPointer)();
            createMethodPointer = (oclint::Reporter* (*)())dlsym(reporterHandle, "create");
            oclint::Reporter* reporter = (oclint::Reporter*)createMethodPointer();
            if(reportTypeSet.find(reporter->name()) != reportTypeSet.end())
            {
                selectedReporterVector.push_back(reporter);
            }
        }
        closedir(pDir);
    }
    if (selectedReporterVector.size() == 0)
    {
        throw oclint::GenericException(
            "cannot find dynamic library for report type: " + oclint::option::reportType());
    }
}

std::vector<oclint::Reporter*> reporter()
{
    return selectedReporterVector;
}
