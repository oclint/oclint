#! /bin/sh -e

./buildCore.sh
./buildMetrics.sh
./buildRules.sh
./buildReporters.sh
./buildClangTooling.sh
./buildRelease.sh
