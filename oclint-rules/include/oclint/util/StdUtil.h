#ifndef OCLINT_UTIL_STDUTIL_H
#define OCLINT_UTIL_STDUTIL_H

#include <sstream>
#include <string>
#include <vector>

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

bool isUnderscore(char c);

std::string removeUnderscores(std::string s);

std::string capitalizeFirstLetter(std::string s);

#endif
