if(HASH MATCHES openssl OR RANDOM MATCHES openssl)
  hunter_add_package(OpenSSL)
endif()

add_subdirectory(ed25519)
add_subdirectory(randombytes)
add_subdirectory(hash)
