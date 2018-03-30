include_directories(${CMAKE_CURRENT_BINARY_DIR})

macro(edtest edname hash rand)
  addtest(test-${edname}
      ed25519_test.cpp
      )
  target_link_libraries(test-${edname} PUBLIC
      ed25519
      )

  gethash(${hash} RESULT)
  if(${RESULT} STREQUAL "SHA2")
    set(sign_input ${CMAKE_CURRENT_SOURCE_DIR}/sign.input.with.sha2.txt)
  elseif(${RESULT} STREQUAL "SHA3")
    set(sign_input ${CMAKE_CURRENT_SOURCE_DIR}/sign.input.with.sha3.txt)
  else()
    message(FATAL_ERROR "${hash} target does not contain sha2/sha3 in its name. Can't identify testvectors for input")
  endif()

  target_compile_definitions(test-${edname} PUBLIC
      -DTESTDATA_PATH=${sign_input}
      )
endmacro()

edtest(
    ${EDIMPL}
    ${HASH}
    ${RANDOM}
)
