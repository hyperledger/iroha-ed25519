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



macro(configure_file_content content file)
  set(CMAKE_CONFIGURABLE_FILE_CONTENT
    "${content}\n")
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/configurable_file_content.in
    ${file}
    @ONLY)
endmacro()


# Get all propreties that cmake supports
execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)

# Convert command output into a CMake list
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")

function(print_properties)
  message ("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
endfunction(print_properties)

function(print_target_properties tgt)
  if(NOT TARGET ${tgt})
    message("There is no target named '${tgt}'")
    return()
  endif()

  foreach (prop ${CMAKE_PROPERTY_LIST})
    string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
    # Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
    if(prop STREQUAL "LOCATION" OR prop MATCHES "^LOCATION_" OR prop MATCHES "_LOCATION$")
      continue()
    endif()
    # message ("Checking ${prop}")
    get_property(propval TARGET ${tgt} PROPERTY ${prop} SET)
    if (propval)
      get_target_property(propval ${tgt} ${prop})
      message ("${tgt} ${prop} = ${propval}")
    endif()
  endforeach(prop)
endfunction(print_target_properties)



function(test_build_amd64 OUT)
  if (UNIX AND CMAKE_SYSTEM_PROCESSOR MATCHES "amd64.*|AMD64.*|x86_64.*")
    set(${OUT} TRUE PARENT_SCOPE)
  else()
    set(${OUT} FALSE PARENT_SCOPE)
  endif()
endfunction()
