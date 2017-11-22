#ifndef SLIDE_HPP
#define SLIDE_HPP

#include <vector>
#include <string>

namespace slide {

//
// Parser
//
	
enum Style { Normal, Strong, Header, Monospace };

struct Token {
  const int line;
  const Style style;
  const std::string text;
};
using Slide = std::vector<Token>;
using Deck = std::vector<Slide>;

static Deck parse(const std::string &text) {
  Deck deck{};
  auto lineno = 0;
  for (auto it = text.cbegin(), end = text.cend(); it < end;) {
    // Skip leading newlines
    for (; *it == '\n' && it < end; ++it, ++lineno)
      ;
    // Parse next slide
    Slide slide{};
    for (; *it != '\n' && it < end; ++it, ++lineno) {
      std::string text;
      Style style{Style::Normal};
      switch (*it) {
      case '@':
        ++it;
				// TODO: parse image path and geometry
				for (; it < end && *it != '\n'; ++it) {
				}
        break;
      case '#':
        ++it;
        style = Style::Header;
        for (; it < end && *it == ' '; ++it)
          ;
        break;
      case ' ':
        ++it;
        if (it < end && *it == ' ') {
          ++it;
          style = Style::Monospace;
        } else {
          text.push_back(' ');
        }
        break;
      case '.':
        ++it;
        break;
      }
      for (; it < end && *it != '\n'; ++it) {
        if (style == Style::Normal && *it == '*') {
          ++it;
          if (it < end && *it != ' ') {
            std::string em;
            for (; it < end && *it != '*' && *it != '\n'; ++it) {
              em.push_back(*it);
            }
            if (*it == '*') {
              slide.push_back(Token{lineno, style, text});
              slide.push_back(Token{lineno, Style::Strong, em});
              text = "";
            } else {
              text.push_back('*');
              text.append(em);
            }
          } else {
            text.push_back('*');
            text.push_back(*it);
          }
        } else {
          text.push_back(*it);
        }
      }
      slide.push_back(Token{lineno, style, text});
    }
    // Skip trailing newlines
    for (; *it == '\n' && it < end; ++it, ++lineno)
      ;
    if (slide.size() > 0) {
      deck.push_back(slide);
    }
  }
  return deck;
}

//
// Renderer
//

using Color = uint32_t;

class Page {
public:
  virtual int width() const = 0;
  virtual int height() const = 0;

  virtual int text_height(Style style, float scale) = 0;
  virtual int text_width(const std::string &text, Style style, float scale) = 0;

  virtual void bg(Color color) = 0;
  virtual void text(const std::string &text, Color color, int x, int y,
                    Style style, float scale) = 0;
};

class Document : public Page {
public:
  virtual void begin_page() = 0;
  virtual void end_page() = 0;
};

static std::pair<int, int> render_scale(Page &p, Slide &slide, Color color,
                                        int xoff, int yoff, float scale) {
  int w = 0;
  int h = 0;
  int line_height = 0;
  int line_width = 0;
  int prev_line = -1;
  for (auto token : slide) {
    if (token.line != prev_line) {
      h = h + line_height;
      line_width = 0;
      prev_line = token.line;
    }
    line_height = p.text_height(token.style, scale);
    int token_width = p.text_width(token.text, token.style, scale);
    int x = xoff + line_width;
    int y = yoff + h;
    if (color != 0) {
      p.text(token.text, color, x, y, token.style, scale);
    }
    line_width = line_width + token_width;
    if (line_width > w) {
      w = line_width;
    }
  }
  h = h + line_height;
  return std::make_pair(xoff + w, yoff + h);
}

static void render(Page &page, Slide &slide, Color fg, Color bg) {
  if (slide.size() == 0) {
    return;
  }

  float scale = 1.f;
  auto size = render_scale(page, slide, 0, 0, 0, scale);
  scale = std::min(page.width() * 0.8 / size.first,
                   page.height() * 0.8 / size.second);
  size = render_scale(page, slide, 0, 0, 0, scale);

  page.bg(bg);
  render_scale(page, slide, fg, (page.width() - size.first) / 2,
               (page.height() - size.second) / 2, scale);
}

} // namespace slide

//
// Rendering surface implementations
//

#include <cairo/cairo-pdf.h>
#include <cairo/cairo.h>

namespace slide {

class PDF : public Document {
public:
  PDF(const int w, const int h) : w(w), h(h) {
    this->surface = cairo_pdf_surface_create("out.pdf", w, h);
    this->cr = cairo_create(this->surface);
    cairo_set_source_rgb(this->cr, 0, 0, 0);
  }

  ~PDF() {
    cairo_surface_destroy(this->surface);
    cairo_destroy(this->cr);
  }

  void begin_page() {}
  void end_page() { cairo_show_page(this->cr); }

  int width() const { return this->w; }
  int height() const { return this->h; }

  int text_height(Style style, float scale) {
    cairo_font_extents_t fe;
    this->set_font(style, scale);
    cairo_font_extents(this->cr, &fe);
    return (int)(fe.height);
  }

  int text_width(const std::string &text, Style style, float scale) {
    cairo_text_extents_t te;
    this->set_font(style, scale);
    cairo_text_extents(this->cr, text.c_str(), &te);
    return (int)(te.x_advance);
  }

  void bg(Color color) {
    this->set_source_color(color);
    cairo_rectangle(this->cr, 0, 0, this->w, this->h);
    cairo_fill(this->cr);
  }

  void text(const std::string &text, Color color, int x, int y, Style style,
            float scale) {
    cairo_font_extents_t fe;
    this->set_source_color(color);
    this->set_font(style, scale);
    cairo_font_extents(this->cr, &fe);
    cairo_move_to(this->cr, x, y + fe.ascent);
    cairo_show_text(this->cr, text.c_str());
  }

private:
  void set_font(Style style, float scale) {
    switch (style) {
    case Normal:
      cairo_select_font_face(this->cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                             CAIRO_FONT_WEIGHT_NORMAL);
      break;
    case Strong:
      cairo_select_font_face(this->cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                             CAIRO_FONT_WEIGHT_BOLD);
      break;
    case Header:
      cairo_select_font_face(this->cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                             CAIRO_FONT_WEIGHT_BOLD);
      scale = scale * 1.6f;
      break;
    case Monospace:
      cairo_select_font_face(this->cr, "Monospace", CAIRO_FONT_SLANT_NORMAL,
                             CAIRO_FONT_WEIGHT_NORMAL);
      break;
    }
    cairo_set_font_size(this->cr, 16 * scale);
  }

  void set_source_color(Color color) {
    double r = ((color >> 16) & 0xff) / 255.0;
    double g = ((color >> 8) & 0xff) / 255.0;
    double b = ((color)&0xff) / 255.0;
    double a = ((color >> 24) & 0xff) / 255.0;
    cairo_set_source_rgba(this->cr, r, g, b, a);
  }

  const int w;
  const int h;
  cairo_surface_t *surface;
  cairo_t *cr;
};

} // namespace slide

#endif // SLIDE_HPP
