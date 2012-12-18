#! /bin/bash -e

# setup environment variables
CWD=`pwd`
PROJECT_ROOT="$CWD/.."
LLVM_SRC="$PROJECT_ROOT/llvm"
LLVM_BUILD="$PROJECT_ROOT/build/llvm-install"
OCLINT_CORE_SRC="$PROJECT_ROOT/oclint-core"
OCLINT_CORE_BUILD="$PROJECT_ROOT/build/oclint-core"
OCLINT_METRICS_SRC="$PROJECT_ROOT/oclint-metrics"
OCLINT_METRICS_BUILD="$PROJECT_ROOT/build/oclint-metrics"
OCLINT_RULES_SRC="$PROJECT_ROOT/oclint-rules"
OCLINT_RULES_BUILD="$PROJECT_ROOT/build/oclint-rules"
OCLINT_RELEASE_BUILD="$PROJECT_ROOT/build/oclint-release"

OCLINT_DOGFOODING="$PROJECT_ROOT/build/oclint-dogfooding"
OCLINT_DOGFOODING_CORE="$OCLINT_DOGFOODING/oclint-core"
OCLINT_DOGFOODING_RULES="$OCLINT_DOGFOODING/oclint-rules"
OCLINT_DOGFOODING_METRICS="$OCLINT_DOGFOODING/oclint-metrics"

# only show existing dogfooding results
if [ $# -eq 1 ] && [ $1 == "show" ]; then
    less $OCLINT_DOGFOODING/dogfoodingresults.txt
    exit 0
fi

# clean dogfooding directory
rm -rf $OCLINT_DOGFOODING
mkdir -p $OCLINT_DOGFOODING_CORE
mkdir -p $OCLINT_DOGFOODING_METRICS
mkdir -p $OCLINT_DOGFOODING_RULES

# generate compile_commands.json file
cd $OCLINT_DOGFOODING_CORE
cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD $OCLINT_CORE_SRC
cd $OCLINT_DOGFOODING_METRICS
cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD $OCLINT_METRICS_SRC
cd $OCLINT_DOGFOODING_RULES
cmake -D CMAKE_EXPORT_COMPILE_COMMANDS=ON -D CMAKE_CXX_COMPILER=$LLVM_BUILD/bin/clang++ -D CMAKE_C_COMPILER=$LLVM_BUILD/bin/clang -D LLVM_ROOT=$LLVM_BUILD -D OCLINT_SOURCE_DIR=$OCLINT_CORE_SRC -D OCLINT_BUILD_DIR=$OCLINT_CORE_BUILD -D OCLINT_METRICS_SOURCE_DIR=$OCLINT_METRICS_SRC -D OCLINT_METRICS_BUILD_DIR=$OCLINT_METRICS_BUILD $OCLINT_RULES_SRC

# copy compile_commands.json to source folders respectively
cp $OCLINT_DOGFOODING_CORE/compile_commands.json $OCLINT_CORE_SRC/compile_commands.json
cp $OCLINT_DOGFOODING_METRICS/compile_commands.json $OCLINT_METRICS_SRC/compile_commands.json
cp $OCLINT_DOGFOODING_RULES/compile_commands.json $OCLINT_RULES_SRC/compile_commands.json

# dog fooding for core
cd $OCLINT_CORE_SRC
$OCLINT_RELEASE_BUILD/bin/oclint-json-compilation-database -- -o $OCLINT_DOGFOODING/dogfooding_core_results.txt

# dog fooding for metrics
cd $OCLINT_METRICS_SRC
$OCLINT_RELEASE_BUILD/bin/oclint-json-compilation-database -- -o $OCLINT_DOGFOODING/dogfooding_metrics_results.txt

# dog fooding for rules
cd $OCLINT_RULES_SRC
$OCLINT_RELEASE_BUILD/bin/oclint-json-compilation-database -- -o $OCLINT_DOGFOODING/dogfooding_rules_results.txt

# display the results
cat $OCLINT_DOGFOODING/dogfooding_core_results.txt
cat $OCLINT_DOGFOODING/dogfooding_metrics_results.txt
cat $OCLINT_DOGFOODING/dogfooding_rules_results.txt

# back to the current folder
cd $CWD
