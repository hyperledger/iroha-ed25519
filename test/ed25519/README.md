Original implementations of Ed25519 is defined with use of SHA2 512.

Test data [`sign.input.with.sha2.txt`](./sign.input.with.sha2.txt) is taken from [http://ed25519.cr.yp.to/python/sign.input](http://ed25519.cr.yp.to/python/sign.input)

However, with sha3 signatures will be different.

To test ed25519 with sha3 I generated own test vectors ([`sign.input.with.sha3.txt`](./sign.input.with.sha3.txt)):
1. use original structure, original message, original private key (public key differs)
2. use ref10 implementation and sha3_brainhub implementation

Source code is available at [tosha3](./tosha3) directory.
