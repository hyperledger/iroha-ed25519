# ed25519_select_supported_impl works as a priority queue, it will return first known and supported library from ARGN
function(ed25519_select_supported_impl SELECTED)
  set(TYPE ${SELECTED})
  if(NOT ED25519_SUPPORTED_LIBRARIES_${TYPE} OR NOT ED25519_KNOWN_LIBRARIES_${TYPE})
    message(FATAL_ERROR "
      ed25519_select_supported_impl is called before all libraries
      ED25519_KNOWN_LIBRARIES_${TYPE}: '${ED25519_KNOWN_LIBRARIES_${TYPE}}'
      ED25519_SUPPORTED_LIBRARIES_${TYPE}: '${ED25519_SUPPORTED_LIBRARIES_${TYPE}}'
    ")
  endif()

  foreach(lib ${ARGN})
    if(${lib} IN_LIST ED25519_KNOWN_LIBRARIES_${TYPE})
      if(${lib} IN_LIST ED25519_SUPPORTED_LIBRARIES_${TYPE})
        set(${SELECTED} ${lib} PARENT_SCOPE)
        return()
      endif()
    else()
      message(FATAL_ERROR "${lib} is NOT FOUND in ED25519_KNOWN_LIBRARIES_${TYPE}")
    endif()
  endforeach()

  message(FATAL_ERROR "
    Did not find suitable implementation.
    ED25519_KNOWN_LIBRARIES_${TYPE}: '${ED25519_KNOWN_LIBRARIES_${TYPE}}'
    ED25519_SUPPORTED_LIBRARIES_${TYPE}: '${ED25519_SUPPORTED_LIBRARIES_${TYPE}}'
  ")

endfunction()
