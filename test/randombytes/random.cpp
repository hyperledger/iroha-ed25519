#include <gtest/gtest.h>
#include <cmath>
#include <random>
#include <ed25519/ed25519/randombytes.h>

double entropy(std::vector<uint8_t> p) {
  std::vector<uint8_t> map(256, 0);

  // calculate frequencies
  for (unsigned char i : p) {
    ++map[i];
  }

  double e = 0;
  for (const auto c : map) {
    double freq = (double)c / p.size();

    if (freq == 0) continue;

    e += freq * log2(freq);
  }

  return e * -1;
}

double max_entropy(int max) { return log2(max + 1); }

TEST(Random, EnoughEntropy) {
  std::vector<uint8_t> buf(256, 0);
  ASSERT_TRUE(randombytes(buf.data(), buf.size()));

  auto max = max_entropy(256); // 8
  auto ent = entropy(buf);

  std::cout << "max entropy: " << max << std::endl;
  std::cout << "calculated:  " << ent << std::endl;

  // this test is probably not necessary
  // but helps to find bad implementations
  ASSERT_GE(ent, max - 2) << "bad randomness source";
}
