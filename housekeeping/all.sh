#!/usr/bin/env bash

cd $(dirname $0)

./valgrind.sh
./coverage.sh
./cppcheck.sh
