#!/bin/sh

cd build
ctest --output-on-failure --verbose
cd ..
