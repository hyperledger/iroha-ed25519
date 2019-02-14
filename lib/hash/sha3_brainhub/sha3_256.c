#include <ed25519/ed25519/errcode.h>
#include <ed25519/ed25519/sha256.h>
#include "sha3.h"

int sha256_init(sha_context *ctx) {
  sha3_Init256((sha3_context *) ctx);
  return ED25519_SUCCESS;
}

int sha256_update(sha_context *ctx, const unsigned char *in,
                  unsigned long long inlen) {
  sha3_Update((sha3_context *) ctx, in, inlen);
  return ED25519_SUCCESS;
}

int sha256_final(sha_context *ctx, unsigned char *out) {
  sha3_Finalize((sha3_context *) ctx, out);
  return ED25519_SUCCESS;
}

int sha256(unsigned char *out, const unsigned char *message,
           unsigned long long message_len) {
  sha_context ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, message, message_len);
  sha256_final(&ctx, out);
  return ED25519_SUCCESS;
}
