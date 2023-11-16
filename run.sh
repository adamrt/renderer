#!/bin/sh

mkdir -p build && \
cmake -S . -B build && \
cmake --build build -j 12 && \
./build/heretic
# valgrind --tool=callgrind ./build/heretic && kcachegrind callgrind.out.*
