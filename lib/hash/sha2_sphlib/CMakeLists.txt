ed25519_add_library(sha2_sphlib HASH
  SOURCES
  sha2.c
  sha2_big.c
  sha2_256.c
  sha2_512.c
  IS_SUPPORTED TRUE
  )
set_target_properties(sha2_sphlib PROPERTIES
  ED25519_HASH_TYPE sha2
  )
