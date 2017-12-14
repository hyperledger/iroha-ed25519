set(LIB ref10)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
add_library(${LIB}
    ${sources}
    )
set_target_properties(${LIB} PROPERTIES EXCLUDE_FROM_ALL TRUE)

setup_ed25519_install_cmake(${LIB} ed25519Config)
