set(is_supported FALSE)
if(WIN32 OR MSVC OR MSYS OR MINGW)
  find_library(BCRYPT_LIB bcrypt)
  if(BCRYPT_LIB)
    set(is_supported TRUE)
    message(STATUS "Found bcrypt library: ${BCRYPT_LIB}")
  endif()
endif()

ed25519_add_library(bcryptgen RANDOM
  SOURCES
    randombytes.c
  LINK_LIBRARIES
    ${BCRYPT_LIB}
  IS_SUPPORTED
    ${is_supported}
  )
