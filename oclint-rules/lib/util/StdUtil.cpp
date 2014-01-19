#include "oclint/util/StdUtil.h"

#include <algorithm>

bool isUnderscore(char aChar) {
    return aChar == '_';
}

std::string removeUnderscores(std::string str) {
    str.erase(remove_if(str.begin(), str.end(), &::isUnderscore), str.end());
    return str;
}

std::string capitalizeFirstLetter(std::string str) {
    if(str.length() > 0) {
        std::transform(str.begin(), str.begin() + 1, str.begin(), ::toupper);
    }
    return str;
}

