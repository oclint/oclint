#! /bin/bash

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
OCLINT_RULES_SRC="$PROJECT_ROOT/oclint-rules"
OCLINT_RULES_BUILD="$PROJECT_ROOT/build/oclint-rules"
OCLINT_RELEASE_BUILD="$PROJECT_ROOT/build/oclint-release"
OCLINT_CORE_TMP="$PROJECT_ROOT/build/oclint-core-tmp"
OCLINT_RULES_TMP="$PROJECT_ROOT/build/oclint-rules-tmp"

# create temporary directory
mkdir -p $OCLINT_CORE_TMP
mkdir -p $OCLINT_RULES_TMP

# generate compile_commands.json file
cd $OCLINT_CORE_TMP
cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD $OCLINT_CORE_SRC
cd $OCLINT_RULES_TMP
cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD -D OCLINT_SOURCE_DIR=$OCLINT_CORE_SRC -D OCLINT_BUILD_DIR=$OCLINT_CORE_BUILD $OCLINT_RULES_SRC

# copy compile_commands.json to source folders respectively
cp $OCLINT_CORE_TMP/compile_commands.json $OCLINT_CORE_SRC/compile_commands.json
cp $OCLINT_RULES_TMP/compile_commands.json $OCLINT_RULES_SRC/compile_commands.json

# dog fooding for core
cd $OCLINT_CORE_SRC
$OCLINT_RELEASE_BUILD/bin/oclint-json-compilation-database

# dog fooding for rules
cd $OCLINT_RULES_SRC
$OCLINT_RELEASE_BUILD/bin/oclint-json-compilation-database

# delete temporary folders
rm -rf $OCLINT_CORE_TMP
rm -rf $OCLINT_RULES_TMP

# back to the current folder
cd $CWD
