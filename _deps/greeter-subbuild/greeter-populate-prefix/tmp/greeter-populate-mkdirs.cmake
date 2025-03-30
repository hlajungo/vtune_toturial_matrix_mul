# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/standalone/.."
  "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/_deps/greeter-build"
  "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/_deps/greeter-subbuild/greeter-populate-prefix"
  "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/_deps/greeter-subbuild/greeter-populate-prefix/tmp"
  "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/_deps/greeter-subbuild/greeter-populate-prefix/src/greeter-populate-stamp"
  "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/_deps/greeter-subbuild/greeter-populate-prefix/src"
  "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/_deps/greeter-subbuild/greeter-populate-prefix/src/greeter-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/_deps/greeter-subbuild/greeter-populate-prefix/src/greeter-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/media/hlajungo/D/linux/test_place/cuda/UseCuda/TestTemplate/_deps/greeter-subbuild/greeter-populate-prefix/src/greeter-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
