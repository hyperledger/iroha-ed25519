include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.[cs])

ed25519_add_library(amd64-64-24k-pic EDIMPL
    SOURCES ${sources}
    IS_SUPPORTED ${CAN_BUILD_AMD64}
    )
