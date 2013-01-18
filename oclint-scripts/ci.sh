#! /bin/sh -e

./testCore.sh clean
./testMetrics.sh clean
./testRules.sh clean
./buildCore.sh clean
./buildMetrics.sh clean
./buildRules.sh clean
./buildRelease.sh clean
./testCore.sh
./testMetrics.sh
./testRules.sh
./buildCore.sh
./buildMetrics.sh
./buildRules.sh
./buildRelease.sh
./dogFooding.sh
