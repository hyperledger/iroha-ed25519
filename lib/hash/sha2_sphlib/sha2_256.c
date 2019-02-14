#include "ed25519/ed25519/sha256.h"
#include "ed25519/ed25519/errcode.h"

#include "sph_sha2.h"

ED25519_EXPORT int sha256_init(sha_context *context) {
  sph_sha256_init((void *) context);
  return ED25519_SUCCESS;
}

ED25519_EXPORT int sha256_update(sha_context *context, const unsigned char *in,
                                 unsigned long long inlen) {
  sph_sha256((void *) context, in, inlen);
  return ED25519_SUCCESS;
}

ED25519_EXPORT int sha256_final(sha_context *context, unsigned char *out) {
  sph_sha256_close((void *) context, out);
  return ED25519_SUCCESS;
}

ED25519_EXPORT int sha256(unsigned char *out, const unsigned char *message,
                          unsigned long long message_len) {
  sha_context context;
  sph_sha256_init((void *) &context);
  sph_sha256((void *) &context, message, message_len);
  sph_sha256_close((void *) &context, out);

  return ED25519_SUCCESS;
}
