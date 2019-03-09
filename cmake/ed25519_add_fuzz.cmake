function(ed25519_add_fuzz fuzz_name)
  add_executable(${fuzz_name} ${ARGN})
  set_target_properties(${fuzz_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/fuzz_bin
    ARCHIVE_OUTPUT_PATH      ${CMAKE_BINARY_DIR}/fuzz_lib
    LIBRARY_OUTPUT_PATH      ${CMAKE_BINARY_DIR}/fuzz_lib
    )
endfunction()
