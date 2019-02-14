#include <benchmark/benchmark.h>
#include <ed25519/ed25519.h>

std::string random_str(size_t size) {
  unsigned int SEED = 1337;

  std::string s(size, 0);
  for (auto i = 0u; i < size; i++) {
    s[i] = static_cast<char>(rand_r(&SEED));
  }
  return s;
}

static void Sign(benchmark::State &state) {
  std::string msg;
  private_key_t priv{};
  public_key_t pub{};
  signature_t sig{};

  // use the same keypair for all signing operations
  ed25519_create_keypair(&priv, &pub);

  for (auto _ : state) {
    state.PauseTiming();
    msg = random_str(state.range(0));
    state.ResumeTiming();

    ed25519_sign(&sig, reinterpret_cast<const unsigned char *>(msg.data()),
                 msg.size(), &pub, &priv);
  }
}

static void VerifyCorrectSig(benchmark::State &state) {
  std::string msg = "hello";
  private_key_t priv;
  public_key_t pub;
  signature_t sig;

  // use the same keypair for all signing operations
  ed25519_create_keypair(&priv, &pub);
  ed25519_sign(&sig, reinterpret_cast<const unsigned char *>(msg.data()),
               msg.size(), &pub, &priv);

  for (auto _ : state) {
    ed25519_verify(&sig, reinterpret_cast<const unsigned char *>(msg.data()),
                   msg.size(), &pub);
  }
}

static void VerifyIncorrectSig(benchmark::State &state) {
  std::string msg = "hello";
  private_key_t priv;
  public_key_t pub;
  signature_t sig;

  // use the same keypair for all signing operations
  ed25519_create_keypair(&priv, &pub);
  ed25519_sign(&sig, reinterpret_cast<const unsigned char *>(msg.data()),
               msg.size(), &pub, &priv);
  // intentionally break the signature
  sig.data[0] = 0;
  sig.data[1] = 1;

  for (auto _ : state) {
    ed25519_verify(&sig, reinterpret_cast<const unsigned char *>(msg.data()),
                   msg.size(), &pub);
  }
}

static void GenerateKeypair(benchmark::State &state) {
  private_key_t priv;
  public_key_t pub;

  for (auto _ : state) {
    ed25519_create_keypair(&priv, &pub);
  }
}

static void SHA512(benchmark::State &state) {
  unsigned char hash[SHA_512_SIZE];
  std::string msg;

  for (auto _ : state) {
    state.PauseTiming();
    msg = random_str(state.range(0));
    state.ResumeTiming();

    sha512(hash, reinterpret_cast<const unsigned char *>(msg.data()),
           msg.size());
  }
}

BENCHMARK(Sign)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK(SHA512)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK(VerifyCorrectSig);
BENCHMARK(VerifyIncorrectSig);
BENCHMARK(GenerateKeypair);

BENCHMARK_MAIN();
