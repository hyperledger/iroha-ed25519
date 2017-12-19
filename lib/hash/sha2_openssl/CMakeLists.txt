find_package(OpenSSL REQUIRED)

set(LIB sha2_openssl)
add_library(${LIB} OBJECT
    sha256.c
    sha512.c
    )
target_link_libraries(${LIB}
    ${OPENSSL_CRYPTO_LIBRARY}
    )
target_include_directories(${LIB} PUBLIC
    ${OPENSSL_INCLUDE_DIR}
    )

set_target_properties(${LIB} PROPERTIES EXCLUDE_FROM_ALL TRUE)
