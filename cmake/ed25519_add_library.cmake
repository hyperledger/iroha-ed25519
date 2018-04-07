function(ed25519_add_library LIBNAME TYPE)
  set(options "")
  set(oneValueArgs "IS_SUPPORTED")
  set(multiValueArgs SOURCES INCLUDES LINK_LIBRARIES COMPILE_OPTIONS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(ARG_IS_SUPPORTED STREQUAL "")
    message(FATAL_ERROR "Specify IS_SUPPORTED:BOOL for ed25519_add_library")
  endif()

  get_property (ED25519_TYPES GLOBAL PROPERTY ED25519_TYPES)
  if (NOT TYPE IN_LIST ED25519_TYPES)
    message(FATAL_ERROR "Invalid TYPE=${TYPE} for ${LIBNAME}. Not one of ${ED25519_TYPES}")
  else()
    # store current library in KNOWN libraries set
    if(NOT LIBNAME IN_LIST ED25519_KNOWN_LIBRARIES_${TYPE})
      set(ED25519_KNOWN_LIBRARIES_${TYPE}
        ${ED25519_KNOWN_LIBRARIES_${TYPE}} ${LIBNAME}
        CACHE INTERNAL "" FORCE)
    endif()

    # store current library in SUPPORTED libraries set, if it is supported
    if(ARG_IS_SUPPORTED AND NOT LIBNAME IN_LIST ED25519_SUPPORTED_LIBRARIES_${TYPE})
      set(ED25519_SUPPORTED_LIBRARIES_${TYPE}
        ${ED25519_SUPPORTED_LIBRARIES_${TYPE}} ${LIBNAME}
        CACHE INTERNAL "" FORCE)
    endif()
  endif()

  # for every sourcefile, replace relative path with absolute path to source file
  foreach(file ${ARG_SOURCES})
    get_filename_component(abspath ${file} REALPATH)
    list(APPEND    ARG_SOURCES ${abspath})
    list(REMOVE_AT ARG_SOURCES 0)
  endforeach()

  if(ARG_IS_SUPPORTED)
    add_library(${LIBNAME} OBJECT
      ${ARG_SOURCES}
      )
    ed25519_target_link_libraries(${LIBNAME}
      ${ARG_LINK_LIBRARIES}
      )
    target_compile_options(${LIBNAME} PUBLIC
      ${ARG_COMPILE_OPTIONS}
      -Ded25519_EXPORTS
      )
    target_include_directories(${LIBNAME}
      PUBLIC
        ${ARG_INCLUDES}
        $<INSTALL_INTERFACE:include>
      PRIVATE
        ${CMAKE_BINARY_DIR}
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/;${CMAKE_SOURCE_DIR}/include/>
      )
    set_target_properties(${LIBNAME} PROPERTIES
      EXCLUDE_FROM_ALL          ON
      RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin
      ARCHIVE_OUTPUT_PATH       ${CMAKE_BINARY_DIR}/archive_output_directory
      LIBRARY_OUTPUT_PATH       ${CMAKE_BINARY_DIR}/library_output_directory
      POSITION_INDEPENDENT_CODE ON
      )

  else()
    message(STATUS "[ed25519] Target ${TYPE}=${LIBNAME} is not supported on your platform")
  endif()
endfunction(ed25519_add_library)
