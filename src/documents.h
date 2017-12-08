#ifndef SLIDE_DOCUMENTS_H
#define SLIDE_DOCUMENTS_H

#include "cairo_wrapper.h"
#include "dimenions.h"
#include "styles.h"

namespace slide {

// abstract
class Page {
public:
  Page(const int w, const int h, cairo_surface_t *const psurf,
       cairo_t *const pcr, const std::string &name);

  Page(const int w, const int h, const std::string &name = "Name Not Set");

  Page(void);

  virtual ~Page(void);

  virtual const Dimensions &Size(void) { return size_; }

  virtual int TextHeight(Style::Style style, float scale) = 0;

  virtual int TextWidth(const std::string &text, Style::Style style,
                        float scale) = 0;

  virtual void Background(const Color &color) = 0;

  virtual void Text(const std::string &text, const Color &color, int x, int y,
					Style::Style style, float scale) = 0;

protected:
  const Dimensions size_;
  cairo_surface_t *surface_;
  cairo_t *cr_;
  const std::string name_;

  virtual void InitialiseContext(void) = 0;
  virtual void EnsureInitialised(void);
};

// Abstract
class Document : public Page {
public:
  Document(const int w, const int h, cairo_surface_t *const psurf,
           cairo_t *const pcr, const std::string &name);

  Document(const int w, const int h, const std::string &name);

  virtual ~Document(void) { /* Intentionally Blank */
  }

  virtual void BeginPage(void) = 0;

  virtual void EndPage(void) = 0;
};

class PNG : public Page {
public:
  PNG(const int w, const int h);

  PNG(const int w, const int h, const std::string &name);

  ~PNG(void);

  int TextHeight(Style::Style style, float scale);

  int TextWidth(const std::string &text, Style::Style style, float scale);

  void Background(const Color &color);

  void Text(const std::string &text, const Color &color, int x, int y,
			Style::Style style, float scale);

  void Save(const std::string filename);

  std::string DataUri(void);

protected:
  void InitialiseContext(void);
};

class PDF : public Document {
public:
  PDF(const std::string name, const int w, const int h);

  ~PDF(void);

  void BeginPage(void);

  void EndPage(void);

  int TextHeight(Style::Style style, float scale);

  int TextWidth(const std::string &text, Style::Style style, float scale);

  void Background(const Color &color);

  void Text(const std::string &text, const Color &color, int x, int y,
			Style::Style style, float scale);

protected:
  void InitialiseContext(void);
};
} // namespace slide

#endif // SLIDE_DOCUMENTS_H
