find_package(OpenSSL)

ed25519_add_library(rand_openssl RANDOM
  SOURCES
    randombytes.c
  LINK_LIBRARIES
    ${OPENSSL_CRYPTO_LIBRARY}
  INCLUDES
    ${OPENSSL_INCLUDE_DIR}
  IS_SUPPORTED
    ${OPENSSL_FOUND}
  )
