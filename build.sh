#!/bin/bash

mkdir -p ~/MarchingCubes/build
pushd ~/MarchingCubes/build
c++ -c -fpic ~/MarchingCubes/marchingcubes.cpp
c++ ~/MarchingCubes/marchingcubes.cpp -lOpenCL -o march -g `sdl2-config --cflags --libs` -DINTERNAL 
popd
