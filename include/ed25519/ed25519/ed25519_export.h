// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define ED25519_IMPORT __declspec(dllimport)
#define ED25519_EXPORT __declspec(dllexport)
#define ED25519_LOCAL
#else
#if __GNUC__ >= 4
#define ED25519_IMPORT __attribute__ ((visibility ("default")))
#define ED25519_EXPORT __attribute__ ((visibility ("default")))
#define ED25519_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define ED25519_IMPORT
#define ED25519_EXPORT
#define ED25519_LOCAL
#endif
#endif
