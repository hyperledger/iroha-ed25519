cmake_minimum_required(VERSION 3.5)
set(ED25519_MAJOR_VERSION 1)
set(ED25519_MINOR_VERSION 3)
set(ED25519_PATCH_VERSION 0)
set(SOVERSION "${ED25519_MAJOR_VERSION}.${ED25519_MINOR_VERSION}.${ED25519_PATCH_VERSION}")
project(ed25519 VERSION ${SOVERSION} LANGUAGES C CXX)


set(CMAKE_POSITION_INDEPENDENT_CODE    TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS      ON)
set(CMAKE_MODULE_PATH                  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")
set(EXECUTABLE_OUTPUT_PATH             "${PROJECT_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH                "${PROJECT_BINARY_DIR}")
set(ARCHIVE_OUTPUT_DIRECTORY           "${PROJECT_BINARY_DIR}")

set(CMAKE_C_STANDARD                   11)    # force std=c11
set(CMAKE_C_STANDARD_REQUIRED          ON)
set(CMAKE_C_EXTENSIONS                 OFF)
set(CMAKE_C_FLAGS_RELEASE              "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG                "${CMAKE_C_FLAGS_DEBUG} -g -O0 -fdiagnostics-color ")

set(CMAKE_CXX_STANDARD                 11)    # force std=c++11
set(CMAKE_CXX_STANDARD_REQUIRED        ON)
set(CMAKE_CXX_EXTENSIONS               OFF)
set(CMAKE_CXX_FLAGS_RELEASE            "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG              "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -fdiagnostics-color")

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wpedantic)

option(TESTING         "Enable testing"      ON)
option(COVERAGE        "Enable coverage"     OFF)
option(BENCHMARKING    "Enable benchmarking" OFF)

if(COVERAGE)
  if(CMAKE_BUILD_TYPE STREQUAL Release)
    message(FATAL_ERROR "Can't collect coverage in Release mode")
  endif()
  include(cmake/coverage.cmake)
endif()

include(GNUInstallDirs)
include(cmake/dependencies.cmake)
include(cmake/functions.cmake)
enable_language(ASM)
test_build_amd64(CAN_BUILD_AMD64)


## DEFAULTS
if(NOT EDIMPL)
  set(EDIMPL "ref10")
endif()
if(NOT HASH)
  set(HASH   "sha3_brainhub")
endif()
if(NOT RANDOM)
  # https://sockpuppet.org/blog/2014/02/25/safely-generate-random-numbers/
  set(RANDOM "dev_urandom")
endif()
if(NOT BUILD)
  set(BUILD  "SHARED")
endif()
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE  "Release")
endif()

## OPTIONS
ENUM(EDIMPL "${EDIMPL}" "Ed25519 implementation"
  ref10
  amd64-64-24k
  amd64-64-24k-pic
  )
ENUM(HASH "${HASH}" "SHA implementation"
  sha2_openssl
  sha3_brainhub
  )
ENUM(RANDOM "${RANDOM}" "RNG implementation"
  rand_openssl
  dev_random
  dev_urandom
  )
ENUM(BUILD "${BUILD}" "library build type"
  SHARED
  STATIC
  )
ENUM(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" "cmake build type"
  Release
  Debug
  )

if(BUILD STREQUAL "SHARED" AND EDIMPL STREQUAL "amd64-64-24k")
  message(FATAL_ERROR
    "EDIMPL=amd64-64-24k implementation has no PIC fixes, "
    "therefore SHARED library can not be built. "
    "Use EDIMPL=amd64-64-24k-pic or BUILD=STATIC")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib)
if(BENCHMARKING)
  add_subdirectory(benchmark)
endif()

add_subdirectory(src)

if(TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
