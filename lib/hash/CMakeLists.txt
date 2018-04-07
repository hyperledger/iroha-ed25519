if(HASH STREQUAL sha2_openssl)
  add_subdirectory(sha2_openssl)
elseif(HASH STREQUAL sha3_brainhub)
  add_subdirectory(sha3_brainhub)
else()
  message(FATAL_ERROR "No such hash implementation ${HASH}")
endif()

include(CheckTypeSize)

check_type_size(${SHA_256_CONTEXT_TYPE} SHA_256_CONTEXT_SIZE BUILTIN_TYPES_ONLY)
configure_file(config_sha256.h.in "${CMAKE_SOURCE_DIR}/include/ed25519/ed25519/config_sha256.h")

check_type_size(${SHA_512_CONTEXT_TYPE} SHA_512_CONTEXT_SIZE BUILTIN_TYPES_ONLY)
configure_file(config_sha512.h.in "${CMAKE_SOURCE_DIR}/include/ed25519/ed25519/config_sha512.h")
