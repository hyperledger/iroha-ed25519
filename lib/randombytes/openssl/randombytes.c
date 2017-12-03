#include <openssl/rand.h>

int randombytes(unsigned char *p, int len) {
  return RAND_bytes(p, len);
}
