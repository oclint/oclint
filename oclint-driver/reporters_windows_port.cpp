#ifdef _MSC_VER
#include "oclint/win_port_dirent.h"
#else
#include <dirent.h>
#endif
#include <windows.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>

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
#ifdef _MSC_VER
	    HMODULE reporterHandle = LoadLibraryA(reporterPath.c_str());
#else
	    HMODULE reporterHandle = LoadLibrary(reporterPath.c_str());
#endif
            if (reporterHandle == NULL)
            {
                std::cerr << GetLastError() << std::endl;
                closedir(pDir);
                throw oclint::GenericException("in reporters cannot open dynamic library: " + reporterPath);
            }
            typedef oclint::Reporter* (*CreateReporterFunc)();
            CreateReporterFunc createMethodPointer;
            createMethodPointer = (CreateReporterFunc)GetProcAddress(reporterHandle, "create");
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
