#! /bin/sh -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
GOOGLE_TEST_SRC="$PROJECT_ROOT/googletest"
GOOGLE_TEST_BUILD="$PROJECT_ROOT/build/googletest"

EXTRA=
GENERATOR_FLAG=
GENERATOR=

OS=$(uname -s)
if [[ "$OS" =~ MINGW32 ]]; then
    GENERATOR_FLAG="-G"
    GENERATOR="MSYS Makefiles"
    # use default compiler (g++)
    EXTRA="-D gtest_disable_pthreads:BOOL=ON"
fi

# create directory and prepare for build
mkdir -p "$GOOGLE_TEST_BUILD"
cd "$GOOGLE_TEST_BUILD"

# configure and build
cmake $EXTRA "$GOOGLE_TEST_SRC" "$GENERATOR_FLAG" "$GENERATOR"
make

# back to the current folder
cd "$CWD"
