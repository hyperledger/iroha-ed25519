include_directories(${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
ed25519_add_library(ref10 EDIMPL
  SOURCES ${sources}
  IS_SUPPORTED TRUE  # pure C implementation, will work on all platforms
  )
