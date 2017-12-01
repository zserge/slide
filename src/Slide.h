#ifndef SLIDE_HPP
#define SLIDE_HPP

#include "Color.h"
#include "Dimenions.h"
#include "Documents.h"
#include "Styles.h"
#include "WCairo.h"
#include <string>
#include <vector>

namespace slide {

struct Token {
  const long position_;
  const int line_;
  const Style style_;
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
