set(LIB sha3_brainhub)
add_library(${LIB}
    sha3.c
    sha3_512.c
    )
set_target_properties(${LIB} PROPERTIES EXCLUDE_FROM_ALL TRUE)

setup_ed25519_install_cmake(${LIB} ed25519Config)

