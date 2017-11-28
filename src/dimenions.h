//
// Created by Patrick Mintram on 28/11/2017.
//

#ifndef SLIDE_DIMENIONS_H
#define SLIDE_DIMENIONS_H


class dimensions{
public:
	dimensions(const int w, const int h)
			:_w(w), _h(h){}

	int &width(void){ return _w; }
	int &height(void){return _h;}

	int width(void) const { return _w; }
	int height(void) const { return _h; }

private:
	int _w;
	int _h;
};



#endif //SLIDE_DIMENIONS_H
