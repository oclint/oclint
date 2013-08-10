#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_METRICS_SRC="$PROJECT_ROOT/oclint-metrics"
OCLINT_METRICS_BUILD="$PROJECT_ROOT/build/oclint-metrics"

# clean test directory
if [ $# -eq 1 ] && [ "$1" = "clean" ]; then
    rm -rf "$OCLINT_METRICS_BUILD"
    exit 0
fi

# configure for release build
RELEASE_CONFIG=""
if [ $# -eq 1 ] && [ "$1" = "release" ]; then
    RELEASE_CONFIG="-D OCLINT_BUILD_TYPE=Release"
fi

# create directory and prepare for build
mkdir -p "$OCLINT_METRICS_BUILD"
cd "$OCLINT_METRICS_BUILD"

# configure and build
case `uname` in
    MINGW32*)
        cmake -G "MSYS Makefiles" $RELEASE_CONFIG -D LLVM_ROOT="$LLVM_BUILD" "$OCLINT_METRICS_SRC"
    ;;
    *)
        cmake $RELEASE_CONFIG -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD $OCLINT_METRICS_SRC
    ;;
esac
make

# back to the current folder
cd "$CWD"
