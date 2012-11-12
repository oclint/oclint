#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
OCLINT_RULES_BUILD="$PROJECT_ROOT/build/oclint-rules"

# remove oclint-core build folder
rm -rf $OCLINT_RULES_BUILD
