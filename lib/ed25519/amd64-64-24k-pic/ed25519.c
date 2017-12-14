#include <ed25519/ed25519.h>
#include <assert.h>
#include <string.h>
#include "ge25519.h"
#include <ed25519/ed25519/crypto_verify.h>

int ed25519_create_keypair(private_key_t *sk, public_key_t *pk) {
  if (!randombytes(sk->data, ed25519_privkey_SIZE))
    return ED25519_ERROR;            /* RNG failed, not enough entropy */
  ed25519_derive_public_key(sk, pk); /* fill with data */
  return ED25519_SUCCESS;            /* ok */
}

void ed25519_derive_public_key(const private_key_t *sk, public_key_t *pk) {
  unsigned char az[64];
  sc25519 scsk;
  ge25519 gepk;

  sha512(az, sk->data, 32);
  az[0] &= 248;
  az[31] &= 127;
  az[31] |= 64;

  sc25519_from32bytes(&scsk, az);

  ge25519_scalarmult_base(&gepk, &scsk);
  ge25519_pack(pk->data, &gepk);
}

void ed25519_sign(signature_t *sig, const unsigned char *msg,
                  unsigned long long msglen, const public_key_t *pk,
                  const private_key_t *sk) {
  unsigned char context[SHA_512_CONTEXT_SIZE];
  unsigned char az[64];
  unsigned char nonce[64];  // r
  unsigned char hram[64];
  sc25519 sck, scs, scsk;
  ge25519 ger;

  sha512_init((void *)context);
  sha512_update((void *)context, sk->data, ed25519_privkey_SIZE);
  sha512_final((void *)context, az);
  az[0] &= 248;
  az[31] &= 127;
  az[31] |= 64;
  /* az: 64-byte H(sk) */
  /* az: 32-byte scalar a, 32-byte randomizer z */

  sha512_init((void *)context);
  sha512_update((void *)context, /* z */ az + 32, 32);
  sha512_update((void *)context, msg, msglen);
  sha512_final((void *)context, nonce);
  /* nonce: 64-byte H(z,msg) */

  sc25519_from64bytes(&sck, nonce);
  ge25519_scalarmult_base(&ger, &sck);
  ge25519_pack(sig->data, &ger);
  /* sig: [32 bytes R | 32 bytes uninit] */

  sha512_init((void *)context);
  // first 32 bytes of signature
  sha512_update((void *)context, /* R */ sig->data, 32);
  sha512_update((void *)context, /* A */ pk->data, ed25519_pubkey_SIZE);
  sha512_update((void *)context, msg, msglen);
  sha512_final((void *)context, hram);
  /* hram: 64-byte H(R,A,m) */

  sc25519_from64bytes(&scs, hram);
  sc25519_from32bytes(&scsk, /* a */ az);
  sc25519_mul(&scs, &scs, &scsk);
  sc25519_add(&scs, &scs, &sck);
  /* scs: S = nonce + H(R,A,m)a */

  sc25519_to32bytes(sig->data + 32, &scs);
  /* sig: [32 bytes R | 32 bytes S] */
}

int ed25519_verify(const signature_t *sig, const unsigned char *msg,
                   unsigned long long msglen, const public_key_t *pk) {
  unsigned char context[SHA_512_CONTEXT_SIZE];
  unsigned char pkcopy[32];
  unsigned char rcopy[32];
  unsigned char hram[64];
  unsigned char rcheck[32];
  ge25519 get1, get2;
  sc25519 schram, scs;

  if (sig->data[63] & 224) goto badsig;
  if (ge25519_unpackneg_vartime(&get1, pk->data)) goto badsig;

  memcpy(pkcopy, pk->data, ed25519_pubkey_SIZE);
  memcpy(rcopy, /* R, first 32 bytes */ sig->data, 32);

  sc25519_from32bytes(&scs, /* S, seconds 32 bytes */ sig->data + 32);

  sha512_init((void *)context);
  // first 32 bytes of signature
  sha512_update((void *)context, /* R */ sig->data, 32);
  sha512_update((void *)context, /* A */ pk->data, ed25519_pubkey_SIZE);
  sha512_update((void *)context, msg, msglen);
  sha512_final((void *)context, hram);
  /* scs: S = nonce + H(R,A,m)a */

  sc25519_from64bytes(&schram, hram);

  ge25519_double_scalarmult_vartime(&get2, &get1, &schram, &scs);
  ge25519_pack(rcheck, &get2);

  if (crypto_verify_32(rcopy, rcheck) == 0) {
    return ED25519_SIGNATURE_VALID;
  }

badsig:
  return ED25519_SIGNATURE_INVALID;
}
