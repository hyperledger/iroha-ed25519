# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

set(compiler  "clang-7")
set(flags     "-g -O1")
set(options   "-fsanitize=fuzzer,undefined")

set(CMAKE_C_COMPILER    ${compiler})
set(CMAKE_CXX_COMPILER  ${compiler})
set(CMAKE_C_FLAGS       ${flags})
set(CMAKE_CXX_FLAGS     ${flags})

add_compile_options(${options})
add_link_options(${options})


set(_random ${RANDOM})
foreach(_edimpl ${ED25519_SUPPORTED_LIBRARIES_EDIMPL})
  foreach(_hash ${ED25519_SUPPORTED_LIBRARIES_HASH})
    foreach(_build STATIC SHARED)

      set(_target ${_edimpl}-${_hash}-${_random}-${_build})

      if(NOT TARGET ${edtarget})
        ed25519_merge_libraries(fuzz-ed25519-${_target} ${_build}
          LIBRARIES
          ${_edimpl}
          ${_hash}
          ${_random}
          )
      endif()


      ed25519_add_fuzz(fuzz-sign-${_target}
        fuzz_sign.cc
        )
      ed25519_target_link_libraries(fuzz-sign-${_target}
        fuzz-ed25519-${_target}
        )

    endforeach()
  endforeach()
endforeach()
