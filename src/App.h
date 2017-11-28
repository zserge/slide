#ifndef SLIDE_APP_H
#define SLIDE_APP_H

#include <fstream>
#include <string>
#include "json.hpp"
#include "webview.h"
#include "slide.hpp"
#include "colour_t.h"
#include "ui.hpp"

class App {
public:
	App(void);
	void run(void);

private:
	void handleCommand(const std::string &data);
	void renderCurrentSlide(void);
	void render(void);

	slide::Deck deck;
	int current_slide = -1;
	std::string current_file;
	std::string current_text;
	std::string preview_data_uri;
	int preview_width = 320;
	int preview_height = 240;
	colour_t fg = 0xffeeeeee;
	colour_t bg = 0xff333333;
	//slide::Color fg = 0xffeeeeee;
	//slide::Color bg = 0xff333333;
	struct webview wview = {};
};


#endif //SLIDE_APP_H
