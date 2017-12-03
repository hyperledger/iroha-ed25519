

add_executable(tosha3 tosha3.cpp)
target_compile_definitions(tosha3 PUBLIC
    -DTESTDATAIN_PATH=${CMAKE_SOURCE_DIR}/test/ed25519/sign.input.with.sha2
    -DTESTDATAOUT_PATH=${CMAKE_SOURCE_DIR}/test/ed25519/sign.input.with.sha3
    )
target_link_libraries(tosha3
    ref10
    sha3_brainhub
    dev_urandom
    )
set_target_properties(tosha3 PROPERTIES EXCLUDE_FROM_ALL TRUE)


add_executable(edsha3test edsha3test.c)
target_link_libraries(edsha3test
    ref10
    sha3_brainhub
    dev_urandom
    )
set_target_properties(edsha3test PROPERTIES EXCLUDE_FROM_ALL TRUE)
