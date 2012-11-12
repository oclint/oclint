#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
GOOGLE_TEST_SRC="$PROJECT_ROOT/googletest"
GOOGLE_TEST_BUILD="$PROJECT_ROOT/build/googletest"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
SUCCESS=0

# create directory and prepare for build
mkdir -p $OCLINT_CORE_BUILD
cd $OCLINT_CORE_BUILD

# configure and build
if [ $SUCCESS -eq 0 ]; then
    cmake -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD -D GOOGLETEST_SRC=$GOOGLE_TEST_SRC -D GOOGLETEST_BUILD=$GOOGLE_TEST_BUILD -D TEST_BUILD=1 $OCLINT_CORE_SRC
    if [ $? -ne 0 ]; then
        SUCCESS=1
    fi
fi
if [ $SUCCESS -eq 0 ]; then
    make
    if [ $? -ne 0 ]; then
        SUCCESS=2
    fi
fi
if [ $SUCCESS -eq 0 ]; then
    $OCLINT_CORE_BUILD/bin/oclint_test > $OCLINT_CORE_BUILD/testresults.txt
    if [ $? -ne 0 ]; then
        SUCCESS=3
    fi
    cat $OCLINT_CORE_BUILD/testresults.txt
fi
if [ $SUCCESS -eq 0 ]; then
    lcov -b . -d . -c -o $OCLINT_CORE_BUILD/output.lcov
    lcov -e $OCLINT_CORE_BUILD/output.lcov "*oclint-core*" -o $OCLINT_CORE_BUILD/output.lcov
    lcov -r $OCLINT_CORE_BUILD/output.lcov "*test*" -o $OCLINT_CORE_BUILD/output.lcov
    genhtml -o $OCLINT_CORE_BUILD/coveragereport -t "OCLint test coverage" --num-spaces 4 $OCLINT_CORE_BUILD/output.lcov
    if [ $? -ne 0 ]; then
        SUCCESS=4
    fi
fi

# back to the current folder
cd $CWD
exit $SUCCESS
