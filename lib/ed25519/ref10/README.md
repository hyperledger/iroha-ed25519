# ref10
```bash
$ diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10 tools/ed25519-sha3/lib/ed25519/ref10
```

```diff
Only in tools/ed25519-sha3/lib/ed25519/ref10/: CMakeLists.txt
Only in tools/ed25519-sha3/lib/ed25519/ref10/: ed25519.c
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/fe_frombytes.c tools/ed25519-sha3/lib/ed25519/ref10/fe_frombytes.c
2,3c2
< #include "crypto_int64.h"
< #include "crypto_uint64.h"
---
> #include "helpers/crypto_types.h"
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/fe.h tools/ed25519-sha3/lib/ed25519/ref10/fe.h
4c4
< #include "crypto_int32.h"
---
> #include "helpers/crypto_types.h"
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/fe_isnonzero.c tools/ed25519-sha3/lib/ed25519/ref10/fe_isnonzero.c
2c2
< #include "crypto_verify_32.h"
---
> #include "helpers/crypto_verify.h"
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/fe_mul.c tools/ed25519-sha3/lib/ed25519/ref10/fe_mul.c
2c2
< #include "crypto_int64.h"
---
> #include "helpers/crypto_types.h"
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/fe_sq2.c tools/ed25519-sha3/lib/ed25519/ref10/fe_sq2.c
2c2
< #include "crypto_int64.h"
---
> #include "helpers/crypto_types.h"
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/fe_sq.c tools/ed25519-sha3/lib/ed25519/ref10/fe_sq.c
2c2
< #include "crypto_int64.h"
---
> #include "helpers/crypto_types.h"
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/ge_scalarmult_base.c tools/ed25519-sha3/lib/ed25519/ref10/ge_scalarmult_base.c
2c2
< #include "crypto_uint32.h"
---
> #include "helpers/crypto_types.h"
Only in tools/ed25519-sha3/lib/ed25519/ref10/: __init__.py
Only in tools/supercop-20171020/crypto_sign/ed25519/ref10: keypair.c
Only in tools/supercop-20171020/crypto_sign/ed25519/ref10: open.c
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/sc_muladd.c tools/ed25519-sha3/lib/ed25519/ref10/sc_muladd.c
2,4c2
< #include "crypto_int64.h"
< #include "crypto_uint32.h"
< #include "crypto_uint64.h"
---
> #include "helpers/crypto_types.h"
diff -r tools/supercop-20171020/crypto_sign/ed25519/ref10/sc_reduce.c tools/ed25519-sha3/lib/ed25519/ref10/sc_reduce.c
2,4c2
< #include "crypto_int64.h"
< #include "crypto_uint32.h"
< #include "crypto_uint64.h"
---
> #include "helpers/crypto_types.h"
Only in tools/supercop-20171020/crypto_sign/ed25519/ref10: sign.c
```

## open.c, keypair.c, sign.c

Now all these files are inside `ed25519.c`.

**keypair.c**
```diff
1,2c1,8
< int crypto_sign_keypair(unsigned char *pk,unsigned char *sk)
< {
---
> int ed25519_create_keypair(private_key_t *sk, public_key_t *pk) {
>   if (!randombytes(sk->data, ed25519_privkey_SIZE))
>     return ED25519_ERROR;            /* RNG failed, not enough entropy */
>   ed25519_derive_public_key(sk, pk); /* fill with data */
>   return ED25519_SUCCESS;            /* ok */
> }
> 
> void ed25519_derive_public_key(const private_key_t *sk, public_key_t *pk) {
6,7c12
<   randombytes(sk,32);
<   crypto_hash_sha512(az,sk,32);
---
>   sha512(az, sk->data, 32);
12,17c17,19
<   ge_scalarmult_base(&A,az);
<   ge_p3_tobytes(pk,&A);
< 
<   memmove(sk + 32,pk,32);
<   return 0;
< }
\ No newline at end of file
---
>   ge_scalarmult_base(&A, az);
>   ge_p3_tobytes(pk->data, &A);
> }
```

