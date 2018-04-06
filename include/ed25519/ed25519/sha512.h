#ifndef PROJECT_SHA512_H_
#define PROJECT_SHA512_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "ed25519_export.h"

#define SHA_512_SIZE 64  // bytes

/* implementation-defined sha context size in bytes. */
extern const unsigned int SHA_512_CONTEXT_SIZE ED25519_EXPORT;

/* context is a block of memory of exactly SHA_512_CONTEXT_SIZE bytes */
/* example: unsigned char context[SHA_512_CONTEXT_SIZE]; */

/**
 * Initializes context with specific for implementation size.
 * @param[in] context always pass preallocated buffer of SHA_512_CONTEXT_SIZE;
 * @return 0 if error, non-0 otherwise.
 * @note some implementations may return bad code sometimes, some may not
 */
extern int sha512_init(void *context) ED25519_EXPORT;

/**
 * Updates hash state with given buffer
 * @param[in] context always pass preallocated buffer of SHA_512_CONTEXT_SIZE;
 * @param[in] in input buffer with info to be hashed
 * @param[in] inlen buffer size
 * @return 0 if error, non-0 otherwise
 * @note some implementations may return bad code sometimes, some may not
 */
extern int sha512_update(void *context, const unsigned char *in,
                         unsigned long long inlen) ED25519_EXPORT;

/**
 * Finish hash calculation. Use this to store hash in output buffer (out).
 * @param[in] context always pass preallocated buffer of SHA_512_CONTEXT_SIZE;
 * @param[in] out output buffer of exactly SHA_512_SIZE bytes
 * @return 0 if error, non-0 otherwise
 * @note some implementations may return bad code sometimes, some may not
 */
extern int sha512_final(void *context, unsigned char *out) ED25519_EXPORT;

/**
 * Inline hash calculation of sha512.
 * @param[out] out output buffer of exactly SHA_512_SIZE bytes
 * @param[in] message message buffer to be hashed
 * @param[in] message_len size of the message buffer
 * @return 0 if error, non-0 otherwise
 * @note some implementations may return bad code sometimes, some may not
 */
extern int sha512(unsigned char *out, const unsigned char *message,
                  unsigned long long message_len) ED25519_EXPORT;

#if defined(__cplusplus)
}
#endif

#endif /* PROJECT_SHA512_H_ */
