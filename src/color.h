#ifndef SLIDE_COLOUR_T_H
#define SLIDE_COLOUR_T_H

#include <cstdint>

namespace slide {
class Color {
public:
  Color(void) : argb_(0xffeeeeee) { /* Intentionally Blank */
  }

  Color(const uint32_t argb) : argb_(argb) { /* Intentionally Blank */
  }

  Color(const int argb) : argb_(argb) { /* Intentionally Blank */
  }

  double red(void) const { return ((argb_ >> 16) & 0xff) / 255.0; }

  double green(void) const { return ((argb_ >> 8) & 0xff) / 255.0; }

  double blue(void) const { return ((argb_)&0xff) / 255.0; }

  double alpha(void) const { return ((argb_ >> 24) & 0xff) / 255.0; }

  uint32_t raw(void) const { return argb_; }

  void operator=(const uint32_t &rhs) { argb_ = rhs; }

  void operator=(const Color &rhs) { argb_ = rhs.argb_; }

  void operator=(const int &rhs) { argb_ = rhs; }

  bool operator==(const uint32_t &rhs) const { return rhs == argb_; }

  bool operator==(const Color &rhs) const { return rhs.argb_ == argb_; }

  bool operator!=(const uint32_t &rhs) const { return rhs != argb_; }

  bool operator!=(const Color &rhs) const { return argb_ != rhs.argb_; }

  bool operator==(const int &rhs) const { return rhs == (int)argb_; }

  bool operator!=(const int &rhs) const { return rhs != (int)argb_; }

  bool operator!=(int &rhs) const { return rhs != (int)argb_; }

protected:
  uint32_t argb_;
};
} // namespace slide

#endif // SLIDE_COLOUR_T_H
