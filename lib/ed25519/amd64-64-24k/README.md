# amd64-64-24k

```bash
$ diff -r tools/supercop-20171020/crypto_sign/ed25519/amd64-64-24k tools/ed25519-sha3/lib/ed25519/amd64-64-24k
```

```diff
Only in tools/supercop-20171020/crypto_sign/ed25519/amd64-64-24k: batch.c
Only in tools/ed25519-sha3/lib/ed25519/amd64-64-24k/: CMakeLists.txt
Only in tools/ed25519-sha3/lib/ed25519/amd64-64-24k/: ed25519.c
diff -r tools/supercop-20171020/crypto_sign/ed25519/amd64-64-24k/hram.c tools/ed25519-sha3/lib/ed25519/amd64-64-24k/hram.c
1c1
< #include "crypto_hash_sha512.h"
---
> #include "ed25519/sha512.h"
12c12
<   crypto_hash_sha512(hram,playground,smlen);
---
>   sha512(hram,playground,smlen);
Only in tools/supercop-20171020/crypto_sign/ed25519/amd64-64-24k: keypair.c
Only in tools/supercop-20171020/crypto_sign/ed25519/amd64-64-24k: open.c
Only in tools/supercop-20171020/crypto_sign/ed25519/amd64-64-24k: sign.c
```

## open.c, keypair.c, sign.c

Now all these files are inside `ed25519.c`.

**open.c**:
```diff
1,6c1,3
< int crypto_sign_open(
<     unsigned char *m,unsigned long long *mlen,
<     const unsigned char *sm,unsigned long long smlen,
<     const unsigned char *pk
<     )
< {
---
> int ed25519_verify(const signature_t *sig, const unsigned char *msg,
>                    unsigned long long msglen, const public_key_t *pk) {
>   unsigned char context[SHA_CONTEXT_SIZE];
14,25c11,25
<   if (smlen < 64) goto badsig;
<   if (sm[63] & 224) goto badsig;
<   if (ge25519_unpackneg_vartime(&get1,pk)) goto badsig;
< 
<   memmove(pkcopy,pk,32);
<   memmove(rcopy,sm,32);
< 
<   sc25519_from32bytes(&scs, sm+32);
< 
<   memmove(m,sm,smlen);
<   memmove(m + 32,pkcopy,32);
<   crypto_hash_sha512(hram,m,smlen);
---
>   if (sig->data[63] & 224) goto badsig;
>   if (ge25519_unpackneg_vartime(&get1, pk->data)) goto badsig;
> 
>   memcpy(pkcopy, pk->data, ed25519_pubkey_SIZE);
>   memcpy(rcopy, /* R, first 32 bytes */ sig->data, 32);
> 
>   sc25519_from32bytes(&scs, /* S, seconds 32 bytes */ sig->data + 32);
> 
>   sha512_init((void *)context);
>   // first 32 bytes of signature
>   sha512_update((void *)context, /* R */ sig->data, 32);
>   sha512_update((void *)context, /* A */ pk->data, ed25519_pubkey_SIZE);
>   sha512_update((void *)context, msg, msglen);
>   sha512_final((void *)context, hram);
>   /* scs: S = nonce + H(R,A,m)a */
32,36c32,33
<   if (crypto_verify_32(rcopy,rcheck) == 0) {
<     memmove(m,m + 64,smlen - 64);
<     memset(m + smlen - 64,0,64);
<     *mlen = smlen - 64;
<     return 0;
---
>   if (crypto_verify_32(rcopy, rcheck) == 0) {
>     return ED25519_SIGNATURE_VALID;
40,43c37,38
<   *mlen = (unsigned long long) -1;
<   memset(m,0,smlen);
<   return -1;
< }
\ No newline at end of file
---
>   return ED25519_SIGNATURE_INVALID;
> }
```

**keypair.c**:
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
7,8c13
<   randombytes(sk,32);
<   crypto_hash_sha512(az,sk,32);
---
>   sha512(az, sk->data, 32);
13,14c18,19
<   sc25519_from32bytes(&scsk,az);
<   
---
>   sc25519_from32bytes(&scsk, az);
> 
16,19c21,22
<   ge25519_pack(pk, &gepk);
<   memmove(sk + 32,pk,32);
<   return 0;
< }
\ No newline at end of file
---
>   ge25519_pack(pk->data, &gepk);
> }
```

**sign.c**:
```diff
1,7c1,4
< int crypto_sign(
<     unsigned char *sm,unsigned long long *smlen,
<     const unsigned char *m,unsigned long long mlen,
<     const unsigned char *sk
<     )
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
14,17c11,13
<   memmove(pk,sk + 32,32);
<   /* pk: 32-byte public key A */
< 
<   crypto_hash_sha512(az,sk,32);
---
>   sha512_init((void *)context);
>   sha512_update((void *)context, sk->data, ed25519_privkey_SIZE);
>   sha512_final((void *)context, az);
20a17
>   /* az: 64-byte H(sk) */
23,29c20,24
<   *smlen = mlen + 64;
<   memmove(sm + 64,m,mlen);
<   memmove(sm + 32,az + 32,32);
<   /* sm: 32-byte uninit, 32-byte z, mlen-byte m */
< 
<   crypto_hash_sha512(nonce, sm+32, mlen+32);
<   /* nonce: 64-byte H(z,m) */
---
>   sha512_init((void *)context);
>   sha512_update((void *)context, /* z */ az + 32, 32);
>   sha512_update((void *)context, msg, msglen);
>   sha512_final((void *)context, nonce);
>   /* nonce: 64-byte H(z,msg) */
33,37c28,29
<   ge25519_pack(sm, &ger);
<   /* sm: 32-byte R, 32-byte z, mlen-byte m */
<   
<   memmove(sm + 32,pk,32);
<   /* sm: 32-byte R, 32-byte A, mlen-byte m */
---
>   ge25519_pack(sig->data, &ger);
>   /* sig: [32 bytes R | 32 bytes uninit] */
39c31,36
<   crypto_hash_sha512(hram,sm,mlen + 64);
---
>   sha512_init((void *)context);
>   // first 32 bytes of signature
>   sha512_update((void *)context, /* R */ sig->data, 32);
>   sha512_update((void *)context, /* A */ pk->data, ed25519_pubkey_SIZE);
>   sha512_update((void *)context, msg, msglen);
>   sha512_final((void *)context, hram);
43c40
<   sc25519_from32bytes(&scsk, az);
---
>   sc25519_from32bytes(&scsk, /* a */ az);
48,51c45,46
<   sc25519_to32bytes(sm + 32,&scs);
<   /* sm: 32-byte R, 32-byte S, mlen-byte m */
< 
<   return 0;
---
>   sc25519_to32bytes(sig->data + 32, &scs);
>   /* sig: [32 bytes R | 32 bytes S] */
```
