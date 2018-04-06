function(ed25519_export)
  set_property (GLOBAL PROPERTY ED25519_EXPORTED "${ARGN}")

  JOIN("${ARGN}" ";\n" _ED25519_EXPORTED)
  configure_file_content("{\nglobal: \n${_ED25519_EXPORTED}; \nlocal: *;\n};\n" ${CMAKE_BINARY_DIR}/linker_exportmap)
endfunction(ed25519_export)
