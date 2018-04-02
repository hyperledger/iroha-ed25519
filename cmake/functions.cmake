function(addtest test_name)
  set(SOURCES ${ARGN})
  add_executable(${test_name} ${SOURCES})
  target_link_libraries(${test_name} PUBLIC gtest gmock)
  add_test(
      NAME ${test_name}
      COMMAND $<TARGET_FILE:${test_name}>
  )

  set_target_properties(${test_name} PROPERTIES
    EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/lib/static
    LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/lib
    )
endfunction()

# Creates benchmark "bench_name"
function(addbenchmark bench_name)
  add_executable(${bench_name} ${ARGN})
  target_link_libraries(${bench_name} PRIVATE benchmark)

  set_target_properties(${bench_name} PROPERTIES
    EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/lib/static
    LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/lib
    )
endfunction()


function(JOIN VALUES GLUE OUTPUT)
  string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()


# ENUM(ALGO "${ALGO}" "A" "B" "C")
# stands for: create ENUM with name ALGO, which can be A, B or C
function(ENUM variable check description)
  set(options ${ARGN})
  JOIN("${options}" "|" opts_pretty)
  list(INSERT opts_pretty 0 "[")
  list(APPEND opts_pretty "]")
  JOIN("${opts_pretty}" "" opts_pretty)

  set_property (GLOBAL PROPERTY ENUM_${variable}_OPTIONS "${options}")

  # get the length of options
  list(LENGTH options options_len)
  if(options_len LESS 1)
    message(FATAL_ERROR "Specify at least 1 option in ENUM")
  else()
    # if variable is empty
    if(NOT check)
      message(FATAL_ERROR "Specify -D${variable} with one of ${opts_pretty}")
    endif()

    # check if provided option is one of given possible options
    set(SUCCESS FALSE)
    foreach(opt ${options})
      if(opt STREQUAL ${check})
        set(SUCCESS TRUE)
      endif()
    endforeach()

    if(SUCCESS)
      message(STATUS "${variable}=${check} is selected (${description})")
    else()
      message(FATAL_ERROR "${variable}=${check} is not one of ${opts_pretty}")
    endif()
  endif()
endfunction()



macro(find_substring string substring out)
  string(FIND ${string} ${substring} RESULT)
  if(${RESULT} EQUAL -1)
    set(${out} FALSE)
  else()
    set(${out} TRUE)
  endif()
endmacro()


function(gethash target out)
  string(TOUPPER ${target} HASHUPPER)
  find_substring(${HASHUPPER} "SHA2" ISSHA2)
  find_substring(${HASHUPPER} "SHA3" ISSHA3)

  if(ISSHA2)
    set(${out} "SHA2" PARENT_SCOPE)
  elseif(ISSHA3)
    set(${out} "SHA3" PARENT_SCOPE)
  else()
    message(FATAL_ERROR "${target} does not contain sha2/sha3 in name. Can't determine test set.")
  endif()
endfunction()

function(ed25519_add_library LIBNAME)
  set(options "")
  set(oneValueArgs "")
  set(multiValueArgs SOURCES INCLUDES LINK_LIBRARIES COMPILE_DEFINITIONS)
  cmake_parse_arguments(LIB_${LIBNAME} "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # for every sourcefile, replace relative path with absolute path to source file
  foreach(file ${LIB_${LIBNAME}_SOURCES})
    get_filename_component(abspath ${file} REALPATH)
    list(APPEND    LIB_${LIBNAME}_SOURCES ${abspath})
    list(REMOVE_AT LIB_${LIBNAME}_SOURCES 0)
  endforeach()

  foreach(j ${multiValueArgs})
    set_property (GLOBAL PROPERTY LIB_${LIBNAME}_${j} "${LIB_${LIBNAME}_${j}}")
  endforeach()

  add_library(${LIBNAME} STATIC
    ${LIB_${LIBNAME}_SOURCES}
    )
  target_link_libraries(${LIBNAME} PUBLIC
    ${LIB_${LIBNAME}_LINK_LIBRARIES}
    )
  target_compile_definitions(${LIBNAME} PUBLIC
    "${LIB_${LIBNAME}_COMPILE_DEFINITIONS}"
    )
  target_include_directories(${LIBNAME} PUBLIC
    ${LIB_${LIBNAME}_INCLUDES}
    )
  set_target_properties(${LIBNAME} PROPERTIES
    EXCLUDE_FROM_ALL       TRUE
    EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_PATH    ${CMAKE_BINARY_DIR}
    LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}
    )
endfunction(ed25519_add_library)


function(test_build_amd64 OUT)
  if (UNIX AND CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|AMD64.*|x86_64.*")
    set(${OUT} TRUE PARENT_SCOPE)
  else()
    set(${OUT} FALSE PARENT_SCOPE)
  endif()
endfunction()
