#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"

# remove oclint-core build folder
rm -rf $OCLINT_CORE_BUILD
