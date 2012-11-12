#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."

# update google test
cd $PROJECT_ROOT/googletest
svn up

# back to the current folder
cd $CWD
