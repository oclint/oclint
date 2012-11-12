#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
OCLINT_RULES_BUILD="$PROJECT_ROOT/build/oclint-rules"
OCLINT_RELEASE_BUILD="$PROJECT_ROOT/build/oclint-release"

# create directory and prepare for build
mkdir -p $OCLINT_RELEASE_BUILD
mkdir -p $OCLINT_RELEASE_BUILD/bin
mkdir -p $OCLINT_RELEASE_BUILD/lib/oclint
cd $OCLINT_RELEASE_BUILD

# put exetuable in place
cp $OCLINT_CORE_BUILD/bin/oclint* $OCLINT_RELEASE_BUILD/bin

# put rules in place
cp -rp $OCLINT_RULES_BUILD/rules.dl $OCLINT_RELEASE_BUILD/lib/oclint/rules

# put clang headers in place
cp -rp $LLVM_BUILD/lib/clang $OCLINT_RELEASE_BUILD/lib

# back to the current folder
cd $CWD
