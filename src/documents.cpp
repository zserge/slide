#include "documents.h"
#include "encoding.h"
#include <iostream>
#include <vector>

Page_b::Page_b(const int w, const int h, const std::string &name)
    : _size(w, h), _name(name) {
  std::cerr << "Page_b::ctor creating _surface and _cr" << std::endl;
  _surface = wcairo::create_surface(w, h);
  _cr = wcairo::create(_surface);
}

Page_b::~Page_b(void) {
  std::cerr << "Page_b::dtor destroying _surface and _cr" << std::endl;
  wcairo::destroy_surface(_surface);
  wcairo::destroy(_cr);
}

Document_b::Document_b(const int w, const int h, cairo_surface_t *const psurf,
                       cairo_t *const pcr, const std::string &name)
    : Page_b(w, h, psurf, pcr, name) { /* intentionally blank */
}

Document_b::Document_b(const int w, const int h, const std::string &name)
    : Page_b(w, h, name) { /* intentionally blank */
}

Page_b::Page_b(const int w, const int h, cairo_surface_t *const psurf,
               cairo_t *const pcr, const std::string &name)
    : _size(w, h), _surface(psurf), _cr(pcr),
      _name(name) { /* intentionally blank */
}

Page_b::Page_b(void)
    : _size(0, 0), _surface(0), _cr(0),
      _name("No Name Set") { /* intentionally blank */
}

PNG::PNG(const int w, const int h)
    : Page_b(w, h, "No Name Set") { /* Intentionally Blank */
}

PNG::PNG(const int w, const int h, const std::string &name)
    : Page_b(w, h, name) { /* Intentionally Blank */
}

PNG::~PNG(void) { /* Intentionally Blank */
}

int PNG::text_height(slide::Style style, float scale) {
  return wcairo::text_height(_cr, style, scale);
}

int PNG::text_width(const std::string &text, slide::Style style, float scale) {
  return wcairo::text_width(_cr, text, style, scale);
}

void PNG::bg(const colour_t &color) {
  wcairo::bg(_cr, _size.width(), _size.width(), color);
}

void PNG::text(const std::string &text, const colour_t &color, int x, int y,
               slide::Style style, float scale) {
  wcairo::text(_cr, text, color, x, y, style, scale);
}

void PNG::save(const std::string filename) {
  wcairo::write_to_png(_surface, filename.c_str());
}

std::string PNG::dataURI(void) {
  std::vector<unsigned char> raw;
  wcairo::write_to_png_stream(
      _surface,
      [](void *closure, const unsigned char *data, unsigned int length) {
        std::vector<unsigned char> *raw =
            (std::vector<unsigned char> *)(closure);
        for (unsigned int i = 0; i < length; i++) {
          raw->push_back(data[i]);
        }
        return CAIRO_STATUS_SUCCESS;
      },
      &raw);
  return "data:image/png;base64," +
         base64::encode(raw.data(), raw.size(), true);
}

PDF::PDF(const std::string name, const int w, const int h)
    : Document_b(w, h, name) { /* Intentionally Blank */
}

PDF::~PDF(void) {
  // Intentionally blank
}

void PDF::begin_page(void) {
  // intentionally blank
}

void PDF::end_page(void) { wcairo::show_page(_cr); }

int PDF::text_height(slide::Style style, float scale) {
  return wcairo::text_height(_cr, style, scale);
}

int PDF::text_width(const std::string &text, slide::Style style, float scale) {
  return wcairo::text_width(_cr, text, style, scale);
}

void PDF::bg(const colour_t &color) {
  wcairo::bg(_cr, _size.width(), _size.height(), color);
}

void PDF::text(const std::string &text, const colour_t &color, int x, int y,
               slide::Style style, float scale) {
  wcairo::text(_cr, text, color, x, y, style, scale);
}