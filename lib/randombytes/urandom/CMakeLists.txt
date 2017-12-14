set(LIB dev_urandom)
add_library(${LIB}
    urandom.c
    )
setup_ed25519_install_cmake(${LIB} ed25519Config)
