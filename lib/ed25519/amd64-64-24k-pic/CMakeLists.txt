enable_language(ASM)
set(LIB amd64-64-24k-pic)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
FILE(GLOB asm ${CMAKE_CURRENT_SOURCE_DIR}/*.s)
add_library(${LIB} OBJECT
    ${sources}
    ${asm}
    )
set_target_properties(${LIB} PROPERTIES EXCLUDE_FROM_ALL TRUE)
