find_package(OpenSSL)
if(OPENSSL_FOUND)
  add_subdirectory(sha2_openssl)
else()
  message(STATUS "sha2_openssl target is not available")
endif()

add_subdirectory(sha3_brainhub)
