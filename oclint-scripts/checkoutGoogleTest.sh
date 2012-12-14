#! /bin/bash -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."

if [ $# -eq 1 ] && [ $1 == "update" ]; then
    cd $PROJECT_ROOT/googletest
    svn up
    exit 0
fi

# checkout googlemock and bundled googletest source code
cd $PROJECT_ROOT
svn checkout http://googlemock.googlecode.com/svn/trunk/ googletest

# back to the current folder
cd $CWD