**open.c**:
```diff
1,6c1,3
< int crypto_sign_open(
<   unsigned char *m,unsigned long long *mlen,
<   const unsigned char *sm,unsigned long long smlen,
<   const unsigned char *pk
< )
< {
---
> t ed25519_verify(const signature_t *sig, const unsigned char *msg,
>                    unsigned long long msglen, const public_key_t *pk) {
>   unsigned char context[SHA_CONTEXT_SIZE];
9,10c6
<   unsigned char scopy[32];
<   unsigned char h[64];
---
>   unsigned char hram[64];
15,34c11,30
<   if (smlen < 64) goto badsig;
<   if (sm[63] & 224) goto badsig;
<   if (ge_frombytes_negate_vartime(&A,pk) != 0) goto badsig;
< 
<   memmove(pkcopy,pk,32);
<   memmove(rcopy,sm,32);
<   memmove(scopy,sm + 32,32);
< 
<   memmove(m,sm,smlen);
<   memmove(m + 32,pkcopy,32);
<   crypto_hash_sha512(h,m,smlen);
<   sc_reduce(h);
< 
<   ge_double_scalarmult_vartime(&R,h,&A,scopy);
<   ge_tobytes(rcheck,&R);
<   if (crypto_verify_32(rcheck,rcopy) == 0) {
<     memmove(m,m + 64,smlen - 64);
<     memset(m + smlen - 64,0,64);
<     *mlen = smlen - 64;
<     return 0;
---
>   if (sig->data[63] & 224) goto badsig;
>   if (ge_frombytes_negate_vartime(&A, pk->data) != 0) goto badsig;
> 
>   memcpy(pkcopy, pk->data, 32);
>   memcpy(rcopy, /* R, first 32 bytes */ sig->data, 32);
> 
>   sha512_init((void *)context);
>   // first 32 bytes of signature
>   sha512_update((void *)context, /* R */ sig->data, 32);
>   sha512_update((void *)context, /* A */ pk->data, ed25519_pubkey_SIZE);
>   sha512_update((void *)context, msg, msglen);
>   sha512_final((void *)context, hram);
>   /* scs: S = nonce + H(R,A,m)a */
> 
>   sc_reduce(hram);
>   ge_double_scalarmult_vartime(&R, hram, &A, /* S */ sig->data + 32);
>   ge_tobytes(rcheck, &R);
> 
>   if (crypto_verify_32(rcopy, rcheck) == 0) {
>     return ED25519_SIGNATURE_VALID;
38,41c34,35
<   *mlen = -1;
<   memset(m,0,smlen);
<   return -1;
< }
---
>   return ED25519_SIGNATURE_INVALID;
> }
\ No newline at end of file
```

**sign.c**:
```diff
1,7c1,4
< int crypto_sign(
<   unsigned char *sm,unsigned long long *smlen,
<   const unsigned char *m,unsigned long long mlen,
<   const unsigned char *sk
< )
< {
<   unsigned char pk[32];
---
> void ed25519_sign(signature_t *sig, const unsigned char *msg,
>                   unsigned long long msglen, const public_key_t *pk,
>                   const private_key_t *sk) {
>   unsigned char context[SHA_CONTEXT_SIZE];
9c6
<   unsigned char nonce[64];
---
>   unsigned char nonce[64];  // r
13,15c10,12
<   memmove(pk,sk + 32,32);
< 
<   crypto_hash_sha512(az,sk,32);
---
>   sha512_init((void *)context);
>   sha512_update((void *)context, sk->data, ed25519_privkey_SIZE);
>   sha512_final((void *)context, az);
18a16,17
>   /* az: 64-byte H(sk) */
>   /* az: 32-byte scalar a, 32-byte randomizer z */
20,24c19,23
<   *smlen = mlen + 64;
<   memmove(sm + 64,m,mlen);
<   memmove(sm + 32,az + 32,32);
<   crypto_hash_sha512(nonce,sm + 32,mlen + 32);
<   memmove(sm + 32,pk,32);
---
>   sha512_init((void *)context);
>   sha512_update((void *)context, /* z */ az + 32, 32);
>   sha512_update((void *)context, msg, msglen);
>   sha512_final((void *)context, nonce);
>   /* nonce: 64-byte H(z,msg) */
27,28c26,36
<   ge_scalarmult_base(&R,nonce);
<   ge_p3_tobytes(sm,&R);
---
>   ge_scalarmult_base(&R, nonce);
>   ge_p3_tobytes(sig->data, &R);
>   /* sig: [32 bytes R | 32 bytes uninit] */
> 
>   sha512_init((void *)context);
>   // first 32 bytes of signature
>   sha512_update((void *)context, /* R */ sig->data, 32);
>   sha512_update((void *)context, /* A */ pk->data, ed25519_pubkey_SIZE);
>   sha512_update((void *)context, msg, msglen);
>   sha512_final((void *)context, hram);
>   /* hram: 64-byte H(R,A,m) */
30d37
<   crypto_hash_sha512(hram,sm,mlen + 64);
32,34c39,40
<   sc_muladd(sm + 32,hram,az,nonce);
< 
<   return 0;
---
>   sc_muladd(sig->data + 32, hram, az, nonce);
>   /* sig: [32 bytes R | 32 bytes S] */
```
