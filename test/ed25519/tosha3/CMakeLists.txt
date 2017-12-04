

add_executable(tosha3 tosha3.cpp)
target_compile_definitions(tosha3 PUBLIC
    -DTESTDATAIN_PATH=${CMAKE_SOURCE_DIR}/test/ed25519/sign.input.with.sha2.txt
    -DTESTDATAOUT_PATH=${CMAKE_SOURCE_DIR}/test/ed25519/sign.input.with.sha3.txt
    )
target_link_libraries(tosha3
    ref10
    sha3_brainhub
    dev_urandom
    )
set_target_properties(tosha3 PROPERTIES EXCLUDE_FROM_ALL TRUE)

