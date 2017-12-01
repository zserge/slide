#include "WCairo.h"
#include <iostream>

void WCairo::SetFont(cairo_t *cr, slide::Style style, float scale) {
  switch (style) {
  case slide::Normal:
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    break;
  case slide::Strong:
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    break;
  case slide::Header:
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    scale = scale * 1.6f;
    break;
  case slide::Monospace:
    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    break;
  }
  cairo_set_font_size(cr, 16 * scale);
}
void WCairo::SetSourceColor(cairo_t *cr, const Color &color) {
  cairo_set_source_rgba(cr, color.red(), color.green(), color.blue(),
                        color.alpha());
}
int WCairo::TextHeight(cairo_t *cr, slide::Style style, float scale) {
  cairo_font_extents_t fe;
  SetFont(cr, style, scale);
  cairo_font_extents(cr, &fe);
  return (int)(fe.height);
}
int WCairo::TextWidth(cairo_t *cr, const std::string &text, slide::Style style,
                      float scale) {
  cairo_text_extents_t te;
  SetFont(cr, style, scale);
  cairo_text_extents(cr, text.c_str(), &te);
  return (int)(te.x_advance);
}

void WCairo::Background(cairo_t *cr, int w, int h, const Color &color) {
  SetSourceColor(cr, color);
  cairo_rectangle(cr, 0, 0, w, h);
  cairo_fill(cr);
}
void WCairo::Text(cairo_t *cr, const std::string &text, const Color &color,
                  int x, int y, slide::Style style, float scale) {
  cairo_font_extents_t fe;
  SetSourceColor(cr, color);
  SetFont(cr, style, scale);
  cairo_font_extents(cr, &fe);
  cairo_move_to(cr, x, y + fe.ascent);
  cairo_show_text(cr, text.c_str());
}

void WCairo::ShowPage(cairo_t *cr) { cairo_show_page(cr); }

void WCairo::DestroySurface(cairo_surface_t *psurf) {
  std::cerr << "Attempting to destroy surface at " << psurf << std::endl;
  if (nullptr != psurf) {
    cairo_surface_destroy(psurf);
  }
}

void WCairo::Destroy(cairo_t *pcr) {
  std::cerr << "Attempting to destroy cairo at " << pcr << std::endl;
  if (nullptr != pcr) {
    cairo_destroy(pcr);
  }
}

cairo_surface_t *WCairo::CreateSurface(int width, int height,
                                       cairo_format_t format) {
  cairo_surface_t *rc = cairo_image_surface_create(format, width, height);
  std::cerr << "Surface created at " << rc << std::endl;
  return rc;
}

cairo_t *WCairo::Create(cairo_surface_t *target) {
  cairo_t *rc = cairo_create(target);
  std::cerr << "Cairo created at " << rc << std::endl;
  return rc;
}

void WCairo::WriteToPng(cairo_surface_t *surface, const char *filename) {
  auto rc = cairo_surface_write_to_png(surface, filename);
  std::cerr << "cairo_surface_write_to_png returned " << rc << std::endl;
}

void WCairo::WriteToPngStream(cairo_surface_t *surface,
                              cairo_write_func_t write_func, void *closure) {
  auto rc = cairo_surface_write_to_png_stream(surface, write_func, closure);
  std::cerr << "cairo_surface_write_to_png_stream returned " << rc << std::endl;
}