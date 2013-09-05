#include <dirent.h>
#include <windows.h>
#include <iostream>

#include "oclint/GenericException.h"
#include "oclint/Options.h"

#include "reporters.h"

static oclint::Reporter *selectedReporter = NULL;

void loadReporter()
{
    selectedReporter = NULL;
    std::string reportDirPath = oclint::option::reporterPath();
    DIR *pDir = opendir(reportDirPath.c_str());
    if (pDir != NULL)
    {
        struct dirent *dirp;
        while ((dirp = readdir(pDir)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            std::string reporterPath = reportDirPath + "/" + std::string(dirp->d_name);
            HMODULE reporterHandle = LoadLibrary(reporterPath.c_str());
            if (reporterHandle == NULL)
            {
                std::cerr << GetLastError() << std::endl;
                closedir(pDir);
                throw oclint::GenericException("cannot open dynamic library: " + reporterPath);
            }
            typedef oclint::Reporter* (*CreateReporterFunc)();
            CreateReporterFunc createMethodPointer;
            createMethodPointer = (CreateReporterFunc) GetProcAddress(reporterHandle, "create");
            oclint::Reporter* reporter = (oclint::Reporter*)createMethodPointer();
            if (reporter->name() == oclint::option::reportType())
            {
                selectedReporter = reporter;
                break;
            }
        }
        closedir(pDir);
    }
    if (selectedReporter == NULL)
    {
        throw oclint::GenericException(
            "cannot find dynamic library for report type: " + oclint::option::reportType());
    }
}

oclint::Reporter* reporter()
{
    return selectedReporter;
}
