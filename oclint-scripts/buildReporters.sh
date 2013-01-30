#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
OCLINT_REPORTERS_SRC="$PROJECT_ROOT/oclint-reporters"
OCLINT_REPORTERS_BUILD="$PROJECT_ROOT/build/oclint-reporters"
SUCCESS=0

# clean test directory
if [ $# -eq 1 ] && [ "$1" = "clean" ]; then
    rm -rf $OCLINT_REPORTERS_BUILD
    exit 0
fi

# create directory and prepare for build
mkdir -p $OCLINT_REPORTERS_BUILD
cd $OCLINT_REPORTERS_BUILD

# configure and build
cmake -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD -D OCLINT_SOURCE_DIR=$OCLINT_CORE_SRC -D OCLINT_BUILD_DIR=$OCLINT_CORE_BUILD $OCLINT_REPORTERS_SRC
make

# back to the current folder
cd $CWD
