cmake_minimum_required(VERSION 3.0)
project(ed25519 C CXX)

SET(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
SET(CMAKE_CXX_FLAGS "-std=c++14 -Wall")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG   "-g -Wextra -O0 -fdiagnostics-color")
SET(CMAKE_C_FLAGS "-Wall -funroll-loops")
SET(CMAKE_C_FLAGS_RELEASE "-O3")
SET(CMAKE_C_FLAGS_DEBUG   "-g -Wextra -O0 -fdiagnostics-color")
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

option(TESTING         "Enable testing"      ON)
option(COVERAGE        "Enable coverage"     ON)
option(AMD64_OPTIMIZED "Enable amd64-64-24k" OFF)

if(COVERAGE)
  include(cmake/coverage.cmake)
endif()

include(cmake/dependencies.cmake)
include(cmake/functions.cmake)

# auto by default
if(NOT EDIMPL)
  set(EDIMPL "ref10")
endif()
if(NOT HASH)
  set(HASH   "sha3_brainhub")
endif()
if(NOT RANDOM)
  set(RANDOM "dev_urandom")
endif()
if(NOT BUILD)
  set(BUILD  "SHARED")
endif()

set(EDIMPL_OPTIONS ref10)
if(AMD64_OPTIMIZED)
  list(APPEND EDIMPL_OPTIONS amd64-64-24k)
endif()

ENUM(EDIMPL "${EDIMPL}" "Ed25519 implementation"
    ${EDIMPL_OPTIONS}
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


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_subdirectory(lib)


set(SOVERSION "1.0.0")
set(LIBED25519_VERSION "${SOVERSION}-${EDIMPL}-${HASH}-${RANDOM}")


add_library(ed25519 ${BUILD}
    src/ed25519.c
    )
target_compile_definitions(ed25519 PUBLIC
    -DLIBED25519_VERSION=${LIBED25519_VERSION}
    )
target_link_libraries(ed25519
    ${EDLIB}
    ${HASH}
    ${RANDOM}
    )
install(TARGETS ed25519
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    CONFIGURATIONS Release
    )
set_target_properties(ed25519 PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION C
    MACOSX_FRAMETWORK_IDENTIFIER warchant.ed25519
    VERSION ${LIBED25519_VERSION}
    SOVERSION ${SOVERSION}
    PUBLIC_HEADER include/ed25519.h
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "warchant"
    )


if(TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
