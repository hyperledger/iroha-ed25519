ed25519_add_library(sha3_brainhub HASH
  SOURCES
    sha3.c
    sha3_256.c
    sha3_512.c
  IS_SUPPORTED
    TRUE
  )
set_target_properties(sha3_brainhub PROPERTIES
  ED25519_HASH_TYPE sha3
  )
