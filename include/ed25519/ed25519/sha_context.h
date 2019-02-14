#ifndef PROJECT_SHA_CONTEXT_H_
#define PROJECT_SHA_CONTEXT_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "ed25519_export.h"

typedef struct sha_context_t {
  unsigned char opaque[224]; // size of context in bytes
} sha_context;

#if defined(__cplusplus)
}
#endif

#endif  //  PROJECT_SHA_CONTEXT_H_
