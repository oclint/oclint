#include <dirent.h>
#include <dlfcn.h>
#include <iostream>

#include "oclint/GenericException.h"
#include "oclint/Options.h"

#include "reporters.h"

static oclint::Reporter* selectedReporter = nullptr;

void loadReporter()
{
    selectedReporter = nullptr;
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
            if (reporter->name() == oclint::option::reportType())
            {
                selectedReporter = reporter;
                break;
            }
        }
        closedir(pDir);
    }
    if (selectedReporter == nullptr)
    {
        throw oclint::GenericException(
            "cannot find dynamic library for report type: " + oclint::option::reportType());
    }
}

oclint::Reporter* reporter()
{
    return selectedReporter;
}
