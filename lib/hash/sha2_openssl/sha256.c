#include <ed25519/ed25519/sha256.h>
#include <openssl/sha.h>
#include <stddef.h>

const unsigned int SHA_256_CONTEXT_SIZE = sizeof(SHA256_CTX);

int sha256_init(void *ctx) {
  /* SHA256_Init returns 1 if succeeded, 0 otherwise */
  return SHA256_Init((SHA256_CTX *)ctx);
}

int sha256_update(void *ctx, const unsigned char *in,
                  unsigned long long inlen) {
  return SHA256_Update((SHA256_CTX *)ctx, in, inlen);
}

int sha256_final(void *ctx, unsigned char *out) {
  return SHA256_Final(out, (SHA256_CTX *)ctx);
}

int sha256(unsigned char *out, const unsigned char *in,
           unsigned long long inlen) {
  // SHA256 returns NULL in case of error
  return SHA256(in, inlen, out) != NULL;
}
