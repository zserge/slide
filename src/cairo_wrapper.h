#ifndef SLIDE_WCAIRO_H
#define SLIDE_WCAIRO_H

#include "color.h"
#include "styles.h"
#include <cairo/cairo-pdf.h>
#include <cairo/cairo.h>
#include <string>

// cairo wrapper class
namespace slide {
class CairoWrapper {
public:
  static cairo_surface_t *
  CreateSurface(int width, int height,
                cairo_format_t format = CAIRO_FORMAT_ARGB32);

  static cairo_surface_t *CreatePDFSurface(const std::string &filename,
                                           const int width, const int height);

  static cairo_t *Create(cairo_surface_t *target);

  static void SetFont(cairo_t *cr, Style::Style style, float scale);

  static void SetSourceColor(cairo_t *cr, const Color &color);

  static int TextHeight(cairo_t *cr, Style::Style style, float scale);

  static int TextWidth(cairo_t *cr, const std::string &text, Style::Style style,
                       float scale);

  static void Background(cairo_t *cr, int w, int h, const Color &color);

  static void Text(cairo_t *cr, const std::string &text, const Color &color,
                   int x, int y, Style::Style style, float scale);

  static void ShowPage(cairo_t *cr);

  static void DestroySurface(cairo_surface_t *psurf);

  static void Destroy(cairo_t *pcr);

  static void WriteToPng(cairo_surface_t *surface, const char *filename);

  static void WriteToPngStream(cairo_surface_t *surface,
                               cairo_write_func_t write_func, void *closure);
};
} // namespace slide
#endif // SLIDE_WCAIRO_H
