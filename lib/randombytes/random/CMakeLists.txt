set(LIB dev_random)
add_library(${LIB}
    random.c
    )

setup_ed25519_install_cmake(${LIB} ed25519Config)
