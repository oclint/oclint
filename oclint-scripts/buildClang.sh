#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm"
LLVM_INSTALL="$PROJECT_ROOT/build/llvm-install"

# clean clang build directory
if [ $# -eq 1 ] && [ "$1" = "clean" ]; then
    rm -rf $LLVM_BUILD
    rm -rf $LLVM_INSTALL
    exit 0
fi

# configure for release build
RELEASE_CONFIG=""
if [ $# -eq 1 ] && [ "$1" = "release" ]; then
    RELEASE_CONFIG="-D CMAKE_BUILD_TYPE=Release"
fi

# create directory and prepare for build
mkdir -p $LLVM_BUILD
cd $LLVM_BUILD

# configure and build
cmake $RELEASE_CONFIG -D CMAKE_INSTALL_PREFIX=$LLVM_INSTALL $LLVM_SRC
make
make install

# back to the current folder
cd $CWD
