macro(shatest name)
  addtest(test-${name} sha_test.cpp $<TARGET_OBJECTS:${name}>)

  gethash(${name} RESULT)
  if(${RESULT} STREQUAL "SHA2")
    set(testdata ${CMAKE_CURRENT_SOURCE_DIR}/sha2.input.txt)
  elseif(${RESULT} STREQUAL "SHA3")
    set(testdata ${CMAKE_CURRENT_SOURCE_DIR}/sha3.input.txt)
  else()
    message(FATAL_ERROR "${name} does not contain sha2/sha3")
  endif()

  target_compile_definitions(test-${name} PUBLIC
      -DTESTDATA_PATH=${testdata}
      )
endmacro()


shatest(${HASH})
