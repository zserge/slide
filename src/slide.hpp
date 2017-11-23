#ifndef SLIDE_HPP
#define SLIDE_HPP

#include <string>
#include <vector>

namespace slide {

//
// Parser
//

enum Style { Normal, Strong, Header, Monospace };

struct Token {
  const long pos;
  const int line;
  const Style style;
  const std::string text;
};
using Slide = std::vector<Token>;
using Deck = std::vector<Slide>;

static Deck parse(const std::string &text) {
  Deck deck{};
  auto lineno = 0;
  auto begin = text.cbegin();
  for (auto it = begin, end = text.cend(); it < end;) {
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
      bool insert_empty_token = true;
      for (; it < end && *it != '\n'; ++it) {
	if (style == Style::Normal && *it == '*') {
	  ++it;
	  if (it < end && *it != ' ' && *it != '*') {
	    std::string em;
	    for (; it < end && *it != '*' && *it != '\n'; ++it) {
	      em.push_back(*it);
	    }
	    if (*it == '*') {
	      if (text.size() > 0) {
		slide.push_back(
		    Token{std::distance(begin, it), lineno, style, text});
	      }
	      slide.push_back(
		  Token{std::distance(begin, it), lineno, Style::Strong, em});
	      insert_empty_token = false;
	      text = "";
	    } else {
	      text.push_back('*');
	      text.append(em);
	    }
	  } else {
	    text.push_back('*');
	    if (*it != '*') {
	      text.push_back(*it);
	    }
	  }
	} else {
	  text.push_back(*it);
	}
      }
      if (insert_empty_token || text.size() > 0) {
	slide.push_back(Token{std::distance(begin, it), lineno, style, text});
      }
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

namespace cairo {

static void set_font(cairo_t *cr, Style style, float scale) {
  switch (style) {
  case Normal:
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
			   CAIRO_FONT_WEIGHT_NORMAL);
    break;
  case Strong:
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
			   CAIRO_FONT_WEIGHT_BOLD);
    break;
  case Header:
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
			   CAIRO_FONT_WEIGHT_BOLD);
    scale = scale * 1.6f;
    break;
  case Monospace:
    cairo_select_font_face(cr, "Monospace", CAIRO_FONT_SLANT_NORMAL,
			   CAIRO_FONT_WEIGHT_NORMAL);
    break;
  }
  cairo_set_font_size(cr, 16 * scale);
}

static void set_source_color(cairo_t *cr, Color color) {
  double r = ((color >> 16) & 0xff) / 255.0;
  double g = ((color >> 8) & 0xff) / 255.0;
  double b = ((color)&0xff) / 255.0;
  double a = ((color >> 24) & 0xff) / 255.0;
  cairo_set_source_rgba(cr, r, g, b, a);
}
static int text_height(cairo_t *cr, Style style, float scale) {
  cairo_font_extents_t fe;
  set_font(cr, style, scale);
  cairo_font_extents(cr, &fe);
  return (int)(fe.height);
}

static int text_width(cairo_t *cr, const std::string &text, Style style,
		      float scale) {
  cairo_text_extents_t te;
  set_font(cr, style, scale);
  cairo_text_extents(cr, text.c_str(), &te);
  return (int)(te.x_advance);
}

static void bg(cairo_t *cr, int w, int h, Color color) {
  set_source_color(cr, color);
  cairo_rectangle(cr, 0, 0, w, h);
  cairo_fill(cr);
}

static void text(cairo_t *cr, const std::string &text, Color color, int x,
		 int y, Style style, float scale) {
  cairo_font_extents_t fe;
  set_source_color(cr, color);
  set_font(cr, style, scale);
  cairo_font_extents(cr, &fe);
  cairo_move_to(cr, x, y + fe.ascent);
  cairo_show_text(cr, text.c_str());
}
} // namespace cairo

namespace base64 {

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
					"abcdefghijklmnopqrstuvwxyz"
					"0123456789+/";
std::string encode(unsigned char const *bytes_to_encode, unsigned int in_len) {
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
	ret += base64_chars[char_array_4[i]];
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
      ret += base64_chars[char_array_4[j]];
    }
    while ((i++ < 3)) {
      ret += '=';
    }
  }
  return ret;
}

} // namespace base64

class PNG : public Page {
public:
  PNG(const int w, const int h) : w(w), h(h) {
    this->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
    this->cr = cairo_create(this->surface);
  }
  ~PNG() {
    cairo_surface_destroy(this->surface);
    cairo_destroy(this->cr);
  }

  int width() const { return this->w; }
  int height() const { return this->h; }
  int text_height(Style style, float scale) {
    return cairo::text_height(this->cr, style, scale);
  }
  int text_width(const std::string &text, Style style, float scale) {
    return cairo::text_width(this->cr, text, style, scale);
  }
  void bg(Color color) { cairo::bg(this->cr, this->w, this->h, color); }
  void text(const std::string &text, Color color, int x, int y, Style style,
	    float scale) {
    cairo::text(cr, text, color, x, y, style, scale);
  }

  void save(const std::string filename) {
    cairo_surface_write_to_png(this->surface, filename.c_str());
  }

  std::string dataURI() {
    std::string b64 = "data:image/png;base64,";
    cairo_surface_write_to_png_stream(
	this->surface,
	[](void *closure, const unsigned char *data, unsigned int length) {
	  std::string *b64 = (std::string *)closure;
	  b64->append(base64::encode(data, length));
	  return CAIRO_STATUS_SUCCESS;
	},
	&b64);
    return b64;
  }

private:
  const std::string name;
  const int w;
  const int h;
  cairo_surface_t *surface;
  cairo_t *cr;
};

class PDF : public Document {
public:
  PDF(const std::string name, const int w, const int h) : w(w), h(h) {
    this->surface = cairo_pdf_surface_create(name.c_str(), w, h);
    this->cr = cairo_create(this->surface);
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
    return cairo::text_height(this->cr, style, scale);
  }
  int text_width(const std::string &text, Style style, float scale) {
    return cairo::text_width(this->cr, text, style, scale);
  }
  void bg(Color color) { cairo::bg(this->cr, this->w, this->h, color); }
  void text(const std::string &text, Color color, int x, int y, Style style,
	    float scale) {
    cairo::text(cr, text, color, x, y, style, scale);
  }

private:
  const int w;
  const int h;
  cairo_surface_t *surface;
  cairo_t *cr;
};

} // namespace slide

#endif // SLIDE_HPP
