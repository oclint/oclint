#ifndef OCLINT_UTIL_STDUTIL_H
#define OCLINT_UTIL_STDUTIL_H

#include <algorithm>
#include <sstream>
#include <string>

template <typename T>
std::string toString(T item)
{
    std::stringstream buffer;
    buffer << item;
    return buffer.str();
}

template <typename T>
bool vectorContains(T item, std::vector<T> &collection)
{
    return std::find(collection.begin(), collection.end(), item) != collection.end();
}

inline bool isUnderscore(char c) {
    return c == '_';
}

inline std::string removeUnderscores(std::string s) {
    s.erase(remove_if(s.begin(), s.end(), &::isUnderscore), s.end());
    return s;
}

inline std::string capitalize(std::string s) {
    std::string result = s.copy();
    if(s.length() > 0) {
        std::transform(s.begin(), s.begin() + 1, s.begin(), ::toupper);
        return s;
    else {
        return s;
    }
}

#endif
