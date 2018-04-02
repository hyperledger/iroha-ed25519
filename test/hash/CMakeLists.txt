if(OPENSSL_FOUND)
  set(lib sha2_openssl)
  addtest(test-${lib} sha_test.cpp)
  target_link_libraries(test-${lib} PUBLIC
    ${lib}
    )
  target_compile_definitions(test-${lib} PUBLIC
    -DTESTDATA_PATH=${CMAKE_CURRENT_SOURCE_DIR}/sha2.input.txt
    )
  unset(lib)
endif()



set(lib sha3_brainhub)
addtest(test-${lib} sha_test.cpp)
target_link_libraries(test-${lib} PUBLIC
  ${lib}
  )
target_compile_definitions(test-${lib} PUBLIC
  -DTESTDATA_PATH=${CMAKE_CURRENT_SOURCE_DIR}/sha3.input.txt
  )
unset(lib)
