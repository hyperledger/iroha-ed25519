macro(shatest name testdata)
  addtest(test-${name} sha_test.cpp)
  target_compile_definitions(test-${name} PUBLIC
      -DTESTDATA_PATH=${testdata}
      )
  target_link_libraries(test-${name} ${name})
endmacro()

if(OPENSSL_FOUND)
  shatest(sha2_openssl
      ${CMAKE_CURRENT_SOURCE_DIR}/sha2.input.txt
      )
else()
  message(WARNING "OpenSSL can not be found, test-sha2_openssl will not be built")
endif()

shatest(sha3_brainhub
    ${CMAKE_CURRENT_SOURCE_DIR}/sha3.input.txt
    )
