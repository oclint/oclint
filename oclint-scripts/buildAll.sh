#! /bin/sh -e

# configure for release build
RELEASE_CONFIG=""
if [ $# -eq 1 ] && [ "$1" = "release" ]; then
    RELEASE_CONFIG="release"
fi

./buildCore.sh $RELEASE_CONFIG
./buildMetrics.sh $RELEASE_CONFIG
./buildRules.sh $RELEASE_CONFIG
./buildReporters.sh $RELEASE_CONFIG
./buildDriver.sh $RELEASE_CONFIG
./buildRelease.sh
