ed25519_add_library(rand_openssl
  SOURCES
    randombytes.c
  LINK_LIBRARIES
    ${OPENSSL_CRYPTO_LIBRARY}
  INCLUDES
    ${OPENSSL_INCLUDE_DIR}
  )
