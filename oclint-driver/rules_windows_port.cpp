#include <dirent.h>
#include <windows.h>
#include <iostream>

#include "oclint/GenericException.h"

#include "rules.h"

void dynamicLoadRules(std::string ruleDirPath)
{
    DIR *pDir = opendir(ruleDirPath.c_str());
    if (pDir != NULL)
    {
        struct dirent *dirp;
        while ((dirp = readdir(pDir)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            std::string rulePath = ruleDirPath + "/" + std::string(dirp->d_name);
            HMODULE rule_library = LoadLibrary(rulePath.c_str());
            if (rule_library == NULL)
            {
                std::cerr << GetLastError() << std::endl;
                closedir(pDir);
                throw oclint::GenericException("cannot open dynamic library: " + rulePath);
            }
        }
        closedir(pDir);
    }
}
