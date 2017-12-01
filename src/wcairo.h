#ifndef SLIDE_WCAIRO_H
#define SLIDE_WCAIRO_H

#include "colour_t.h"
#include "styles.h"
#include <cairo/cairo-pdf.h>
#include <cairo/cairo.h>
#include <string>

// cairo wrapper class
class wcairo {
public:
  static cairo_surface_t *
  create_surface(int width, int height,
                 cairo_format_t format = CAIRO_FORMAT_ARGB32);
  static cairo_t *create(cairo_surface_t *target);
  static void set_font(cairo_t *cr, slide::Style style, float scale);
  static void set_source_color(cairo_t *cr, const colour_t &color);
  static int text_height(cairo_t *cr, slide::Style style, float scale);
  static int text_width(cairo_t *cr, const std::string &text,
                        slide::Style style, float scale);
  static void bg(cairo_t *cr, int w, int h, const colour_t &color);
  static void text(cairo_t *cr, const std::string &text, const colour_t &color,
                   int x, int y, slide::Style style, float scale);
  static void show_page(cairo_t *cr);
  static void destroy_surface(cairo_surface_t *psurf);
  static void destroy(cairo_t *pcr);
  static void write_to_png(cairo_surface_t *surface, const char *filename);
  static void write_to_png_stream(cairo_surface_t *surface,
                                  cairo_write_func_t write_func, void *closure);
};

#endif // SLIDE_WCAIRO_H
