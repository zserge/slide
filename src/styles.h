#ifndef SLIDE_STYLES_H
#define SLIDE_STYLES_H

namespace slide {
enum Style { Normal, Strong, Header, Monospace };
	static const struct Markers{
	public:
		static const char Bold = '*';
		static const char Heading = '#';
		static const char ImagePath = '@';
	}Markers;
};

#endif // SLIDE_STYLES_H
