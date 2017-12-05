find_package(OpenSSL REQUIRED)

add_library(sha2_openssl
    sha512.c
    )
target_link_libraries(sha2_openssl
    ${OPENSSL_CRYPTO_LIBRARY}
    )
target_include_directories(sha2_openssl PUBLIC
    ${OPENSSL_INCLUDE_DIR}
    )

set_target_properties(sha2_openssl PROPERTIES EXCLUDE_FROM_ALL TRUE)
