#ifndef PROJECT_SHA512_H_
#define PROJECT_SHA512_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "ed25519_export.h"
#include "sha_context.h"

#define SHA_512_SIZE 64  // bytes

/**
 * Initializes context with specific for implementation size.
 * @param[in] context
 * @return 0 if error, non-0 otherwise.
 * @note some implementations may return bad code sometimes, some may not
 */
ED25519_EXPORT extern int sha512_init(sha_context *context);

/**
 * Updates hash state with given buffer
 * @param[in] context
 * @param[in] in input buffer with info to be hashed
 * @param[in] inlen buffer size
 * @return 0 if error, non-0 otherwise
 * @note some implementations may return bad code sometimes, some may not
 */
ED25519_EXPORT extern int sha512_update(sha_context *context, const unsigned char *in,
                                        unsigned long long inlen);

/**
 * Finish hash calculation. Use this to store hash in output buffer (out).
 * @param[in] context
 * @param[in] out output buffer of exactly SHA_512_SIZE bytes
 * @return 0 if error, non-0 otherwise
 * @note some implementations may return bad code sometimes, some may not
 */
ED25519_EXPORT extern int sha512_final(sha_context *context, unsigned char *out);

/**
 * Inline hash calculation of sha512.
 * @param[out] out output buffer of exactly SHA_512_SIZE bytes
 * @param[in] message message buffer to be hashed
 * @param[in] message_len size of the message buffer
 * @return 0 if error, non-0 otherwise
 * @note some implementations may return bad code sometimes, some may not
 */
ED25519_EXPORT extern int sha512(unsigned char *out, const unsigned char *message,
                                 unsigned long long message_len);

#if defined(__cplusplus)
}
#endif

#endif /* PROJECT_SHA512_H_ */
