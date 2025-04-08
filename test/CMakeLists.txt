cmake_minimum_required(VERSION 3.14...3.22)

# ---- Project name -----

set (TEST_PROJECT_PREFIX "Matrix_multiplication")
set (TEST_PROJECT_SUFFIX "Tests")
project (${TEST_PROJECT_PREFIX}_${TEST_PROJECT_SUFFIX} LANGUAGES CXX)

# ---- Options for -D ----

option (ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option (TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

# --- Import tools ----

include (../cmake/tools.cmake)

# ---- CPM dependencies ----

include (../cmake/CPM.cmake)

CPMAddPackage ("gh:doctest/doctest@2.4.9")
CPMAddPackage ("gh:TheLartians/Format.cmake@1.7.3")

# ---- dependencies

if (TEST_INSTALLED_VERSION)
  find_package (${TEST_PROJECT_PREFIX} CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../install)
else ()
  CPMAddPackage (NAME ${TEST_PROJECT_PREFIX} SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif ()


# ---- Compiler ----

set (OpenMP_CXX_FLAGS "-qopenmp")
set (CMAKE_C_COMPILER icx)
set (CMAKE_CXX_COMPILER icpx)

# ---- Global setting

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")


#set (OPT_FLAGS "-Ofast")
set (OPT_FLAGS "-g -qopenmp -march=native")
set (MKL_SHARE_LIB "-lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core")
set (ITT_STATIC_LIB $ENV{VTUNE_ROOT}/lib64/libittnotify.a)

file (GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

###### Target ######
# ---- Code ----
# ---- Macros ----
# ---- Flags ----
# ---- Include path ----
# ---- Library path ----
# ---- Library ----



###### Target ######
add_executable (${PROJECT_NAME})
# ---- Code ----
target_sources (${PROJECT_NAME} PRIVATE ${sources})
# ---- Macros ----
#target_compile_definitions (${PROJECT_NAME} PUBLIC MATRIX_SIZE=4096)
# ---- Flags ----
target_compile_options (${PROJECT_NAME} PRIVATE -g -qopenmp -march=native)
#target_compile_options (${PROJECT_NAME} PRIVATE -g -march=native)
#target_compile_options (${PROJECT_NAME} PRIVATE -g -Ofast)
# ---- Include path ----
# ---- Library path ----
target_link_directories (${PROJECT_NAME} PRIVATE $ENV{MKLROOT})
# ---- Library ----
target_link_libraries   (${PROJECT_NAME} PRIVATE doctest::doctest ${TEST_PROJECT_PREFIX})
target_link_libraries   (${PROJECT_NAME} PRIVATE ${ITT_STATIC_LIB} ${MKL_SHARE_LIB} iomp5)




# --- Add test ----
add_test (NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} )


#message(STATUS "CMAKE_CUDA_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

# enable compiler warnings
if (NOT TEST_INSTALLED_VERSION)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    #target_compile_options(UseCudaLib PUBLIC -Wall -Wpedantic -Wextra -Werror)
    target_compile_options (${PROJECT_NAME} PUBLIC
      $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wpedantic -Wextra -Werror>
      $<$<COMPILE_LANGUAGE:CUDA>:--compiler-options=-Wall,-Wextra,-Werror>
    )
  elseif (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /W4 /WX)
    target_compile_definitions(${PROJECT_NAME} PUBLIC DOCTEST_CONFIG_USE_STD_HEADERS)
  endif ()
endif ()

enable_testing ()

# Note: doctest and similar testing frameworks can automatically configure CMake tests. For other
# testing frameworks add the tests target instead: add_test(NAME ${PROJECT_NAME} COMMAND
# ${PROJECT_NAME})

include (${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests (${PROJECT_NAME})

install (TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION tests
)

# ---- code coverage ----

if (ENABLE_TEST_COVERAGE)
  target_compile_options (${TEST_PROJECT_PREFIX} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
  target_link_options (${TEST_PROJECT_PREFIX} PUBLIC -fprofile-arcs -ftest-coverage)
endif ()
