#ifndef PROJECT_TEST_FUNC_HPP_
#define PROJECT_TEST_FUNC_HPP_

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

inline std::string hex2bytes(const std::string &hex) {
  std::string bytes;

  for (unsigned int i = 0; i < hex.length(); i += 2) {
    std::string byteString = hex.substr(i, 2);
    char byte = (char)strtol(byteString.c_str(), NULL, 16);
    bytes.push_back(byte);
  }

  return bytes;
}

inline std::string bytes2hex(unsigned char *bytes, size_t size) {
  std::stringstream stream;

  for (auto i = 0u; i < size; i++) {
    stream << std::hex << std::setfill('0') << std::setw(2) << (int)bytes[i];
  }

  return stream.str();
}

#endif //  PROJECT_TEST_FUNC_HPP_
