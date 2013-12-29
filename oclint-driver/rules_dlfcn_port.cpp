#include <dirent.h>
#include <dlfcn.h>
#include <iostream>

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
            if (dlopen(rulePath.c_str(), RTLD_LAZY) == nullptr)
            {
                std::cerr << dlerror() << std::endl;
                closedir(pDir);
                throw oclint::GenericException("cannot open dynamic library: " + rulePath);
            }
        }
        closedir(pDir);
    }
}
