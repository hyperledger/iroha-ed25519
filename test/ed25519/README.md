Original implementations of Ed25519 is defined with use of SHA2 512.

Test data [`sign.input.with.sha2.txt`](./sign.input.with.sha2.txt) is taken from [http://ed25519.cr.yp.to/python/sign.input](http://ed25519.cr.yp.to/python/sign.input)

However, with sha3 signatures will be different.

To test ed25519 with sha3 I generated own test vectors ([`sign.input.with.sha3.txt`](./sign.input.with.sha3.txt)):
1. use original structure, original message, original private key (public key differs)
2. use ref10 implementation and sha3_brainhub implementation

Source code is available at [tosha3](../../example/tosha3) directory.

# sign.input format

```
[ 
  32 bytes private key (hexencoded - 64 bytes)
  32 bytes public key  (hexencoded - 64 bytes)
  :
  32 bytes public key  (hexencoded - 64 bytes) - should be same as previous
  :
  hexencoded message. Can be arbitrary length, even 0
  :
  64 bytes signature   (hexencoded - 128 bytes)
  hexencoded message. Same as previous msg field.
  :
  \n
]```
