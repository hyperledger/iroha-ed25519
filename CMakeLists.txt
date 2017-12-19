cmake_minimum_required(VERSION 3.0)
set(SOVERSION "1.2.0")
project(ed25519 VERSION ${SOVERSION} LANGUAGES C CXX)

SET(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wextra -O0 -fdiagnostics-color")
SET(CMAKE_C_FLAGS "-Wall")
SET(CMAKE_C_FLAGS_RELEASE "-O3 -funroll-loops -fomit-frame-pointer")
SET(CMAKE_C_FLAGS_DEBUG   "-g -Wextra -O0 -fdiagnostics-color ")
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

option(TESTING         "Enable testing"      ON)
option(COVERAGE        "Enable coverage"     OFF)
option(BENCHMARKING    "Enable benchmarking" OFF)

if(COVERAGE)
  if(CMAKE_BUILD_TYPE STREQUAL Release)
    message(FATAL_ERROR "Can't collect coverage in Release mode")
  endif()
  include(cmake/coverage.cmake)
endif()

include(GenerateExportHeader)
include(GNUInstallDirs)
include(cmake/dependencies.cmake)
include(cmake/functions.cmake)

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

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(lib)
if(BENCHMARKING)
  add_subdirectory(benchmark)
endif()


set(LIBED25519_VERSION "${SOVERSION}-${EDIMPL}-${HASH}-${RANDOM}")

add_library(ed25519 ${BUILD}
    src/ed25519.c
    )
target_compile_definitions(ed25519 PUBLIC
    -DLIBED25519_VERSION=${LIBED25519_VERSION}
    )
target_link_libraries(ed25519
    ${EDIMPL}
    ${HASH}
    ${RANDOM}
    )

setup_ed25519_install_cmake(ed25519 ed25519Config)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT ed25519Config DESTINATION share/ed25519/cmake)
export(TARGETS ed25519 ${EDIMPL} ${HASH} ${RANDOM} FILE ed25519Config.cmake)
target_include_directories(ed25519 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

set_target_properties(ed25519 PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMETWORK_IDENTIFIER warchant.ed25519
    VERSION   ${SOVERSION}
    SOVERSION ${SOVERSION}
    PUBLIC_HEADER include/ed25519/ed25519.h
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "warchant"
    )


if(TESTING)
  enable_testing()
  add_subdirectory(test)
endif()

set_target_properties(${EDIMPL} PROPERTIES EXCLUDE_FROM_ALL FALSE)
set_target_properties(${HASH}   PROPERTIES EXCLUDE_FROM_ALL FALSE)
set_target_properties(${RANDOM} PROPERTIES EXCLUDE_FROM_ALL FALSE)

