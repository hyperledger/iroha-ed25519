#include "ed25519/sha512.h"
#include <openssl/sha.h>
#include <stddef.h>

const unsigned int SHA_CONTEXT_SIZE = sizeof(SHA512_CTX);

int sha512_init(void *ctx) {
  /* SHA512_Init returns 1 if succeeded, 0 otherwise */
  /* sha512_init should return 0 if succeded */
  /* other functions are the same */
  return SHA512_Init((SHA512_CTX *)ctx);
}

int sha512_update(void *ctx, const unsigned char *in,
                  unsigned long long inlen) {
  return SHA512_Update((SHA512_CTX *)ctx, in, inlen);
}

int sha512_final(void *ctx, unsigned char *out) {
  return SHA512_Final(out, (SHA512_CTX *)ctx);
}

int sha512(unsigned char *out, const unsigned char *in,
           unsigned long long inlen) {
  // SHA512 returns NULL in case of error
  // so if it returned error, then we must return 1
  // in case of success we return 0
  return SHA512(in, inlen, out) == NULL;
}
