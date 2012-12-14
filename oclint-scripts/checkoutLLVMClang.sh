#! /bin/bash -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
BRANCH="trunk"

if [ $# -eq 1 ] && [ $1 == "branch" ]; then
    echo "trunk (default)"
    echo "branches/release_32"
    echo "tags/RELEASE_32/final"
    exit 0
fi

if [ $# -eq 1 ] && [ $1 == "update" ]; then
    cd $PROJECT_ROOT/llvm
    svn update
    cd tools/clang
    svn update
    cd ../../projects/compiler-rt
    svn update
    cd $CWD
    exit 0
fi

if [ $# -eq 1 ]; then
    BRANCH=$1
fi

LLVM_REPO_URL="http://llvm.org/svn/llvm-project/llvm/$BRANCH"
CLANG_REPO_URL="http://llvm.org/svn/llvm-project/cfe/$BRANCH"
RT_REPO_URL="http://llvm.org/svn/llvm-project/compiler-rt/$BRANCH"

# checkout llvm/clang source code
cd $PROJECT_ROOT
svn co $LLVM_REPO_URL llvm
cd llvm/tools
svn co $CLANG_REPO_URL clang
cd ../..
cd llvm/projects
svn co $RT_REPO_URL compiler-rt

# back to the current folder
cd $CWD
