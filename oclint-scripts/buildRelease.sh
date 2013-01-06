#! /bin/bash -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
OCLINT_RULES_BUILD="$PROJECT_ROOT/build/oclint-rules"
OCLINT_RELEASE_BUILD="$PROJECT_ROOT/build/oclint-release"
LLVM_SRC="$PROJECT_ROOT/llvm"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_JSON_CD_FOLDER="$PROJECT_ROOT/oclint-json-compilation-database"
OCLINT_XCODBUILD_FOLDER="$PROJECT_ROOT/oclint-xcodebuild"

# clean test directory
if [ $# -eq 1 ] && [ $1 == "clean" ]; then
    rm -rf $OCLINT_RELEASE_BUILD
    exit 0
fi

# create directory and prepare for build
mkdir -p $OCLINT_RELEASE_BUILD
mkdir -p $OCLINT_RELEASE_BUILD/bin
mkdir -p $OCLINT_RELEASE_BUILD/lib/oclint
cd $OCLINT_RELEASE_BUILD

# put exetuable in place
cp $OCLINT_CORE_BUILD/bin/oclint* $OCLINT_RELEASE_BUILD/bin
cd $OCLINT_RELEASE_BUILD/bin
if [ -e oclint ]; then
    rm oclint
fi
ln -s oclint* oclint

# put rules in place
cp -rp $OCLINT_RULES_BUILD/rules.dl $OCLINT_RELEASE_BUILD/lib/oclint/rules

# put clang headers in place
cp -rp $LLVM_BUILD/lib/clang $OCLINT_RELEASE_BUILD/lib

# put oclint-json-compilation-database in place if exists
if [ -d "$OCLINT_JSON_CD_FOLDER" ]; then
    cp $OCLINT_JSON_CD_FOLDER/oclint* $OCLINT_RELEASE_BUILD/bin
fi

# put oclint-xcodebuild in place if exists
if [ -d "$OCLINT_XCODBUILD_FOLDER" ] && [ `uname` == "Darwin" ]; then
    cp $OCLINT_XCODBUILD_FOLDER/oclint* $OCLINT_RELEASE_BUILD/bin
fi

# licenses
cat $OCLINT_CORE_SRC/LICENSE > $OCLINT_RELEASE_BUILD/LICENSE
echo "" >> $OCLINT_RELEASE_BUILD/LICENSE
head -n 44 $LLVM_SRC/LICENSE.TXT >> $OCLINT_RELEASE_BUILD/LICENSE

# back to the current folder
cd $CWD
