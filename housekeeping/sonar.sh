#!/bin/bash
cd $(dirname $0)/..

if [ -z "$SONAR_TOKEN" ]; then
	echo "Define SONAR_TOKEN"
	exit 1
fi

sonar-scanner -Dsonar.logn=$SONAR_TOKEN
