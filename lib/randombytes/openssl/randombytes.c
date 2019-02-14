#include <openssl/rand.h>
#include <ed25519/ed25519/randombytes.h>

ED25519_EXPORT int randombytes(unsigned char *p, int len) {
  return RAND_bytes(p, len);
}
