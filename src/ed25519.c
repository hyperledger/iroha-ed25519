#define STR2(x) #x
#define STR(x) STR2(x)

#if defined(LIBED25519_VERSION)
const char* libed25519_version = STR(LIBED25519_VERSION);
#else
const char* libed25519_version = STR("1.2.0");
#endif
