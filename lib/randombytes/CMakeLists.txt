if(OPENSSL_FOUND)
  add_subdirectory(openssl)
else()
  message(STATUS "RANDOM=rand_openssl is not available")
endif()

add_subdirectory(random)
add_subdirectory(urandom)
