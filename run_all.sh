#!/usr/bin/env bash

cmake -B build
cmake --build build

mkdir -p output

# build/PA2 testcases/cornell.txt output/cornell.bmp pathtracing
# build/PA2 testcases/glossy.txt output/glossy.bmp pathtracing
# build/PA2 testcases/scene08_smallpt_square.txt output/NEE.bmp NEE
# build/PA2 testcases/scene08_smallpt_square.txt output/noNEE.bmp pathtracing
# build/PA2 testcases/depth.txt output/depth.bmp pathtracing
# build/PA2 testcases/texture.txt output/texture.bmp pathtracing
build/PA2 testcases/china.txt output/china.bmp pathtracing
# build/PA2 testcases/move.txt output/move.bmp pathtracing