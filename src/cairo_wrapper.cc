#include "cairo_wrapper.h"
#include <iostream>

#define print(a) /* std::cerr << a << std::endl */

namespace slide {
void CairoWrapper::SetFont(cairo_t *cr, Style::Style style, float scale) {
  switch (style) {
  case Style::Normal:
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    break;
  case Style::Strong:
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    break;
  case Style::Header:
    cairo_select_font_face(cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_BOLD);
    scale = scale * 1.6f;
    break;
  case Style::Monospace:
    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    break;
  }
  cairo_set_font_size(cr, 16 * scale);
}

void CairoWrapper::SetSourceColor(cairo_t *cr, const Color &color) {
  cairo_set_source_rgba(cr, color.Red(), color.Green(), color.Blue(),
                        color.Alpha());
}

int CairoWrapper::TextHeight(cairo_t *cr, Style::Style style, float scale) {
  cairo_font_extents_t fe = {};
  SetFont(cr, style, scale);
  cairo_font_extents(cr, &fe);
  return (int)(fe.height);
}

int CairoWrapper::TextWidth(cairo_t *cr, const std::string &text,
                            Style::Style style, float scale) {
  cairo_text_extents_t te = {};
  SetFont(cr, style, scale);
  cairo_text_extents(cr, text.c_str(), &te);
  return (int)(te.x_advance);
}

void CairoWrapper::Background(cairo_t *cr, int w, int h, const Color &color) {
  SetSourceColor(cr, color);
  cairo_rectangle(cr, 0, 0, w, h);
  cairo_fill(cr);
}

void CairoWrapper::Text(cairo_t *cr, const std::string &text,
                        const Color &color, int x, int y, Style::Style style,
                        float scale) {
  cairo_font_extents_t fe = {};
  SetSourceColor(cr, color);
  SetFont(cr, style, scale);
  cairo_font_extents(cr, &fe);
  cairo_move_to(cr, x, y + fe.ascent);
  cairo_show_text(cr, text.c_str());
}

void CairoWrapper::ShowPage(cairo_t *cr) { cairo_show_page(cr); }

void CairoWrapper::DestroySurface(cairo_surface_t *psurf) {
  print("Attempting to destroy surface at " << psurf);
  if (nullptr != psurf) {
    cairo_surface_destroy(psurf);
  }
}

void CairoWrapper::Destroy(cairo_t *pcr) {
  print("Attempting to destroy cairo at " << pcr);
  if (nullptr != pcr) {
    cairo_destroy(pcr);
  }
}

cairo_surface_t *CairoWrapper::CreateSurface(int width, int height,
                                             cairo_format_t format) {
  cairo_surface_t *rc = cairo_image_surface_create(format, width, height);
  print("Surface created at " << rc);
  return rc;
}

cairo_t *CairoWrapper::Create(cairo_surface_t *target) {
  cairo_t *rc = cairo_create(target);
  print("Cairo created at " << rc);
  return rc;
}

void CairoWrapper::WriteToPng(cairo_surface_t *surface, const char *filename) {
  auto rc = cairo_surface_write_to_png(surface, filename);
  print("cairo_surface_write_to_png returned " << rc);
}

void CairoWrapper::WriteToPngStream(cairo_surface_t *surface,
                                    cairo_write_func_t write_func,
                                    void *closure) {
  auto rc = cairo_surface_write_to_png_stream(surface, write_func, closure);
  print("cairo_surface_write_to_png_stream returned " << rc);
}

cairo_surface_t *CairoWrapper::CreatePDFSurface(const std::string &filename,
                                                const int width,
                                                const int height) {
  auto rc = cairo_pdf_surface_create(filename.c_str(), width, height);
  print("cairo_pdf_surface_create returned " << rc);
  return rc;
}
} // namespace slide