cmake_minimum_required(VERSION 3.14...3.22)

# ---- Project ----

project (
  Graph
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message (
    FATAL_ERROR
    "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

# ---- Add dependencies via CPM ----

include (cmake/CPM.cmake)

CPMAddPackage ("gh:TheLartians/PackageProject.cmake@1.8.0")

#CPMAddPackage(
#NAME fmt
#GIT_TAG 10.2.1
#GITHUB_REPOSITORY fmtlib/fmt
#OPTIONS "FMT_INSTALL YES" # create an installable target
#)

# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.
file (GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.tpp" )
#file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cu")

# ---- C++ 20 Setting ----

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20" )

# ---- Create library ----

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(${PROJECT_NAME} INTERFACE)
add_library (Graph INTERFACE )
#add_library(Graph::Graph ALIAS Graph)
set_target_properties (${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

# add header to target
target_include_directories (${PROJECT_NAME} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)


# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options (${PROJECT_NAME} INTERFACE "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->")

# Link dependencies to target
#target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)


# ---- Create an installable target ----

# store string to variable
string (TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageProject (
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY SameMajorVersion
)
# DEPENDENCIES "fmt 10.2.1"
