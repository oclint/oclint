#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
OCLINT_RULES_BUILD="$PROJECT_ROOT/build/oclint-rules"

less $OCLINT_RULES_BUILD/testresults.txt
