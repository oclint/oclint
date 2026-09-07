#include "oclint/win_port_dirent.h"
#include <windows.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>

#include "oclint/GenericException.h"

#include "rules.h"

void dynamicLoadRules(std::string ruleDirPath)
{
    DIR *pDir = opendir(ruleDirPath.c_str());
    if (pDir != nullptr)
    {
        struct dirent *dirp;
        while ((dirp = readdir(pDir)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            std::string rulePath = ruleDirPath + "/" + std::string(dirp->d_name);
#ifdef _MSC_VER
	    HMODULE rule_library = LoadLibraryA(rulePath.c_str());
#else
	    HMODULE rule_library = LoadLibrary(rulePath.c_str());
#endif
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
