#include <ed25519/ed25519/crypto_types.h>
#include <ed25519/ed25519.h>
#include <gtest/gtest.h>
#include <fstream>
#include "hexutil.hpp"

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#ifndef TESTDATA_PATH
#error \
    "Define TESTDATA_PATH with path to test data. Example file: http://ed25519.cr.yp.to/python/sign.input"
#endif

#include "tosha3/helpers.hpp"

/***************************/
class Ed25519_Fixture : public ::testing::Test {
 public:
  Ed25519_Fixture() {
    std::ifstream f(STRINGIFY(TESTDATA_PATH));

    if (!f.is_open()) {
      throw std::runtime_error("can not open file " STRINGIFY(TESTDATA_PATH));
    }

    testcases = parseTestCases(f);
    f.close();
  }

  void SetUp() override {}

  void TearDown() override {}

  std::vector<TestCase> testcases;
};

/***************************/

TEST_F(Ed25519_Fixture, Sign) {
  uint32_t tcase = 0;
  for (const auto &c : testcases) {
    std::string expected = c.signature;
    std::string actual   = sign(c.message, c.pubkey, c.privkey);
    EXPECT_EQ(expected, actual) << "signatures are different: case " << tcase++;
  }
}

TEST_F(Ed25519_Fixture, Verify) {
  uint32_t tcase = 0;
  for (const auto &c : testcases) {
    bool actual = verify(c.message, c.pubkey, c.signature);
    EXPECT_TRUE(actual) << "signature is invalid: case " << tcase++;
  }
}

TEST(Ed25519, SignThenVerifyFixedMsg) {
  std::string hmsg = bytes2hex((unsigned char *)"hello", 5);

  public_key_t pub{};
  private_key_t priv{};
  ASSERT_TRUE(ed25519_create_keypair(&priv, &pub));

  std::string hpub  = bytes2hex(pub.data, ed25519_pubkey_SIZE);
  std::string hpriv = bytes2hex(priv.data, ed25519_privkey_SIZE);
  std::string hsig  = sign(hmsg, hpub, hpriv);

  ASSERT_TRUE(verify(hmsg, hpub, hsig)) << "signature is incorect";
}

TEST(Ed25519, DifferentKeypairs) {
  public_key_t pub1{};
  private_key_t priv1{};
  ASSERT_TRUE(ed25519_create_keypair(&priv1, &pub1));

  public_key_t pub2{};
  private_key_t priv2{};
  ASSERT_TRUE(ed25519_create_keypair(&priv2, &pub2));

  auto a = make_str(&pub1, ed25519_pubkey_SIZE);
  auto b = make_str(&pub2, ed25519_pubkey_SIZE);
  auto c = make_str(&priv1, ed25519_privkey_SIZE);
  auto d = make_str(&priv2, ed25519_privkey_SIZE);
  ASSERT_NE(a, b) << "two consecutively generated pubkeys are equal!";
  ASSERT_NE(c, d) << "two consecutively generated privkeys are equal!";
}

TEST(Ed25519, NonEmptyKeypair) {
  std::string empty(32, 0);
  public_key_t pub1{};
  private_key_t priv1{};
  ed25519_create_keypair(&priv1, &pub1);

  auto a = make_str(&pub1, ed25519_pubkey_SIZE);
  auto b = make_str(&priv1, ed25519_privkey_SIZE);

  ASSERT_NE(a, empty);
  ASSERT_NE(b, empty);
}

TEST(Ed25519, PubkeyDerivation) {
  public_key_t pub1{};
  private_key_t priv{};
  ASSERT_TRUE(ed25519_create_keypair(&priv, &pub1));

  public_key_t pub2{};
  ed25519_derive_public_key(&priv, &pub2);

  auto a = make_str(&pub1, ed25519_pubkey_SIZE);
  auto b = make_str(&pub2, ed25519_pubkey_SIZE);
  ASSERT_EQ(a, b) << "pubkeys are not equal";
}
