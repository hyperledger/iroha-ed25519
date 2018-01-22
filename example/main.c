#include <assert.h>
#include <ed25519/ed25519.h>

int main(int arc, char** argv) {
  public_key_t pub;
  private_key_t priv;
  int ret = ed25519_create_keypair(&priv, &pub);
  assert(ret != 0);  // status code: ok

  const int size           = 7;
  unsigned char *buff = "ed25519";

  signature_t sig;
  ed25519_sign(&sig, buff, size, &pub, &priv);

  ret = ed25519_verify(&sig, buff, size, &pub);
  assert(ret != 0);  // correct result

  return 0;
}
