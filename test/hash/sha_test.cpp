#include <ed25519/ed25519/sha256.h>
#include <ed25519/ed25519/sha512.h>
#include <gtest/gtest.h>
#include "hexutil.hpp"

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#ifndef TESTDATA_PATH
#error "Define TESTDATA_PATH"
#endif

struct TestCase {
  std::string msg;
  std::string sha256;
  std::string sha512;
};

const std::vector<TestCase> testdata = {
#include STRINGIFY(TESTDATA_PATH)
};

TEST(SHA_512, IUF_partial) {
  // search for test case where msg.size > 1
  uint32_t tcase = 0;
  while (testdata[tcase].msg.size() <= 1) {
    tcase++;
    if (tcase > testdata.size())
      throw std::runtime_error("can not find test case with msg.size() > 1");
  }

  std::string expected = testdata[tcase].sha512;
  std::string msg      = testdata[tcase].msg;
  std::string out(SHA_512_SIZE, 0);

  // first half
  std::string first = msg.substr(0, msg.size() / 2);
  // second half
  std::string second = msg.substr(msg.size() / 2, msg.size() - 1);

  unsigned char ctx[SHA_512_CONTEXT_SIZE];
  sha512_init((void *)ctx);
  /* first half */
  sha512_update((void *)ctx,
                reinterpret_cast<const unsigned char *>(first.data()),
                first.size());
  /* second half */
  sha512_update((void *)ctx,
                reinterpret_cast<const unsigned char *>(second.data()),
                second.size());

  sha512_final((void *)ctx, (unsigned char *)out.data());

  std::string actual = bytes2hex((unsigned char *)out.data(), out.size());

  EXPECT_EQ(expected, actual) << "different hashes: case " << tcase;
}

TEST(SHA_512, IUF) {
  uint32_t tcase = 0;
  for (const auto &c : testdata) {
    std::string expected = c.sha512;
    std::string msg      = c.msg;
    std::string out(SHA_512_SIZE, 0);

    unsigned char ctx[SHA_512_CONTEXT_SIZE];
    sha512_init((void *)ctx);
    sha512_update((void *)ctx,
                  reinterpret_cast<const unsigned char *>(msg.data()),
                  msg.size());
    sha512_final((void *)ctx, (unsigned char *)out.data());

    std::string actual = bytes2hex((unsigned char *)out.data(), out.size());

    EXPECT_EQ(expected, actual) << "different hashes: case " << tcase++;
  }
}

TEST(SHA_512, Immediate) {
  uint32_t tcase = 0;
  for (const auto &c : testdata) {
    std::string expected = c.sha512;
    std::string msg      = c.msg;
    std::string out(SHA_512_SIZE, 0);

    sha512((unsigned char *)out.data(),
           reinterpret_cast<const unsigned char *>(msg.data()),
           msg.size());

    std::string actual = bytes2hex((unsigned char *)out.data(), out.size());
    EXPECT_EQ(expected, actual) << "different hashes: case " << tcase++;
  }
}

TEST(SHA_256, IUF_partial) {
  // search for test case where msg.size > 1
  uint32_t tcase = 0;
  while (testdata[tcase].msg.size() <= 1) {
    tcase++;
    if (tcase > testdata.size())
      throw std::runtime_error("can not find test case with msg.size() > 1");
  }

  std::string expected = testdata[tcase].sha256;
  std::string msg      = testdata[tcase].msg;
  std::string out(SHA_256_SIZE, 0);

  // first half
  std::string first = msg.substr(0, msg.size() / 2);
  // second half
  std::string second = msg.substr(msg.size() / 2, msg.size() - 1);

  unsigned char ctx[SHA_256_CONTEXT_SIZE];
  sha256_init((void *)ctx);
  /* first half */
  sha256_update((void *)ctx,
                reinterpret_cast<const unsigned char *>(first.data()),
                first.size());
  /* second half */
  sha256_update((void *)ctx,
                reinterpret_cast<const unsigned char *>(second.data()),
                second.size());

  sha256_final((void *)ctx, (unsigned char *)out.data());

  std::string actual = bytes2hex((unsigned char *)out.data(), out.size());

  EXPECT_EQ(expected, actual) << "different hashes: case " << tcase;
}

TEST(SHA_256, IUF) {
  uint32_t tcase = 0;
  for (const auto &c : testdata) {
    std::string expected = c.sha256;
    std::string msg      = c.msg;
    std::string out(SHA_256_SIZE, 0);

    unsigned char ctx[SHA_256_CONTEXT_SIZE];
    sha256_init((void *)ctx);
    sha256_update((void *)ctx,
                  reinterpret_cast<const unsigned char *>(msg.data()),
                  msg.size());
    sha256_final((void *)ctx, (unsigned char *)out.data());

    std::string actual = bytes2hex((unsigned char *)out.data(), out.size());

    EXPECT_EQ(expected, actual) << "different hashes: case " << tcase++;
  }
}

TEST(SHA_256, Immediate) {
  uint32_t tcase = 0;
  for (const auto &c : testdata) {
    std::string expected = c.sha256;
    std::string msg      = c.msg;
    std::string out(SHA_256_SIZE, 0);

    sha256((unsigned char *)out.data(),
           reinterpret_cast<const unsigned char *>(msg.data()),
           msg.size());

    std::string actual = bytes2hex((unsigned char *)out.data(), out.size());
    EXPECT_EQ(expected, actual) << "different hashes: case " << tcase++;
  }
}
