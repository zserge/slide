#include "documents.h"
#include "base64.h"
#include <iostream>
#include <vector>

#define print(a) /* std::cerr << a << std::endl; */
namespace slide {
Page::Page(const int w, const int h, const std::string &name)
    : size_(w, h), name_(name), surface_(nullptr), cr_(nullptr) {}

Page::~Page(void) {
  print("Page_b::dtor destroying _surface and _cr");
  CairoWrapper::DestroySurface(surface_);
  CairoWrapper::Destroy(cr_);
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

void Page::EnsureInitialised(void) {
  if (cr_ == nullptr || surface_ == nullptr) {
    InitialiseContext();
  }
}

PNG::PNG(const int w, const int h)
    : Page(w, h, "No Name Set") { /* Intentionally Blank */
}

PNG::PNG(const int w, const int h, const std::string &name)
    : Page(w, h, name) { /* Intentionally Blank */
}

PNG::~PNG(void) { /* Intentionally Blank */
}

int PNG::TextHeight(Style::Style style, float scale) {
  EnsureInitialised();
  return CairoWrapper::TextHeight(cr_, style, scale);
}

int PNG::TextWidth(const std::string &text, Style::Style style, float scale) {
  EnsureInitialised();
  return CairoWrapper::TextWidth(cr_, text, style, scale);
}

void PNG::Background(const Color &color) {
  EnsureInitialised();
  CairoWrapper::Background(cr_, size_.Width(), size_.Width(), color);
}

void PNG::Text(const std::string &text, const Color &color, int x, int y,
			   Style::Style style, float scale) {
  EnsureInitialised();
  CairoWrapper::Text(cr_, text, color, x, y, style, scale);
}

void PNG::Save(const std::string filename) {
  EnsureInitialised();
  CairoWrapper::WriteToPng(surface_, filename.c_str());
}

std::string PNG::DataUri(void) {
  EnsureInitialised();
  std::vector<unsigned char> raw;
  CairoWrapper::WriteToPngStream(
      surface_,
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
         Base64::Encode(raw.data(), raw.size(), true);
}

void PNG::InitialiseContext(void) {
  //	std::cerr << "PNG::ctor creating surface_ and cr_" << std::endl;
  surface_ = CairoWrapper::CreateSurface(size_.Width(), size_.Height());
  cr_ = CairoWrapper::Create(surface_);
}

PDF::PDF(const std::string name, const int w, const int h)
    : Document(w, h, name) {
  /* Intentionally Blank */
}

PDF::~PDF(void) {
  // Intentionally blank
}

void PDF::BeginPage(void) {
  // intentionally blank
}

void PDF::EndPage(void) { CairoWrapper::ShowPage(cr_); }

int PDF::TextHeight(Style::Style style, float scale) {
  EnsureInitialised();
  return CairoWrapper::TextHeight(cr_, style, scale);
}

int PDF::TextWidth(const std::string &text, Style::Style style, float scale) {
  EnsureInitialised();
  return CairoWrapper::TextWidth(cr_, text, style, scale);
}

void PDF::Background(const Color &color) {
  EnsureInitialised();
  CairoWrapper::Background(cr_, size_.Width(), size_.Height(), color);
}

void PDF::Text(const std::string &text, const Color &color, int x, int y,
			   Style::Style style, float scale) {
  EnsureInitialised();
  CairoWrapper::Text(cr_, text, color, x, y, style, scale);
}

void PDF::InitialiseContext(void) {
  print("PDF::ctor creating surface_ and cr_");
  surface_ =
      CairoWrapper::CreatePDFSurface(name_, size_.Width(), size_.Height());
  cr_ = CairoWrapper::Create(surface_);
}
} // namespace slide