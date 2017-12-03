#!/usr/bin/env bash

cd $(dirname $PWD)/build
BUILD_DIR=$PWD
REPORT_DIR=$BUILD_DIR/reports
GTEST_DIR=${BUILD_DIR}/bin
for test in $(ls $GTEST_DIR); do
VGXML=${REPORT_DIR}/valgrind-$test.xml
XUXML=${REPORT_DIR}/xunit-$test.xml

bin=$GTEST_DIR/$test
valgrind --xml=yes --xml-file=$VGXML \
  --leak-check=full --demangle=yes \
  $bin --gtest_output=xml:$XUXML
done
