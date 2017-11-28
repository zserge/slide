//
// Created by Patrick Mintram on 28/11/2017.
//

#include "App.h"

App::App(void) {
	this->wview.title = "Slide";
	this->wview.url = html_data_uri;
	this->wview.width = 480;
	this->wview.height = 320;
	this->wview.resizable = 1;
	this->wview.userdata = this;
	this->wview.external_invoke_cb = [](struct webview *w, const char *data) {
		App *app = static_cast<App *>(w->userdata);
		app->handleCommand(data);
	};
}

void App::run(void) {
	webview_init(&this->wview);
	webview_dispatch(&this->wview,
					 [](struct webview *w, void *arg) {
						 App *app = static_cast<App *>(w->userdata);
						 webview_eval(w, css_inject(styles_css).c_str());
						 webview_eval(w, picodom_js);
						 webview_eval(w, app_js);
					 },
					 nullptr);
	while (webview_loop(&this->wview, 1) == 0) {
	}
	webview_exit(&this->wview);
}

void App::handleCommand(const std::string &data) {
	char path[PATH_MAX];
	auto json = nlohmann::json::parse(data);
	auto cmd = json.at("cmd").get<std::string>();
	if (cmd == "create_file") {
		webview_dialog(&this->wview, WEBVIEW_DIALOG_TYPE_SAVE, 0,
					   "New presentation...", nullptr, path, sizeof(path) - 1);
		if (strlen(path) != 0) {
			this->current_file = path;
			webview_set_title(&this->wview,
							  ("Slide - " + this->current_file).c_str());
		}
	} else if (cmd == "open_file") {
		webview_dialog(&this->wview, WEBVIEW_DIALOG_TYPE_OPEN, 0,
					   "Open presentation...", nullptr, path, sizeof(path) - 1);
		if (strlen(path) != 0) {
			this->current_file = path;
			webview_set_title(&this->wview,
							  ("Slide - " + this->current_file).c_str());
			std::ifstream ifs(this->current_file);
			std::string text((std::istreambuf_iterator<char>(ifs)),
							 (std::istreambuf_iterator<char>()));
			this->current_text = text;
			this->deck = slide::parse(this->current_text);
		}
	} else if (cmd == "export_pdf") {
		webview_dialog(&this->wview, WEBVIEW_DIALOG_TYPE_SAVE, 0,
					   "Export PDF...", nullptr, path, sizeof(path) - 1);
		if (strlen(path) != 0) {
			slide::PDF pdf(path, 640, 480);
			for (auto slide : this->deck) {
				pdf.begin_page();
				slide::render(pdf, slide, this->fg, this->bg);
				pdf.end_page();
			}
		}
	} else if (cmd == "set_preview_size") {
		this->preview_width = json.at("w").get<int>();
		this->preview_height = json.at("h").get<int>();
		this->renderCurrentSlide();
	} else if (cmd == "set_palette") {
		this->fg = json.at("fg").get<int>();
		this->bg = json.at("bg").get<int>();
		this->renderCurrentSlide();
	} else if (cmd == "set_text") {
		this->current_text = json.at("text").get<std::string>();
		this->deck = slide::parse(this->current_text);
		std::ofstream file(this->current_file);
		file << this->current_text;
	} else if (cmd == "set_cursor") {
		auto cursor = json.at("cursor").get<int>();
		this->current_slide = -1;
		for (int i = 0; this->current_slide == -1 && i < this->deck.size(); i++) {
			for (auto token : this->deck[i]) {
				if (token.pos >= cursor) {
					this->current_slide = i;
					break;
				}
			}
		}
		this->renderCurrentSlide();
	}
	this->render();
}

void App::renderCurrentSlide(void) {
	if (this->current_slide != -1) {
		slide::PNG png(this->preview_width, this->preview_height);
		slide::render(png, this->deck[this->current_slide], this->fg, this->bg);
		this->preview_data_uri = png.dataURI();
	}
}

void App::render(void) {
	auto json = nlohmann::json({});
	json["text"] = this->current_text;
	json["previewDataURI"] = this->preview_data_uri;
	webview_eval(
			&this->wview,
			("window.app.state=" + json.dump() + "; window.render()").c_str());
}




