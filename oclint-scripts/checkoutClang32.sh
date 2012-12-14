#! /bin/bash -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."

# checkout llvm/clang source code
cd $PROJECT_ROOT
svn co http://llvm.org/svn/llvm-project/llvm/branches/release_32 llvm
cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/branches/release_32 clang
cd ../..
cd llvm/projects
svn co http://llvm.org/svn/llvm-project/compiler-rt/branches/release_32 compiler-rt

# back to the current folder
cd $CWD
