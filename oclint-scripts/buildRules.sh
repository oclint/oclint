#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
#LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
OCLINT_METRICS_SRC="$PROJECT_ROOT/oclint-metrics"
OCLINT_METRICS_BUILD="$PROJECT_ROOT/build/oclint-metrics"
OCLINT_RULES_SRC="$PROJECT_ROOT/oclint-rules"
OCLINT_RULES_BUILD="$PROJECT_ROOT/build/oclint-rules"

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
    rm -rf $OCLINT_RULES_BUILD
    exit 0
fi

# configure for release build
RELEASE_CONFIG=""
if [ $# -eq 1 ] && [ "$1" = "release" ]; then
    RELEASE_CONFIG="-D OCLINT_BUILD_TYPE=Release"
fi

# create directory and prepare for build
mkdir -p "$OCLINT_RULES_BUILD"
cd "$OCLINT_RULES_BUILD"

# configure and build
cmake $RELEASE_CONFIG $EXTRA -D LLVM_ROOT="$LLVM_BUILD" -D OCLINT_SOURCE_DIR="$OCLINT_CORE_SRC" -D OCLINT_BUILD_DIR="$OCLINT_CORE_BUILD" -D OCLINT_METRICS_SOURCE_DIR="$OCLINT_METRICS_SRC" -D OCLINT_METRICS_BUILD_DIR="$OCLINT_METRICS_BUILD" "$OCLINT_RULES_SRC" "$GENERATOR_FLAG" "$GENERATOR"
make

# back to the current folder
cd "$CWD"
