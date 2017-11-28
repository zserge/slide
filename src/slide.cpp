#include "slide.hpp"

const std::string slide::base64::base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string slide::base64::encode(unsigned char const *bytes_to_encode, unsigned int in_len, bool url) {
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
				char c = base64_chars[char_array_4[i]];
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
			char c = base64_chars[char_array_4[i]];
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

slide::Page_b::Page_b(const int w, const int h)
	: w(w), h(h) {
	surface = 	cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
	cr = 		cairo_create(surface);
}
slide::Page_b::~Page_b(void){
	if(surface)	{ cairo_surface_destroy(surface); }
	if(cr) 		{ cairo_destroy(cr); }
}

slide::PNG::PNG(const int w, const int h) : Page_b(w, h)
{ /* Intentionally Blank */}

slide::PNG::~PNG(void) { /* Intentionally Blank */ }

int slide::PNG::width() const {
	return this->w;
}

int slide::PNG::height() const {
	return this->h;
}

int slide::PNG::text_height(Style style, float scale) {
	return cairo::text_height(this->cr, style, scale);
}

int slide::PNG::text_width(const std::string &text, Style style, float scale) {
	return cairo::text_width(this->cr, text, style, scale);
}

void slide::PNG::bg(const colour_t& color) {
	cairo::bg(this->cr, this->w, this->h, color);
}

void slide::PNG::text(const std::string &text, const colour_t& color, int x, int y, Style style, float scale) {
	cairo::text(cr, text, color, x, y, style, scale);
}

void slide::PNG::save(const std::string filename) {
	cairo_surface_write_to_png(this->surface, filename.c_str());
}

std::string slide::PNG::dataURI(void) {
	std::vector<unsigned char> raw;
	cairo_surface_write_to_png_stream(
			this->surface,
			[](void *closure, const unsigned char *data, unsigned int length) {
				std::vector<unsigned char> *raw =
						(std::vector<unsigned char> *)(closure);
				for (unsigned int i = 0; i < length; i++) {
					raw->push_back(data[i]);
				}
				return CAIRO_STATUS_SUCCESS;
			},
			&raw);
	return "data:image/png;base64," + base64::encode(raw.data(), raw.size(), true);
}



slide::PDF::PDF(const std::string name, const int w, const int h) : Document_b(w, h)
{ /* Intentionally Blank */ }

slide::PDF::~PDF(void) {
	cairo_surface_destroy(this->surface);
	cairo_destroy(this->cr);
}

void slide::PDF::begin_page(void) {}
void slide::PDF::end_page(void) { cairo_show_page(this->cr); }

int slide::PDF::width(void) const { return this->w; }
int slide::PDF::height(void) const { return this->h; }
int slide::PDF::text_height(Style style, float scale) {
	return cairo::text_height(this->cr, style, scale);
}
int slide::PDF::text_width(const std::string &text, Style style, float scale) {
	return cairo::text_width(this->cr, text, style, scale);
}
void slide::PDF::bg(const colour_t& color) { cairo::bg(this->cr, this->w, this->h, color); }
void slide::PDF::text(const std::string &text, const colour_t& color, int x, int y, Style style,
					  float scale) {
	cairo::text(cr, text, color, x, y, style, scale);
}


void slide::cairo::set_font(cairo_t *cr, Style style, float scale) {
	switch (style) {
		case Normal:
			cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
								   CAIRO_FONT_WEIGHT_NORMAL);
			break;
		case Strong:
			cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
								   CAIRO_FONT_WEIGHT_BOLD);
			break;
		case Header:
			cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
								   CAIRO_FONT_WEIGHT_BOLD);
			scale = scale * 1.6f;
			break;
		case Monospace:
			cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL,
								   CAIRO_FONT_WEIGHT_NORMAL);
			break;
	}
	cairo_set_font_size(cr, 16 * scale);
}
void slide::cairo::set_source_color(cairo_t *cr, const colour_t& color) {
	cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(), color.alpha());
}
int slide::cairo::text_height(cairo_t *cr, Style style, float scale) {
	cairo_font_extents_t fe;
	set_font(cr, style, scale);
	cairo_font_extents(cr, &fe);
	return (int)(fe.height);
}
int slide::cairo::text_width(cairo_t *cr, const std::string &text, Style style,
							 float scale) {
	cairo_text_extents_t te;
	set_font(cr, style, scale);
	cairo_text_extents(cr, text.c_str(), &te);
	return (int)(te.x_advance);
}

void slide::cairo::bg(cairo_t *cr, int w, int h, const colour_t& color) {
	set_source_color(cr, color);
	cairo_rectangle(cr, 0, 0, w, h);
	cairo_fill(cr);
}
void slide::cairo::text(cairo_t *cr, const std::string &text, const colour_t& color, int x,
						int y, Style style, float scale) {
	cairo_font_extents_t fe;
	set_source_color(cr, color);
	set_font(cr, style, scale);
	cairo_font_extents(cr, &fe);
	cairo_move_to(cr, x, y + fe.ascent);
	cairo_show_text(cr, text.c_str());
}

std::pair<int, int> slide::render_scale(Page_b &p, Slide &slide, const colour_t& color,
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

void slide::render(Page_b &page, Slide &slide, const colour_t& fg, const colour_t& bg) {
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

slide::Deck slide::parse(const std::string &text) {
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