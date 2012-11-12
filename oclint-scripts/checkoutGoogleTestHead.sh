#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."

# checkout googlemock and bundled googletest source code
cd $PROJECT_ROOT
svn checkout http://googlemock.googlecode.com/svn/trunk/ googletest

# back to the current folder
cd $CWD
