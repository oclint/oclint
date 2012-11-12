#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
OCLINT_RELEASE_BUILD="$PROJECT_ROOT/build/oclint-release"

# remove oclint-core build folder
rm -rf $OCLINT_RELEASE_BUILD
