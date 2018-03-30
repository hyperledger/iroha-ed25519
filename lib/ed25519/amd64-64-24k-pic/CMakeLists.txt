include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.[cs])

ed25519_add_library(amd64-64-24k-pic
    SOURCES ${sources}
    )
