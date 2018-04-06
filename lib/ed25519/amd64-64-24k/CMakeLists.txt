include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.[cs])

set(_is_supported TRUE)
if(CMAKE_SYSTEM_NAME MATCHES "Darwin" OR NOT CAN_BUILD_AMD64)
  set(_is_supported FALSE)
endif()

ed25519_add_library(amd64-64-24k EDIMPL
  SOURCES ${sources}
  IS_SUPPORTED ${_is_supported}
  )
