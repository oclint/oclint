#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm"
LLVM_INSTALL="$PROJECT_ROOT/build/llvm-install"

EXTRA=
GENERATOR=

OS=$(uname -s)
if [[ "$OS" =~ MINGW32 ]]; then
    GENERATOR="MSYS Makefiles"
    # Compiler_RT may not compile on windows, so disable it
    EXTRA="-D LLVM_EXTERNAL_COMPILER_RT_BUILD:BOOL=OFF"
fi

# clean clang build directory
if [ $# -eq 1 ] && [ "$1" = "clean" ]; then
    rm -rf "$LLVM_BUILD"
    rm -rf "$LLVM_INSTALL"
    exit 0
fi

# configure for release build
RELEASE_CONFIG=""
if [ $# -eq 1 ] && [ "$1" = "release" ]; then
    RELEASE_CONFIG="-D CMAKE_BUILD_TYPE=Release"
fi

# create directory and prepare for build
mkdir -p "$LLVM_BUILD"
cd "$LLVM_BUILD"

if [ "$CPU_CORES" = "" ]; then
    # Default to building on all cores, but allow single core builds using:
    # "CPU_CORES=1 ./buildClang.sh release"
    CPU_CORES=1
    if [ "$OS" = "Linux" ]; then
        CPU_CORES=$(grep -c ^processor /proc/cpuinfo)
    elif [ "$OS" = "Darwin" ]; then
        CPU_CORES=$(sysctl -n hw.ncpu)
    fi
fi

# configure and build
cmake $RELEASE_CONFIG -D CMAKE_INSTALL_PREFIX="$LLVM_INSTALL" "$LLVM_SRC" ${GENERATOR:+-G "$GENERATOR"}
make -j $CPU_CORES
make install

# back to the current folder
cd "$CWD"
