#include "Documents.h"
#include "Encoding.h"
#include <iostream>
#include <vector>

namespace slide {
    Page::Page(const int w, const int h, const std::string &name)
            : size_(w, h), name_(name) {
      std::cerr << "Page_b::ctor creating _surface and _cr" << std::endl;
      surface_ = WCairo::CreateSurface(w, h);
      cr_ = WCairo::Create(surface_);
    }

    Page::~Page(void) {
      std::cerr << "Page_b::dtor destroying _surface and _cr" << std::endl;
      WCairo::DestroySurface(surface_);
      WCairo::Destroy(cr_);
    }

    Document::Document(const int w, const int h, cairo_surface_t *const psurf,
                       cairo_t *const pcr, const std::string &name)
            : Page(w, h, psurf, pcr, name) { /* intentionally blank */
    }

    Document::Document(const int w, const int h, const std::string &name)
            : Page(w, h, name) { /* intentionally blank */
    }

    Page::Page(const int w, const int h, cairo_surface_t *const psurf,
               cairo_t *const pcr, const std::string &name)
            : size_(w, h), surface_(psurf), cr_(pcr),
              name_(name) { /* intentionally blank */
    }

    Page::Page(void)
            : size_(0, 0), surface_(0), cr_(0),
              name_("No Name Set") { /* intentionally blank */
    }

    PNG::PNG(const int w, const int h)
            : Page(w, h, "No Name Set") { /* Intentionally Blank */
    }

    PNG::PNG(const int w, const int h, const std::string &name)
            : Page(w, h, name) { /* Intentionally Blank */
    }

    PNG::~PNG(void) { /* Intentionally Blank */
    }

    int PNG::TextHeight(slide::Style style, float scale) {
      return WCairo::TextHeight(cr_, style, scale);
    }

    int PNG::TextWidth(const std::string &text, slide::Style style, float scale) {
      return WCairo::TextWidth(cr_, text, style, scale);
    }

    void PNG::Background(const Color &color) {
      WCairo::Background(cr_, size_.width(), size_.width(), color);
    }

    void PNG::Text(const std::string &text, const Color &color, int x, int y,
                   slide::Style style, float scale) {
      WCairo::Text(cr_, text, color, x, y, style, scale);
    }

    void PNG::Save(const std::string filename) {
      WCairo::WriteToPng(surface_, filename.c_str());
    }

    std::string PNG::DataUri(void) {
      std::vector<unsigned char> raw;
      WCairo::WriteToPngStream(
              surface_,
              [](void *closure, const unsigned char *data, unsigned int length) {
                  std::vector<unsigned char> *raw =
                          ( std::vector<unsigned char> * ) ( closure );
                  for( unsigned int i = 0; i < length; i++ ) {
                    raw->push_back(data[i]);
                  }
                  return CAIRO_STATUS_SUCCESS;
              },
              &raw);
      return "data:image/png;base64," +
             Base64::Encode(raw.data(), raw.size(), true);
    }

    PDF::PDF(const std::string name, const int w, const int h)
            : Document(w, h, name) { /* Intentionally Blank */
    }

    PDF::~PDF(void) {
      // Intentionally blank
    }

    void PDF::BeginPage(void) {
      // intentionally blank
    }

    void PDF::EndPage(void) { WCairo::ShowPage(cr_); }

    int PDF::TextHeight(slide::Style style, float scale) {
      return WCairo::TextHeight(cr_, style, scale);
    }

    int PDF::TextWidth(const std::string &text, slide::Style style, float scale) {
      return WCairo::TextWidth(cr_, text, style, scale);
    }

    void PDF::Background(const Color &color) {
      WCairo::Background(cr_, size_.width(), size_.height(), color);
    }

    void PDF::Text(const std::string &text, const Color &color, int x, int y,
                   slide::Style style, float scale) {
      WCairo::Text(cr_, text, color, x, y, style, scale);
    }
}//namespace slide