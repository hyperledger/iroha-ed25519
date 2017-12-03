enable_language(ASM)
set(LIB amd64-64-24k)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
FILE(GLOB asm ${CMAKE_CURRENT_SOURCE_DIR}/*.s)
add_library(${LIB}
    ${sources}
    ${asm}
    )
