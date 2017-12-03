find_package(OpenSSL REQUIRED)

add_library(rand_openssl
    randombytes.c
    )
target_link_libraries(rand_openssl
    ${OPENSSL_CRYPTO_LIBRARY}
    )
target_include_directories(rand_openssl PUBLIC
    ${OPENSSL_INCLUDE_DIR}
    )
