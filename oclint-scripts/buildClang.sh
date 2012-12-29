#! /bin/bash -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm"
LLVM_INSTALL="$PROJECT_ROOT/build/llvm-install"

# create directory and prepare for build
mkdir -p $LLVM_BUILD
cd $LLVM_BUILD

# configure and build
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=$LLVM_INSTALL $LLVM_SRC
make
make install

# back to the current folder
cd $CWD
