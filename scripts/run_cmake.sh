#!/bin/bash

script_name="$(basename "$(realpath "${BASH_SOURCE[0]}")")"
script_dir="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
cpu_core_num=$(( $(nproc) * 3 / 4 ))

cd $script_dir/..

# cmake main
cmake -S . -B ./build/main -G "Unix Makefiles" \
  -DCMAKE_INSTALL_PREFIX=./install \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=$CC \
  -DCMAKE_CXX_COMPILER=$CXX
cmake --build ./build/main --parallel $cpu_core_num
cmake --install ./build/main

# cmake standalone
cmake -S ./standalone -B ./build/standalone -G "Unix Makefiles" \
  -DCMAKE_INSTALL_PREFIX=./install \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=$CC \
  -DCMAKE_CXX_COMPILER=$CXX
cmake --build ./build/standalone --parallel $cpu_core_num
cmake --install ./build/standalone

# cmake test

# cmake documentation
cmake -S ./documentation -B ./build/documentation -G "Unix Makefiles" \
  -DCMAKE_INSTALL_PREFIX=./install \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=$CC \
  -DCMAKE_CXX_COMPILER=$CXX
cmake --build ./build/documentation --parallel $cpu_core_num
cmake --install ./build/documentation


# template cpp
cmake -S . -B build/headers -DCMAKE_INSTALL_PREFIX="./install"
cmake --install ./build/headers/
