#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
OCLINT_CLANG_TOOLING_SRC="$PROJECT_ROOT/oclint-clang-tooling"
OCLINT_CLANG_TOOLING_BUILD="$PROJECT_ROOT/build/oclint-clang-tooling"
SUCCESS=0

# clean test directory
if [ $# -eq 1 ] && [ "$1" = "clean" ]; then
    rm -rf $OCLINT_CLANG_TOOLING_BUILD
    exit 0
fi

# configure for release build
RELEASE_CONFIG=""
if [ $# -eq 1 ] && [ "$1" = "release" ]; then
    RELEASE_CONFIG="-D OCLINT_BUILD_TYPE=Release"
fi

# create directory and prepare for build
mkdir -p $OCLINT_CLANG_TOOLING_BUILD
cd $OCLINT_CLANG_TOOLING_BUILD

# configure and build
cmake $RELEASE_CONFIG -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD -D OCLINT_SOURCE_DIR=$OCLINT_CORE_SRC -D OCLINT_BUILD_DIR=$OCLINT_CORE_BUILD $OCLINT_CLANG_TOOLING_SRC
make

# back to the current folder
cd $CWD
