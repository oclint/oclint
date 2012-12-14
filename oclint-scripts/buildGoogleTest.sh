#! /bin/bash -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
GOOGLE_TEST_SRC="$PROJECT_ROOT/googletest"
GOOGLE_TEST_BUILD="$PROJECT_ROOT/build/googletest"

# create directory and prepare for build
mkdir -p $GOOGLE_TEST_BUILD
cd $GOOGLE_TEST_BUILD

# configure and build
cmake $GOOGLE_TEST_SRC
make

# back to the current folder
cd $CWD
