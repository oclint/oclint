#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."

MODULE="core"

if [ $# -eq 1 ]; then
    MODULE=$1
fi

less $PROJECT_ROOT/build/oclint-$MODULE-test/testresults.txt
