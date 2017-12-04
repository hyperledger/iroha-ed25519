#!/usr/bin/env bash

HERE=$(dirname $0)
BUILD=$HERE/../build

cd $BUILD
make gcovr

if [ -z "$CODECOV_TOKEN" ]; then
	echo "Define CODECOV_TOKEN"
	exit 1
fi

bash <(curl -s https://codecov.io/bash) -f $BUILD/reports/gcovr.xml -t $CODECOV_TOKEN
