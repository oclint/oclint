#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
OCLINT_METRICS_BUILD="$PROJECT_ROOT/build/oclint-metrics"

# remove oclint-metrics build folder
rm -rf $OCLINT_METRICS_BUILD
