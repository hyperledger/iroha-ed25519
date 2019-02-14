#include <ed25519/ed25519/sha512.h>
#include <openssl/sha.h>
#include <stddef.h>

ED25519_EXPORT int sha512_init(sha_context *ctx) {
  /* SHA512_Init returns 1 if succeeded, 0 otherwise */
  return SHA512_Init((SHA512_CTX *) ctx);
}

ED25519_EXPORT int sha512_update(sha_context *ctx, const unsigned char *in,
                  unsigned long long inlen) {
  return SHA512_Update((SHA512_CTX *) ctx, in, inlen);
}

ED25519_EXPORT int sha512_final(sha_context *ctx, unsigned char *out) {
  return SHA512_Final(out, (SHA512_CTX *) ctx);
}

ED25519_EXPORT int sha512(unsigned char *out, const unsigned char *in,
           unsigned long long inlen) {
  // SHA512 returns NULL in case of error
  return SHA512(in, inlen, out) != NULL;
}
