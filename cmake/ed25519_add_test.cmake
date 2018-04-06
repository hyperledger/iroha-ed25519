function(ed25519_add_test test_name)
  add_executable(${test_name} ${ARGN})
  target_link_libraries(${test_name}
    gtest
    gmock
    )
  target_include_directories(${test_name} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/;${CMAKE_BINARY_DIR}/ed25519;${CMAKE_SOURCE_DIR}/include/>
    )
  add_test(
    NAME    ${test_name}
    COMMAND $<TARGET_FILE:${test_name}>
    )

  set_target_properties(${test_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_PATH      ${CMAKE_BINARY_DIR}/lib/static
    LIBRARY_OUTPUT_PATH      ${CMAKE_BINARY_DIR}/lib
    )
endfunction()
