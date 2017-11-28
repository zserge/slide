//
// Created by Patrick Mintram on 28/11/2017.
//

#ifndef SLIDE_COLOUR_T_H
#define SLIDE_COLOUR_T_H


#include <cstdint>

class colour_t{
public:
	colour_t(void)
			: _rgba(0xffeeeeee) {}

	colour_t(const uint32_t rgba)
			:_rgba(rgba) {}

	colour_t(const int rgba)
			: _rgba(rgba) {}


	double red(void) const { return ((_rgba >> 16) & 0xff) / 255.0; }
	double green(void) const { return ((_rgba >> 8) & 0xff) / 255.0; }
	double blue(void) const { return ((_rgba) & 0xff) / 255.0; }
	double alpha(void) const { return ((_rgba >> 24) & 0xff) / 255.0; }
	uint32_t raw(void) const { return _rgba; }

	void operator=(const uint32_t& rhs){
		_rgba = rhs;
	}
	void operator=(const colour_t& rhs){
		_rgba = rhs._rgba;
	}
	void operator=(const int& rhs){
		_rgba = rhs;
	}
	bool operator==(const uint32_t &rhs) const {
		return rhs == _rgba;
	}
	bool operator==(const colour_t& rhs) const {
		return rhs._rgba == _rgba;
	}
	bool operator!=(const uint32_t& rhs) const {
		return rhs != _rgba;
	}
	bool operator!=(const colour_t& rhs) const {
		return _rgba != rhs._rgba;
	}
	bool operator==(const int& rhs) const {
		return rhs == (int)_rgba;
	}
	bool operator!=(const int& rhs) const {
		return rhs != (int)_rgba;
	}
	bool operator!=(int &rhs) const {
		return rhs != (int)_rgba;
	}

protected:
	uint32_t _rgba;
};


#endif //SLIDE_COLOUR_T_H
