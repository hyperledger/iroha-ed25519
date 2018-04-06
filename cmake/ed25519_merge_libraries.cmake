function(ed25519_merge_libraries TARGET LIBTYPE)
  set(options "")
  set(oneValueArgs "VERSION")
  set(multiValueArgs LIBRARIES COMPILE_OPTIONS)
  cmake_parse_arguments(ARG
    "${options}"
    "${oneValueArgs}"
    "${multiValueArgs}"
    ${ARGN}
    )

  # create dummy C source file
  set(${TARGET}_SRC ${CMAKE_BINARY_DIR}/${TARGET}_dummy.c)
  configure_file_content("const char* libed25519_version = \"${ARG_VERSION}\";" ${${TARGET}_SRC})

  # define target
  add_library(${TARGET} ${LIBTYPE}
    ${${TARGET}_SRC}
    )
  ed25519_target_link_libraries(${TARGET}
    ${ARG_LIBRARIES}
    )
  target_compile_options(${TARGET} PUBLIC
    ${ARG_COMPILE_OPTIONS}
    )
  target_include_directories(${TARGET}
    INTERFACE
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/;${CMAKE_SOURCE_DIR}/include/>
    )

  if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # linux only solution
    ed25519_target_link_libraries(${TARGET}
      "-Wl,--version-script=${CMAKE_BINARY_DIR}/linker_exportmap"
      )
  endif()

  # check that every static library has PIC enabled
  foreach(lib ${ARG_LIBRARIES})
    if(TARGET ${lib})
      add_dependencies(${TARGET} ${lib})
      get_target_property(LIB_TYPE ${lib} TYPE)
      if(LIB_TYPE STREQUAL "STATIC_LIBRARY")
        get_target_property(PIC ${lib} POSITION_INDEPENDENT_CODE)
        if(NOT PIC)
          message(FATAL_ERROR
            "Attempted to link non-PIC static library ${LIB} to shared library ${TARGET}\n"
            "Please, use ed25519_add_library"
            )
        endif()
      endif()
    endif()
  endforeach()

  set_target_properties(${TARGET} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()
