find_package(OpenSSL REQUIRED)

set(LIB rand_openssl)
add_library(${LIB} OBJECT
    randombytes.c
    )
target_link_libraries(${LIB}
    ${OPENSSL_CRYPTO_LIBRARY}
    )
target_include_directories(${LIB} PUBLIC
    ${OPENSSL_INCLUDE_DIR}
    )
