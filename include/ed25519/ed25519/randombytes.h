#ifndef PROJECT_RANDOMBYTES_H_
#define PROJECT_RANDOMBYTES_H_

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Fills preallocated buffer p of length len with random data.
 * @param[out] p buffer of length len
 * @param[in] len length
 * @return 1 if success, non-0 otherwise
 * @note You should always check return code of randombytes
 */
extern int randombytes(unsigned char *p, int len);

#if defined(__cplusplus)
}
#endif

#endif //  PROJECT_RANDOMBYTES_H_
