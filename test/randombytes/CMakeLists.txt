foreach(_random ${ED25519_SUPPORTED_LIBRARIES_RANDOM})
  set(_target test-${_random})
  ed25519_add_test(${_target}
    random.cpp
    )
  ed25519_target_link_libraries(${_target}
    ${_random}
    )
endforeach()
