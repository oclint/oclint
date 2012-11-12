#include <sstream>

#include "oclint/util/StdUtil.h"

string intToString(int number)
{
    stringstream buffer;
    buffer << number;
    return buffer.str();
}
