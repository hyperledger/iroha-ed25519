find_package(OpenSSL REQUIRED)

ed25519_add_library(sha2_openssl HASH
  SOURCES
    sha256.c
    sha512.c
  LINK_LIBRARIES
    ${OPENSSL_CRYPTO_LIBRARY}
  INCLUDES
    ${OPENSSL_INCLUDE_DIR}
  IS_SUPPORTED
    ${OPENSSL_FOUND}
  )

if(OPENSSL_FOUND)
  set_target_properties(sha2_openssl PROPERTIES
    ED25519_HASH_TYPE sha2
    )
endif()
