
#ifndef SLIDE_DIMENIONS_H
#define SLIDE_DIMENIONS_H

#include <ostream>
namespace slide {
class Dimensions {
public:
  Dimensions(const int w, const int h) : w_(w), h_(h) {}

  int &Width(void) { return w_; }

  int &Height(void) { return h_; }

  int Width(void) const { return w_; }

  int Height(void) const { return h_; }

	friend std::ostream&operator << (std::ostream& out, const Dimensions& d){
		out << "(w_: " << d.w_ << ", h_: " << d.h_ << ")";
		return out;
	}

private:
  int w_;
  int h_;
};
} // namespace slide

#endif // SLIDE_DIMENIONS_H
