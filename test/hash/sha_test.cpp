#include <gtest/gtest.h>
#include <ed25519/ed25519/sha512.h>
#include "hexutil.hpp"

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#ifndef TESTDATA_PATH
#error "Define TESTDATA_PATH"
#endif

const std::vector<std::pair<std::string, std::string>> testdata = {
#include STRINGIFY(TESTDATA_PATH)
};

TEST(SHA_512, IUF_partial) {
  // search for test case where msg.size > 1
  uint32_t tcase = 0;
  while (testdata[tcase].first.size() <= 1) {
    tcase++;
    if (tcase > testdata.size())
      throw std::runtime_error("can not find test case with msg.size() > 1");
  }

  std::string expected = testdata[tcase].second;
  std::string msg      = testdata[tcase].first;
  std::string out(64, 0);

  // first half
  std::string first = msg.substr(0, msg.size() / 2);
  // second half
  std::string second = msg.substr(msg.size() / 2, msg.size() - 1);

  unsigned char ctx[SHA_CONTEXT_SIZE];
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
    std::string expected = c.second;
    std::string msg      = c.first;
    std::string out(64, 0);

    unsigned char ctx[SHA_CONTEXT_SIZE];
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
    std::string expected = c.second;
    std::string msg      = c.first;
    std::string out(64, 0);

    sha512((unsigned char *)out.data(),
           reinterpret_cast<const unsigned char *>(msg.data()),
           msg.size());

    std::string actual = bytes2hex((unsigned char *)out.data(), out.size());
    EXPECT_EQ(expected, actual) << "different hashes: case " << tcase++;
  }
}
