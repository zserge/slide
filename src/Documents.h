#ifndef SLIDE_DOCUMENTS_H
#define SLIDE_DOCUMENTS_H

#include "Dimenions.h"
#include "Styles.h"
#include "WCairo.h"

// Abstract
class Page {
public:
  Page(const int w, const int h, cairo_surface_t *const psurf,
       cairo_t *const pcr, const std::string &name);
  Page(const int w, const int h, const std::string &name);
  Page(void);

  virtual ~Page(void);
  virtual const Dimensions &size(void) { return size_; }
  virtual int TextHeight(slide::Style style, float scale) = 0;
  virtual int TextWidth(const std::string &text, slide::Style style,
                        float scale) = 0;
  virtual void Background(const Color &color) = 0;
  virtual void Text(const std::string &text, const Color &color, int x, int y,
                    slide::Style style, float scale) = 0;

protected:
  const Dimensions size_;
  cairo_surface_t *surface_;
  cairo_t *cr_;
  const std::string name_;
};

// Abstract
class Document : public Page {
public:
  Document(const int w, const int h, cairo_surface_t *const psurf,
           cairo_t *const pcr, const std::string &name);
  Document(const int w, const int h, const std::string &name);

  virtual void BeginPage(void) = 0;
  virtual void EndPage(void) = 0;
};

class PNG : public Page {
public:
  PNG(const int w, const int h);
  PNG(const int w, const int h, const std::string &name);
  ~PNG(void);

  int TextHeight(slide::Style style, float scale);
  int TextWidth(const std::string &text, slide::Style style, float scale);
  void Background(const Color &color);
  void Text(const std::string &text, const Color &color, int x, int y,
            slide::Style style, float scale);
  void Save(const std::string filename);
  std::string DataUri(void);
};

class PDF : public Document {
public:
  PDF(const std::string name, const int w, const int h);
  ~PDF(void);

  void BeginPage(void);
  void EndPage(void);

  int TextHeight(slide::Style style, float scale);
  int TextWidth(const std::string &text, slide::Style style, float scale);
  void Background(const Color &color);
  void Text(const std::string &text, const Color &color, int x, int y,
            slide::Style style, float scale);
};

#endif // SLIDE_DOCUMENTS_H
