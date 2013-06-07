#! /bin/sh -e

./testCore.sh clean
./testMetrics.sh clean
./testRules.sh clean
./testReporters.sh clean
./buildCore.sh clean
./buildMetrics.sh clean
./buildRules.sh clean
./buildReporters.sh clean
./buildDriver.sh clean
./testAll.sh
./buildAll.sh
./dogFooding.sh
