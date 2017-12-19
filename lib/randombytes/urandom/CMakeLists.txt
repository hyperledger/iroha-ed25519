set(LIB dev_urandom)
add_library(${LIB} OBJECT
    urandom.c
    )
