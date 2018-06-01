#ifndef SLIDE_HPP
#define SLIDE_HPP

#include "cairo_wrapper.h"
#include "color.h"
#include "dimensions.h"
#include "documents.h"
#include "styles.h"
#include <ostream>
#include <string>
#include <vector>

namespace slide {

class Token {
public:
  Token(const long p, const int l, const Style::Style s, const std::string t)
      : position_(p), line_(l), style_(s), text_(t) { /* Intentionally blank */
  }
  long position(void) const { return position_; }
  int line(void) const { return line_; }
  Style::Style style(void) const { return style_; }
  const std::string &text(void) const { return text_; }

  bool operator==(const Token &rhs) const {
    return (style_ == rhs.style_ && text_ == rhs.text_);
  }

  bool operator==(const Style::Style &rhs) const { return style_ == rhs; }

  bool operator==(const std::string &rhs) const { return text_ == rhs; }

  friend std::ostream &operator<<(std::ostream &out, const Token &rhs) {
    out << rhs.position_ << ", " << rhs.line_ << ", "
        << Style::to_str(rhs.style_) << ", " << rhs.text_;
    return out;
  }

protected:
  const long position_;
  const int line_;
  const Style::Style style_;
  const std::string text_;
};

using Slide = std::vector<Token>;
using Deck = std::vector<Slide>;

// Static functions only
Deck Parse(const std::string &text);
std::pair<int, int> RenderScale(Page &p, Slide &slide, const Color &color,
                                int xoff, int yoff, float scale);
void Render(Page &page, Slide &slide, const Color &fg, const Color &bg);

} // namespace slide

#endif // SLIDE_HPP
