#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <ed25519/ed25519.h>

#define STR2(x) #x
#define STR(x) STR2(x)

#include "helpers.hpp"

int main() {
  std::ifstream f(STR(TESTDATAIN_PATH));
  std::ofstream out(STR(TESTDATAOUT_PATH));

  if (!f.is_open()) {
    throw std::runtime_error(":(");
  }

  auto cases = parseTestCases(f);

  f.close();

  bool hasfailed = false;
  for (const auto &c : cases) {
    std::string hpriv = c.privkey;
    auto sk           = make<private_key_t>(hex2bytes(hpriv));

    public_key_t pk{};
    ed25519_derive_public_key(&sk, &pk);
    std::string pub  = make_str(&pk, ed25519_pubkey_SIZE);
    std::string hpub = bytes2hex((unsigned char *) pub.data(), pub.size());

    std::string hmsg = c.message;

    std::string hsig = sign(hmsg, hpub, hpriv);
    out << hpriv;
    out << hpub;
    out << ":";
    out << hpub;
    out << ":";
    out << hmsg;
    out << ":";
    out << hsig;
    out << hmsg;
    out << ":" << std::endl;

    if (!verify(hmsg, hpub, hsig)) {
      hasfailed = true;
      std::cerr << "sig is invalid" << std::endl;
      std::exit(0);
    }
  }
  if (!hasfailed) {
    std::cout << "all signatures valid" << std::endl;
  }

  out.close();
}
