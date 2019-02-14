#include <ed25519/ed25519/errcode.h>
#include <ed25519/ed25519/sha512.h>
#include "sha3.h"

ED25519_EXPORT int sha512_init(sha_context *ctx) {
  sha3_Init512((sha3_context *) ctx);
  return ED25519_SUCCESS;
}

ED25519_EXPORT int sha512_update(sha_context *ctx, const unsigned char *in,
                  unsigned long long inlen) {
  sha3_Update((sha3_context *) ctx, in, inlen);
  return ED25519_SUCCESS;
}

ED25519_EXPORT int sha512_final(sha_context *ctx, unsigned char *out) {
  sha3_Finalize((sha3_context *) ctx, out);
  return ED25519_SUCCESS;
}

ED25519_EXPORT int sha512(unsigned char *out, const unsigned char *message,
           unsigned long long message_len) {
  sha_context ctx;
  sha512_init(&ctx);
  sha512_update(&ctx, message, message_len);
  sha512_final(&ctx, out);
  return ED25519_SUCCESS;
}
