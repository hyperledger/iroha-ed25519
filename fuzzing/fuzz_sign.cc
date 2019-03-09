/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include <stdlib.h>
#include <stdint.h>
#include <ed25519/ed25519.h>

static public_key_t pub;
static private_key_t priv;
static signature_t sig;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  ed25519_sign(&sig, Data, Size, &pub, &priv);
  return 0;  // Non-zero return values are reserved for future use.
}
