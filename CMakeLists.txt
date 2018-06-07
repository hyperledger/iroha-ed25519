cmake_minimum_required(VERSION 3.5)

set(ED25519_MAJOR_VERSION 1)
set(ED25519_MINOR_VERSION 3)
set(ED25519_PATCH_VERSION 0)
set(SOVERSION "${ED25519_MAJOR_VERSION}.${ED25519_MINOR_VERSION}.${ED25519_PATCH_VERSION}")
project(ed25519 VERSION ${SOVERSION} LANGUAGES C CXX)

enable_language(ASM)

set(CMAKE_POSITION_INDEPENDENT_CODE    TRUE)
set(CMAKE_C_VISIBILITY_PRESET          hidden)
set(CMAKE_CXX_VISIBILITY_PRESET        hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN    TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS      ON)
set(CMAKE_MODULE_PATH                  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

set(CMAKE_C_STANDARD                   11)    # force std=c11
set(CMAKE_C_STANDARD_REQUIRED          ON)
set(CMAKE_C_EXTENSIONS                 OFF)

set(CMAKE_CXX_STANDARD                 11)    # force std=c++11
set(CMAKE_CXX_STANDARD_REQUIRED        ON)
set(CMAKE_CXX_EXTENSIONS               OFF)

#add_compile_options(-Wall)
#add_compile_options(-Wextra)
#add_compile_options(-Wpedantic)

option(TESTING         "Enable testing"      OFF)
option(COVERAGE        "Enable coverage"     OFF)
option(BENCHMARKING    "Enable benchmarking" OFF)

include_directories(
  ${CMAKE_BINARY_DIR}/
  ${CMAKE_SOURCE_DIR}/include/
)

include(GNUInstallDirs)
include(GenerateExportHeader)
include(cmake/dependencies.cmake)
include(cmake/functions.cmake)
include(cmake/ed25519_init.cmake)
include(cmake/ed25519_export.cmake)
include(cmake/ed25519_target_link_libraries.cmake)
include(cmake/ed25519_add_library.cmake)
include(cmake/ed25519_merge_libraries.cmake)
include(cmake/ed25519_add_test.cmake)
test_build_amd64(CAN_BUILD_AMD64)

ed25519_init(EDIMPL HASH RANDOM)

# resulting shared/static library will export these symbols as GLOBAL symbols, and
# mark all others as LOCAL (hidden)
ed25519_export(
    ed25519_create_keypair
    ed25519_derive_public_key
    ed25519_sign
    ed25519_verify
    sha512_init
    sha512_final
    sha512_update
    sha512
    SHA_512_CONTEXT_SIZE
    sha256_init
    sha256_final
    sha256_update
    sha256
    SHA_256_CONTEXT_SIZE
    randombytes
)

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


if(NOT CMAKE_ASM_COMPILER_WORKS)
  message(WARNING "Can not find ASM compiler. Only EDIMPL=ref10 is available.")
  set(CAN_BUILD_AMD64 FALSE)
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


add_subdirectory(lib)
#if(BENCHMARKING)
#  add_subdirectory(benchmark)
#endif()


ed25519_merge_libraries(ed25519 ${BUILD}
  LIBRARIES
    ${EDIMPL}
    ${HASH}
    ${RANDOM}
  VERSION
    VERSION-${SOVERSION}-${EDIMPL}-${HASH}-${RANDOM}-${BUILD}-${CMAKE_BUILD_TYPE}
  )
set(_export_file ${CMAKE_BINARY_DIR}/ed25519_export.h)
generate_export_header(ed25519
  EXPORT_FILE_NAME ${_export_file}
  )
install(
  FILES ${_export_file}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ed25519/ed25519/
)
set_target_properties(ed25519 PROPERTIES
  VERSION   ${SOVERSION}
  SOVERSION ${SOVERSION}
  INTERFACE_ed25519_MAJOR_VERSION ${ED25519_MAJOR_VERSION}
  )
set_property(
  TARGET ed25519
  APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING ed25519_MAJOR_VERSION
  )
install(TARGETS ed25519 EXPORT ed25519Config
  LIBRARY DESTINATION       ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION       ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION       ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION      ${CMAKE_INSTALL_INCLUDEDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FRAMEWORK DESTINATION     ${CMAKE_INSTALL_PREFIX}
  )
install(
  DIRECTORY   ${CMAKE_SOURCE_DIR}/include/ed25519
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
install(
  EXPORT ed25519Config
  DESTINATION share/ed25519/cmake
  )
export(
  TARGETS ed25519
  FILE ed25519Config.cmake
  )



if(TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
