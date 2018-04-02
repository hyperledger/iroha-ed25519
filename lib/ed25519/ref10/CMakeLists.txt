set(LIB ref10)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
ed25519_add_library(ref10
    SOURCES ${sources}
    )
