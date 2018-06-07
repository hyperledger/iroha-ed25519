set(_is_supported TRUE)
if(NOT UNIX)
  set(_is_supported FALSE)
endif()

ed25519_add_library(dev_urandom RANDOM
  SOURCES urandom.c
  IS_SUPPORTED ${_is_supported}
  )
