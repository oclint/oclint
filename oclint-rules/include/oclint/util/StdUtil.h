#ifndef OCLINT_UTIL_STDUTIL_H
#define OCLINT_UTIL_STDUTIL_H

#include <sstream>
#include <string>

using namespace std;

template <typename T>
string toString(T item)
{
    stringstream buffer;
    buffer << item;
    return buffer.str();
}

template <typename T>
bool vectorContains(T item, vector<T> &collection)
{
    return find(collection.begin(), collection.end(), item) != collection.end();
}

#endif
