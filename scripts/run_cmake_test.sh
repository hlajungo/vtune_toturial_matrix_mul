#!/bin/bash

script_name="$(basename "$(realpath "${BASH_SOURCE[0]}")")"
script_dir="$(dirname "$(realpath "${BASH_SOURCE[0]}")")"
cpu_core_num=$(( $(nproc) * 1 / 4 ))

cd $script_dir/..

cmake -S ./test -B ./build/test -G "Unix Makefiles" \
  -DCMAKE_INSTALL_PREFIX=./install \
  -DCMAKE_BUILD_TYPE=Debug

cmake --build ./build/test --parallel $cpu_core_num
cmake --install ./build/test
ctest --test-dir ./build/test

