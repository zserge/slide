#ifndef SLIDE_ENCODING_H
#define SLIDE_ENCODING_H

#include <string>

class base64 {
public:
  static std::string encode(unsigned char const *bytes_to_encode,
                            unsigned int in_len, bool url);

private:
  static const std::string base64_chars;
};

#endif // SLIDE_ENCODING_H
