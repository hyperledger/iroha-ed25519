function(addtest test_name)
  set(SOURCES ${ARGN})
  add_executable(${test_name} ${SOURCES})
  target_link_libraries(${test_name} gtest gmock)
  add_test(
      NAME ${test_name}
      COMMAND $<TARGET_FILE:${test_name}>
  )
endfunction()

# Creates benchmark "bench_name", with "SOURCES" (use string as second argument)
function(addbenchmark bench_name SOURCES)
  add_executable(${bench_name} ${SOURCES})
  target_link_libraries(${bench_name} PRIVATE benchmark)
  strictmode(${bench_name})
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

#  message(STATUS "${variable}=${opts_pretty}")

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


function(getplatform out)
 message(STATUS ${CMAKE_SYSTEM_PROCESSOR})
endfunction()
