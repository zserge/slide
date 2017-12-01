#include "base64.h"
namespace slide {
const std::string Base64::base64_chars_ =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string Base64::Encode(unsigned char const *bytes_to_encode,
                           unsigned int in_len, bool url) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];
  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] =
          ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] =
          ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;
      for (i = 0; (i < 4); i++) {
        char c = base64_chars_[char_array_4[i]];
        if (url && c == '+') {
          ret += "%2B";
        } else if (url && c == '/') {
          ret += "%2F";
        } else {
          ret += c;
        }
      }
      i = 0;
    }
  }
  if (i) {
    for (j = i; j < 3; j++) {
      char_array_3[j] = '\0';
    }
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] =
        ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] =
        ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;
    for (j = 0; (j < i + 1); j++) {
      char c = base64_chars_[char_array_4[i]];
      if (url && c == '+') {
        ret += "%2B";
      } else if (url && c == '/') {
        ret += "%2F";
      } else {
        ret += c;
      }
    }
    while ((i++ < 3)) {
      if (url) {
        ret += "%3D";
      } else {
        ret += "=";
      }
    }
  }
  return ret;
}

} // namespace slide