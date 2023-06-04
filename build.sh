#!/bin/sh

rm -rf build
cmake -S . -B build
cd build
make
cd ..
cp build/compile_commands.json compile_commands.json
