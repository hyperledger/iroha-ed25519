cmake_minimum_required(VERSION 3.5)


find_program(CCACHE_FOUND ccache)
if (CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif (CCACHE_FOUND)


include("cmake/Hunter/init.cmake")
HunterGate(
  URL "https://github.com/ruslo/hunter/archive/v0.23.127.tar.gz"
  SHA1 "2cd79807cb829127896811f8afa15e790b6bde19"
)


set(ED25519_MAJOR_VERSION 2)
set(ED25519_MINOR_VERSION 0)
set(ED25519_PATCH_VERSION 0)
set(SOVERSION "${ED25519_MAJOR_VERSION}.${ED25519_MINOR_VERSION}.${ED25519_PATCH_VERSION}")
project(ed25519 VERSION ${SOVERSION} LANGUAGES C CXX)

enable_language(ASM)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

  set(CMAKE_C_STANDARD 11)    # force std=c11
  set(CMAKE_C_STANDARD_REQUIRED ON)
  set(CMAKE_C_EXTENSIONS OFF)

  set(CMAKE_CXX_STANDARD 11)    # force std=c++11
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

option(TESTING "Enable testing" OFF)
option(COVERAGE "Enable coverage" OFF)
option(BENCHMARKING "Enable benchmarking" OFF)
option(FUZZING "Enable fuzzing targets" OFF)

include_directories(
  ${CMAKE_BINARY_DIR}/
  ${CMAKE_SOURCE_DIR}/include/
)

include(GNUInstallDirs)
include(cmake/dependencies.cmake)
include(cmake/functions.cmake)
include(cmake/ed25519_init.cmake)
include(cmake/ed25519_target_link_libraries.cmake)
include(cmake/ed25519_add_library.cmake)
include(cmake/ed25519_merge_libraries.cmake)
include(cmake/ed25519_add_test.cmake)
include(cmake/ed25519_add_fuzz.cmake)
include(cmake/ed25519_select_supported_impl.cmake)
test_build_amd64(CAN_BUILD_AMD64)

ed25519_init(EDIMPL HASH RANDOM)

add_subdirectory(lib)

## DEFAULTS
if (NOT EDIMPL)
  ed25519_select_supported_impl(EDIMPL amd64-64-24k-pic ref10)
endif ()
if (NOT HASH)
  ed25519_select_supported_impl(HASH sha2_sphlib sha2_openssl)
endif ()
if (NOT RANDOM)
  # https://sockpuppet.org/blog/2014/02/25/safely-generate-random-numbers/
  ed25519_select_supported_impl(RANDOM dev_urandom bcryptgen rand_openssl)
endif ()
if (NOT BUILD)
  set(BUILD "STATIC")
endif ()


if (NOT CMAKE_ASM_COMPILER_WORKS)
  message(WARNING "Can not find ASM compiler. Only EDIMPL=ref10 is available.")
  set(CAN_BUILD_AMD64 FALSE)
endif ()


## OPTIONS
ENUM(EDIMPL "${EDIMPL}" "Ed25519 implementation"
  ref10
  amd64-64-24k-pic
  )
ENUM(HASH "${HASH}" "SHA implementation"
  sha2_openssl
  sha2_sphlib
  sha3_brainhub
  )
ENUM(RANDOM "${RANDOM}" "RNG implementation"
  rand_openssl
  dev_random
  dev_urandom
  bcryptgen
  )
ENUM(BUILD "${BUILD}" "library build type"
  SHARED
  STATIC
  )


ed25519_merge_libraries(ed25519 ${BUILD}
  LIBRARIES
  ${EDIMPL}
  ${HASH}
  ${RANDOM}
  VERSION
  VERSION-${SOVERSION}-${EDIMPL}-${HASH}-${RANDOM}-${BUILD}-${CMAKE_BUILD_TYPE}
  )
set_target_properties(ed25519 PROPERTIES
  VERSION ${SOVERSION}
  SOVERSION ${SOVERSION}
  INTERFACE_ed25519_MAJOR_VERSION ${ED25519_MAJOR_VERSION}
  )
set_property(
  TARGET ed25519
  APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING ed25519_MAJOR_VERSION
)
install(TARGETS ed25519 EXPORT ed25519Config
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FRAMEWORK DESTINATION ${CMAKE_INSTALL_PREFIX}
  )
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include/ed25519
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(
  EXPORT ed25519Config
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ed25519
  NAMESPACE iroha::
)
install(
  FILES ${CMAKE_SOURCE_DIR}/linker_exportmap
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ed25519
)
export(
  TARGETS ed25519
  FILE ed25519Config.cmake
)


if (TESTING)
  enable_testing()
  add_subdirectory(test)
endif ()


if (BENCHMARK)
  add_subdirectory(benchmark)
endif ()


if (FUZZING)
  add_subdirectory(fuzzing)
endif ()
