[![](https://travis-ci.org/hyperledger/iroha-ed25519.svg?branch=master)](https://travis-ci.org/hyperledger/iroha-ed25519)
[![codecov](https://codecov.io/gh/hyperledger/iroha-ed25519/branch/master/graph/badge.svg)](https://codecov.io/gh/hyperledger/iroha-ed25519)

# Ed25519 digital signature algorithm


Ed25519 digital signature algorithm is described in [RFC8032](https://tools.ietf.org/html/rfc8032).
This repository aims to provide modularized implementation of this algorithm.

Originally Ed25519 consists of three *modules*:
- digital signature algorithm itself
- SHA512 hash function
- random number generator, to generate keypairs

This repository offers at least two different C implementations for every module.
Every implementation is tested and can be replaced with other at link-time.
New implementations can be added as well.

During CMake time, users are able to choose any of these implementations using cmake definitions:

- `EDIMPL`
    - `ref10` - portable C implementation.
    - `amd64-64-24k-pic` - optimized C and ASM implementation, works only on Linux amd64. Has fixes in ASM files, to allow *position independent code* (`-fPIC`) builds.
- `HASH`
    - `sha2_openssl`
    - `sha2_sphlib`
    - `sha3_brainhub` - default
- `RANDOM`
    - `rand_openssl`
    - `dev_urandom` - default
    - `dev_random`
    - `bcryptgen` (windows only)
- `BUILD`
    - `STATIC`
    - `SHARED` - build ed25519 library as shared library (default)

**Example**:
We want to build shared library with fast amd64 implementation, SHA3 and PRNG, which reads entropy from `/dev/urandom`:

```bash
$ cmake .. -DEDIMPL=amd64-64-24k-pic -DHASH=sha3_brainhub -DRANDOM=dev_urandom -DBUILD=SHARED                   bogdan@Bogdans-MacBook-Pro
-- [hunter] Calculating Toolchain-SHA1
-- [hunter] Calculating Config-SHA1
-- [hunter] HUNTER_ROOT: /Users/bogdan/.hunter
-- [hunter] [ Hunter-ID: 1c7aaff | Toolchain-ID: 9e23df5 | Config-ID: 997ea55 ]
-- [hunter] GTEST_ROOT: /Users/bogdan/.hunter/_Base/1c7aaff/9e23df5/997ea55/Install (ver.: 1.8.0-hunter-p11)
-- EDIMPL=amd64-64-24k-pic is selected (Ed25519 implementation)
-- HASH=sha3_brainhub is selected (SHA implementation)
-- RANDOM=dev_urandom is selected (RNG implementation)
-- BUILD=SHARED is selected (library build type)
-- [ed25519] Target RANDOM=bcryptgen is not supported on your platform
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/bogdan/tools/iroha-ed25519/build
```

**Note**: only those targets (including tests) will be built, which are specified in `EDIMPL`, `HASH`, `RANDOM` variables.

# API

- API for Ed25519 is defined at [ed25519.h](include/ed25519/ed25519/ed25519.h)
- API for SHA512 is defined at [sha512.h](include/ed25519/ed25519/sha512.h)
- API for RNG is defined at [randombytes.h](include/ed25519/ed25519/randombytes.h)

# Modules

## ed25519 digital signature algorithm

- `ref10` - portable but relatively slow C implementation, originally copied from [supercop-20171020](http://bench.cr.yp.to/supercop.html).
Its API was redesigned to separate signature data from the *signed message* content.
- `amd64-64-24k-pic` - fast (4x ref10) but non-portable C and ASM implementation, only for AMD64.
                       Copied from [supercop-20171020](http://bench.cr.yp.to/supercop.html).
                       Adopted to be included as a module. Has Position Independent Code (`-fPIC`) fixes by @l4l.

## SHA512 has function as a dependency of ed25519

- `sha2_openssl` - implementation of FIPS 180-4 SHA2 512 hash function, which uses openssl underneath
- `sha2_sphlib` - implementation of FIPS 180-4 SHA2 512 hash function, which was taken from [supercop-20190110](http://bench.cr.yp.to/supercop.html)
- `sha3_brainhub` - implementation of FIPS 202 SHA3 512 hash function taken from [brainhub repository](https://github.com/brainhub/SHA3IUF).
Repository consists of a single C file, which was adopted to be included in a project as a module.

## PRNG implementation as a dependency of ed25519

To generate keypair ed25519 needs a source of randomness (entropy).

This repository offers 4 implementations:
- `rand_openssl` uses RAND_bytes from openssl
- `dev_urandom` reads entropy from `/dev/urandom`
- `dev_random` reads entropy from `/dev/random` (blocking call, uses busy waiting when user asks for more entropy than device can offer)
- `bcryptgen` reads entropy from windows preferred entropy source.
