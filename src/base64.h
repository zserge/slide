#ifndef SLIDE_ENCODING_H
#define SLIDE_ENCODING_H

#include <string>

namespace slide {
class Base64 {
public:
  static std::string Encode(unsigned char const *bytes_to_encode,
                            unsigned int in_len, bool url);

private:
  static const std::string base64_chars_;
};
} // namespace slide

#endif // SLIDE_ENCODING_H
