ed25519_add_library(rand_bcryptgenrandom RANDOM
  SOURCES
    randombytes.c
  IS_SUPPORTED
    ${WIN32}
  )
