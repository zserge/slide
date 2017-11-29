#ifndef SLIDE_DOCUMENTS_H
#define SLIDE_DOCUMENTS_H

#include "wcairo.h"
#include "dimenions.h"
#include "styles.h"

// Abstract
class Page_b {
public:
	Page_b(const int w, const int h, cairo_surface_t * const psurf, cairo_t * const pcr, const std::string& name );
	Page_b(const int w, const int h, const std::string& name);
	Page_b(void);

	virtual ~Page_b(void);
	virtual const dimensions& size(void){ return _size; }
	virtual int 	text_height(slide::Style style, float scale) 														= 0;
	virtual int 	text_width(const std::string &text, slide::Style style, float scale) 								= 0;
	virtual void 	bg(const colour_t& color) 																			= 0;
	virtual void 	text(const std::string &text, const colour_t& color, int x, int y, slide::Style style, float scale) = 0;
protected:
	const dimensions	_size;
	cairo_surface_t 	*_surface;
	cairo_t 			*_cr;
	const std::string 	_name;
};

// Abstract
class Document_b : public Page_b {
public:
	Document_b(const int w, const int h, cairo_surface_t * const psurf, cairo_t * const pcr, const std::string& name);
	Document_b(const int w, const int h, const std::string& name);

	virtual void begin_page(void) 																					= 0;
	virtual void end_page(void) 																					= 0;
};


class PNG : public Page_b {
public:
	PNG(const int w, const int h);
	PNG(const int w, const int h, const std::string& name);
	~PNG(void);

	int 	text_height(slide::Style style, float scale);
	int 	text_width(const std::string &text, slide::Style style, float scale);
	void 	bg(const colour_t& color);
	void 	text(const std::string &text, const colour_t& color, int x, int y, slide::Style style, float scale);
	void 	save(const std::string filename);
	std::string dataURI(void);

};

class PDF : public Document_b {
public:
	PDF(const std::string name, const int w, const int h);
	~PDF(void);

	void begin_page(void);
	void end_page(void);

	int 	text_height(slide::Style style, float scale);
	int 	text_width(const std::string &text, slide::Style style, float scale);
	void 	bg(const colour_t& color);
	void 	text(const std::string &text, const colour_t& color, int x, int y, slide::Style style, float scale);
};


#endif //SLIDE_DOCUMENTS_H
