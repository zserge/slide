
#ifndef SLIDE_DIMENIONS_H
#define SLIDE_DIMENIONS_H

class Dimensions {
public:
  Dimensions(const int w, const int h) : w_(w), h_(h) {}

  int &width(void) { return w_; }
  int &height(void) { return h_; }

  int width(void) const { return w_; }
  int height(void) const { return h_; }

private:
  int w_;
  int h_;
};

#endif // SLIDE_DIMENIONS_H
