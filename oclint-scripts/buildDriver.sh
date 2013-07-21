#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
OCLINT_DRIVER_SRC="$PROJECT_ROOT/oclint-driver"
OCLINT_DRIVER_BUILD="$PROJECT_ROOT/build/oclint-driver"
SUCCESS=0

EXTRA="-D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang"
GENERATOR_FLAG=
GENERATOR=

OS=$(uname -s)
if [[ "$OS" =~ MINGW32 ]]; then
    GENERATOR_FLAG="-G"
    GENERATOR="MSYS Makefiles"
    # use default compiler (g++)
    EXTRA=
fi

# clean test directory
if [ $# -eq 1 ] && [ "$1" = "clean" ]; then
    rm -rf $OCLINT_DRIVER_BUILD
    exit 0
fi

# configure for release build
RELEASE_CONFIG=""
if [ $# -eq 1 ] && [ "$1" = "release" ]; then
    RELEASE_CONFIG="-D OCLINT_BUILD_TYPE=Release"
fi

# create directory and prepare for build
mkdir -p $OCLINT_DRIVER_BUILD
cd $OCLINT_DRIVER_BUILD

# configure and build
cmake $RELEASE_CONFIG $EXTRA -D LLVM_ROOT="$LLVM_BUILD" -D OCLINT_SOURCE_DIR="$OCLINT_CORE_SRC" -D OCLINT_BUILD_DIR="$OCLINT_CORE_BUILD" "$OCLINT_DRIVER_SRC" "$GENERATOR_FLAG" "$GENERATOR"
make

# back to the current folder
cd "$CWD"
