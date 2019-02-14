set(_random dev_urandom)
foreach(_edimpl ${ED25519_SUPPORTED_LIBRARIES_EDIMPL})
  foreach(_hash ${ED25519_SUPPORTED_LIBRARIES_HASH})
    foreach(_build STATIC SHARED)

      if(_build MATCHES SHARED AND _edimpl MATCHES amd64-64-24k)
        # these builds are incompatible and will fail in any circumstance
        continue()
      endif()

      set(_target ${_edimpl}-${_hash}-${_random}-${_build})
      ed25519_merge_libraries(ed25519-${_target} ${_build}
        LIBRARIES
          ${_edimpl}
          ${_hash}
          ${_random}
        )
      ed25519_add_test(test-${_target}
        ed25519_test.cpp
        )
      ed25519_target_link_libraries(test-${_target}
        ed25519-${_target}
        )

      if(TARGET ${_hash})
        get_target_property(_type "${_hash}" ED25519_HASH_TYPE)
        target_compile_definitions(test-${_target} PRIVATE
          -DTESTDATA_PATH=${CMAKE_CURRENT_SOURCE_DIR}/sign.input.with.${_type}.txt
          )
      endif()
    endforeach()
  endforeach()
endforeach()
