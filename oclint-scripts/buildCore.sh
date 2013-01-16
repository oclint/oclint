#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"

# clean test directory
if [ $# -eq 1 ] && [ $1 == "clean" ]; then
    rm -rf $OCLINT_CORE_BUILD
    exit 0
fi

# create directory and prepare for build
mkdir -p $OCLINT_CORE_BUILD
cd $OCLINT_CORE_BUILD

# configure and build
cmake -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD $OCLINT_CORE_SRC
make

# back to the current folder
cd $CWD
