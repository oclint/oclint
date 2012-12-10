#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
GOOGLE_TEST_SRC="$PROJECT_ROOT/googletest"
GOOGLE_TEST_BUILD="$PROJECT_ROOT/build/googletest"
OCLINT_METRICS_SRC="$PROJECT_ROOT/oclint-metrics"
OCLINT_METRICS_BUILD="$PROJECT_ROOT/build/oclint-metrics"
SUCCESS=0

# create directory and prepare for build
mkdir -p $OCLINT_METRICS_BUILD
cd $OCLINT_METRICS_BUILD

# configure and build
if [ $SUCCESS -eq 0 ]; then
    cmake -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD -D GOOGLETEST_SRC=$GOOGLE_TEST_SRC -D GOOGLETEST_BUILD=$GOOGLE_TEST_BUILD -D TEST_BUILD=1 $OCLINT_METRICS_SRC
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
    ctest --output-on-failure > $OCLINT_METRICS_BUILD/testresults.txt
    if [ $? -ne 0 ]; then
        SUCCESS=3
    fi
    cat $OCLINT_METRICS_BUILD/testresults.txt
fi
if [ $SUCCESS -eq 0 ]; then
    lcov -b . -d . -c -o $OCLINT_METRICS_BUILD/output.lcov
    lcov -e $OCLINT_METRICS_BUILD/output.lcov "*oclint-metrics*" -o $OCLINT_METRICS_BUILD/output.lcov
    lcov -r $OCLINT_METRICS_BUILD/output.lcov "*test*" -o $OCLINT_METRICS_BUILD/output.lcov
    genhtml -o $OCLINT_METRICS_BUILD/coveragereport -t "OCLint Metrics test coverage" --num-spaces 4 $OCLINT_METRICS_BUILD/output.lcov
    if [ $? -ne 0 ]; then
        SUCCESS=4
    fi
fi

# back to the current folder
cd $CWD
exit $SUCCESS
