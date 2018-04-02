#ifndef ED25519_HELPERS_HPP_
#define ED25519_HELPERS_HPP_

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "ed25519/ed25519.h"
#include "hexutil.hpp"

// hexencoded data
struct TestCase {
  std::string message{};
  std::string pubkey{};
  std::string privkey{};
  std::string signature{};
};

template <typename T>
T make(std::string buf) {
  T s{};
  std::copy(buf.begin(), buf.begin() + buf.size(), s.data);
  return s;
}

template <typename T>
std::string make_str(T *t, size_t size) {
  std::string s(size, 0);
  std::copy(t->data, t->data + size, s.begin());
  return s;
}

inline std::string /* sig */ sign(std::string hmsg,
                                  std::string hpub,
                                  std::string hpriv) {
  auto msg  = hex2bytes(hmsg);
  auto pub  = make<public_key_t>(hex2bytes(hpub));
  auto priv = make<private_key_t>(hex2bytes(hpriv));

  signature_t sig{};
  ed25519_sign(&sig, reinterpret_cast<const unsigned char *>(msg.data()),
               msg.size(), &pub, &priv);

  return bytes2hex(sig.data, 64);
}

inline bool verify(std::string hmsg, std::string hpub, std::string hsig) {
  auto msg = hex2bytes(hmsg);
  auto pub = make<public_key_t>(hex2bytes(hpub));
  auto sig = make<signature_t>(hex2bytes(hsig));

  return 1 ==
         ed25519_verify(&sig,
                        reinterpret_cast<const unsigned char *>(msg.data()),
                        msg.size(),
                        &pub);
}

/**
 * Parse sign.input file
 * @param f can be std::ifstream or std::istringstream or any derived class
 * @return
 */
template <typename T>
inline std::vector<TestCase> parseTestCases(T &&arg) {
  T f = std::forward<T>(arg);
  std::vector<TestCase> cases{};
  std::string line;
  while (std::getline(f, line)) {
    std::istringstream str(line);
    std::string token;

    int i = 0;
    TestCase c;
    while (std::getline(str, token, ':')) {
      // [32 bytes secret key| 32 bytes public key]:pubkey:msg:[64 bytes
      // signature | message]:
      switch (i) {
        case 0: {
          c.privkey = token.substr(0, 64 /* 32 hexencoded bytes */);
          c.pubkey  = token.substr(64, 128 /* 32 hexencoded bytes */);
          break;
        }
        case 1: {
          if (c.pubkey != token) {
            throw std::runtime_error(
                "pubkey for signing and for verifying are different");
          }
          break;
        }
        case 2: {
          c.message = token;
          break;
        }
        case 3: {
          c.signature = token.substr(0, 128 /* 64 bytes */);
        }
        default: {
          // do nothing
        }
      }

      i++;
    }

    cases.push_back(std::move(c));
  }

  return cases;
}

#endif  //  ED25519_HELPERS_HPP_
