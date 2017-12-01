#ifndef SLIDE_HPP
#define SLIDE_HPP

#include "colour_t.h"
#include "dimenions.h"
#include "documents.h"
#include "styles.h"
#include "wcairo.h"
#include <string>
#include <vector>

namespace slide {

struct Token {
  const long pos;
  const int line;
  const Style style;
  const std::string text;
};

using Slide = std::vector<Token>;
using Deck = std::vector<Slide>;

// Static functions only
Deck parse(const std::string &text);
std::pair<int, int> render_scale(Page_b &p, Slide &slide, const colour_t &color,
                                 int xoff, int yoff, float scale);
void render(Page_b &page, Slide &slide, const colour_t &fg, const colour_t &bg);

} // namespace slide

#endif // SLIDE_HPP
