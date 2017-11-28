#ifndef SLIDE_HPP
#define SLIDE_HPP

#include <string>
#include <vector>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo.h>
#include "colour_t.h"
#include "dimenions.h"

namespace slide {

	enum Style {
		Normal,
		Strong,
		Header,
		Monospace
	};

	struct Token {
		const long 			pos;
		const int 			line;
		const Style 		style;
		const std::string 	text;
	};

	using Slide = std::vector<Token>;
	using Deck 	= std::vector<Slide>;

	class base64 {
	private:
		static const std::string base64_chars;

	public:
		static std::string encode(unsigned char const *bytes_to_encode, unsigned int in_len, bool url);

	};

	class Page_b {
	public:
		Page_b(const int w, const int h, cairo_surface_t * const psurf, cairo_t * const pcr, const std::string& name )
				: _size(w,h), _surface(psurf), _cr(pcr), _name(name)
		{/* intentionally blank */}

		Page_b(const int w, const int h, const std::string& name);

		Page_b(void) : _size(0, 0), _surface(0), _cr(0), _name("No Name Set")
		{/* intentionally blank */}

		virtual ~Page_b(void);
		virtual const dimensions& size(void){ return _size; }
		virtual int 	text_height(Style style, float scale) 															= 0;
		virtual int 	text_width(const std::string &text, Style style, float scale) 									= 0;
		virtual void 	bg(const colour_t& color) 																		= 0;
		virtual void 	text(const std::string &text, const colour_t& color, int x, int y, Style style, float scale) 	= 0;
	protected:
		const dimensions	_size;
		cairo_surface_t 	*_surface;
		cairo_t 			*_cr;
		const std::string 	_name;
	};

	class Document_b : public Page_b {
	public:
		Document_b(const int w, const int h, cairo_surface_t * const psurf, cairo_t * const pcr, const std::string& name) : Page_b(w, h, psurf, pcr, name)
		{/* intentionally blank */}

		Document_b(const int w, const int h, const std::string& name): Page_b(w, h, name)
		{/* intentionally blank */}

		//Document_b(void) : Page_b()
		//{ /* Intentionally Blank */ }

		virtual void begin_page(void) 																					= 0;
		virtual void end_page(void) 																					= 0;
	};

	class cairo {
	public:
		static void set_font(cairo_t *cr, Style style, float scale);
		static void set_source_color(cairo_t *cr, const colour_t& color);
		static int 	text_height(cairo_t *cr, Style style, float scale);
		static int 	text_width(cairo_t *cr, const std::string &text, Style style, float scale);
		static void bg(cairo_t *cr, int w, int h, const colour_t& color);
		static void text(cairo_t *cr, const std::string &text, const colour_t& color, int x, int y, Style style, float scale);
	};

	class PNG : public Page_b {
	public:
		PNG(const int w, const int h);
		PNG(const int w, const int h, const std::string& name);
		~PNG(void);
		int 	text_height(Style style, float scale);
		int 	text_width(const std::string &text, Style style, float scale);
		void 	bg(const colour_t& color);
		void 	text(const std::string &text, const colour_t& color, int x, int y, Style style, float scale);
		void 	save(const std::string filename);
		std::string dataURI(void);

	};

	class PDF : public Document_b {
	public:
		PDF(const std::string name, const int w, const int h);
		~PDF(void);

		void begin_page(void);
		void end_page(void);

		int 	text_height(Style style, float scale);
		int 	text_width(const std::string &text, Style style, float scale);
		void 	bg(const colour_t& color);
		void 	text(const std::string &text, const colour_t& color, int x, int y, Style style, float scale);
	};

	// Static functions only
	Deck 				parse(const std::string &text);
	std::pair<int, int> render_scale(Page_b &p, Slide &slide, const colour_t& color, int xoff, int yoff, float scale);
	void 				render(Page_b &page, Slide &slide, const colour_t& fg, const colour_t& bg);

} // namespace slide

#endif // SLIDE_HPP
