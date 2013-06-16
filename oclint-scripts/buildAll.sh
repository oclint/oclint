#! /bin/sh -e

./buildCore.sh
./buildMetrics.sh
./buildRules.sh
./buildReporters.sh
./buildDriver.sh
./buildRelease.sh
