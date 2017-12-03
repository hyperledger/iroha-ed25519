#ifndef PROJECT_SHA512_H_
#define PROJECT_SHA512_H_

#if defined(__cplusplus)
extern "C" {
#endif

#define SHA_512_SIZE 64  // bytes

/* implementation-defined sha context size in bytes. */
extern const unsigned int SHA_CONTEXT_SIZE;

/* context is a block of memory of exactly SHA_CONTEXT_SIZE bytes */
/* example: unsigned char context[SHA_CONTEXT_SIZE]; */

/**
 * Initializes context with specific for implementation size.
 * @param context always pass preallocated buffer of SHA_CONTEXT_SIZE;
 * @return 0 if success, non-0 otherwise.
 */
extern int sha512_init(void *context);

/**
 * Updates hash state with given buffer
 * @param context always pass preallocated buffer of SHA_CONTEXT_SIZE;
 * @param in input buffer with info to be hashed
 * @param inlen buffer size
 * @return 0 if success, non-0 otherwise
 */
extern int sha512_update(void *context, const unsigned char *in,
                         unsigned long long inlen);

/**
 * Finish hash calculation. Use this to store hash in output buffer (out).
 * @param context always pass preallocated buffer of SHA_CONTEXT_SIZE;
 * @param out output buffer of exactly SHA_512_SIZE bytes
 * @return 0 if success, non-0 otherwise
 */
extern int sha512_final(void *context, unsigned char *out);

/**
 * Inline hash calculation of sha512.
 * @param out output buffer of exactly SHA_512_SIZE bytes
 * @param message message buffer to be hashed
 * @param message_len size of the message buffer
 * @return 0 if success, non-0 otherwise
 */
extern int sha512(unsigned char *out, const unsigned char *message,
                  unsigned long long message_len);

#if defined(__cplusplus)
}
#endif

#endif /* PROJECT_SHA512_H_ */
