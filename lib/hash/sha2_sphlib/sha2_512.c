#include "ed25519/ed25519/sha512.h"
#include "ed25519/ed25519/errcode.h"

#include "sph_sha2.h"

ED25519_EXPORT inline int sha512_init(sha_context *context) {
  sph_sha512_init((void *) context);
  return ED25519_SUCCESS;
}

ED25519_EXPORT inline int sha512_update(sha_context *context, const unsigned char *in,
                                 unsigned long long inlen) {
  sph_sha512((void *) context, in, inlen);
  return ED25519_SUCCESS;
}

ED25519_EXPORT inline int sha512_final(sha_context *context, unsigned char *out) {
  sph_sha512_close((void *) context, out);
  return ED25519_SUCCESS;
}

ED25519_EXPORT inline int sha512(unsigned char *out, const unsigned char *message,
                          unsigned long long message_len) {
  sha_context context;
  sph_sha512_init((void *) &context);
  sph_sha512((void *) &context, message, message_len);
  sph_sha512_close((void *) &context, out);

  return ED25519_SUCCESS;
}
