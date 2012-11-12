#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."

# update all modules of llvm/clang
cd $PROJECT_ROOT/llvm
svn update
cd tools/clang
svn update
cd ../../projects/compiler-rt
svn update

# back to the current folder
cd $CWD
