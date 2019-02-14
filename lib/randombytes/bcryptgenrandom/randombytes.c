#include <ed25519/ed25519/randombytes.h>
#include <ed25519/ed25519/errcode.h>
#include <windows.h>
#include <bcrypt.h>
#pragma comment(lib, "bcrypt.lib")

ED25519_EXPORT int randombytes(unsigned char *p, int len) {
  return BCRYPT_SUCCESS(
      BCryptGenRandom(NULL, p, len, BCRYPT_USE_SYSTEM_PREFERRED_RNG));
}
