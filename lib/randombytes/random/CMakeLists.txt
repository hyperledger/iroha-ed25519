ed25519_add_library(dev_random RANDOM
  SOURCES random.c
  IS_SUPPORTED ${UNIX}
  )
