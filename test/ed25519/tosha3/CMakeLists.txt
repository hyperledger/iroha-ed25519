add_executable(tosha3
    tosha3.cpp
    $<TARGET_OBJECTS:${EDIMPL}>
    $<TARGET_OBJECTS:sha3_brainhub>
    $<TARGET_OBJECTS:${RANDOM}>
    )
target_compile_definitions(tosha3 PUBLIC
    -DTESTDATAIN_PATH=${CMAKE_SOURCE_DIR}/test/ed25519/sign.input.with.sha2.txt
    -DTESTDATAOUT_PATH=${CMAKE_SOURCE_DIR}/test/ed25519/sign.input.with.sha3.txt
    )
set_target_properties(tosha3 PROPERTIES EXCLUDE_FROM_ALL TRUE)

