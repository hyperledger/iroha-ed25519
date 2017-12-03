set(LIB ref10)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_library(${LIB}
    ${sources}
    )
