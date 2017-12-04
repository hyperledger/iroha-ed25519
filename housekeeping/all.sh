#!/usr/bin/env bash
HERE=$(dirname $0)
BUILD=$HERE/../build

if [ ! -d "$BUILD" ]; then
	echo "build ed25519 first"
	exit 1
fi

cd $HERE

./valgrind.sh
./cppcheck.sh
./coverage.sh
./sonar.sh
