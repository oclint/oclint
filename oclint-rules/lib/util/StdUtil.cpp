#include <algorithm>

#include "oclint/util/StdUtil.h"


bool isUnderscore(char c) {
    return c == '_';
}

std::string removeUnderscores(std::string s) {
    s.erase(remove_if(s.begin(), s.end(), &::isUnderscore), s.end());
    return s;
}

std::string capitalizeFirstLetter(std::string s) {
    if(s.length() > 0) {
        std::transform(s.begin(), s.begin() + 1, s.begin(), ::toupper);
        return s;
    }
    else {
        return s;
    }
}

