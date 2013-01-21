#! /bin/sh -e

./testCore.sh clean
./testMetrics.sh clean
./testRules.sh clean
./testReporters.sh clean
./buildCore.sh clean
./buildMetrics.sh clean
./buildRules.sh clean
./buildReporters.sh clean
./buildRelease.sh clean
./testCore.sh
./testMetrics.sh
./testRules.sh
./testReporters.sh
./buildCore.sh
./buildMetrics.sh
./buildRules.sh
./buildReporters.sh
./buildRelease.sh
./dogFooding.sh
