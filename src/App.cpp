#include "App.h"

App::App(void) {
	_wview.title 				= "Slide";
	_wview.url 					= html_data_uri;
	_wview.width 				= 480;
	_wview.height 				= 320;
	_wview.resizable 			= 1;
	_wview.userdata 			= this;

	// The callback for when a webvie event is invoked is the handleCommand method
	_wview.external_invoke_cb 	= [](struct webview *w, const char *data) {
		App *app = static_cast<App *>(w->userdata);
		app->handleCommand(data);
	};

	// Command Pattern used.
	// These must be static to prevent segfault
	// If new commands are required, just make it here, then
	// add it's command string to the map as done here.
	static create_file_cmd 			create;
	static open_file_cmd			open;
	static export_pdf_cmd			pdf;
	static set_preview_size_cmd 	prev;
	static set_text_cmd 			text;
	static set_palette_cmd 			pal;
	static set_cursor_cmd 			cur;

	_cmds_map["create_file"] 		= &create;
	_cmds_map["open_file"] 			= &open;
	_cmds_map["export_pdf"]			= &pdf;
	_cmds_map["set_preview_size"] 	= &prev;
	_cmds_map["set_palette"] 		= &pal;
	_cmds_map["set_text"] 			= &text;
	_cmds_map["set_cursor"] 		= &cur;
}

void App::run(void) {
	webview_init(&this->_wview);
	webview_dispatch(&this->_wview,
					 [](struct webview *w, void *arg) {
						 App *app = static_cast<App *>(w->userdata);
						 webview_eval(w, css_inject(styles_css).c_str());
						 webview_eval(w, picodom_js);
						 webview_eval(w, app_js);
					 },
					 nullptr);
	while (webview_loop(&this->_wview, 1) == 0) { /* Intentionally Blank */ }
	webview_exit(&this->_wview);
}

void App::handleCommand(const std::string &data) {

	auto json 	= nlohmann::json::parse(data);
	auto cmd 	= json.at("cmd").get<std::string>();
	std::cerr << cmd << std::endl;

	// If the command isn't found, but we try to access it the command_map will
	// add the key with a null value, so before accessing confirm it's in the map
	if(_cmds_map.find(cmd) == _cmds_map.end()){ return; }

	std::cerr << "Command found " << std::endl;
	_cmds_map[cmd]->execute(*this, json);
	std::cerr << "Command done" << std::endl;

	render();
}

void App::renderCurrentSlide(void) {
	if (_current_slide != -1) {
		slide::PNG png(_preview_size.width(), _preview_size.height());
		slide::render(png, _deck[_current_slide], _fg, _bg);
		_preview_data_uri = png.dataURI();
	}
}

void App::render(void) {
	auto json = nlohmann::json({});
	json["text"] = _current_text;
	json["previewDataURI"] = _preview_data_uri;
	webview_eval(&_wview, ("window.app.state=" + json.dump() + "; window.render()").c_str());
}

//----------- Below here is the command implementations for each key in the command map.
// If adding a new command simply add it here.

void create_file_cmd::execute(App& app, nlohmann::json & json){
	std::cerr<< "Creating File" << std::endl;

	char path[PATH_MAX];
	webview_dialog(&app.webview(), WEBVIEW_DIALOG_TYPE_SAVE, 0, "New presentation...", nullptr, path, PATH_MAX - 1);
	if (std::string(path).length() != 0) {
		app.current_file() = std::string(path);
		webview_set_title(&app.webview(), ("Slide - " + app.current_file()).c_str());
	}
}

void open_file_cmd::execute(App &app, nlohmann::json &json ){
	std::cerr<< "Opening File" << std::endl;

	char path[PATH_MAX];
	webview_dialog(&app.webview(), WEBVIEW_DIALOG_TYPE_OPEN, 0, "Open presentation...", nullptr, path, sizeof(path) - 1);
	if (std::string(path).length() != 0) {
		app.current_file() = path;
		webview_set_title(&app.webview(),
						  ("Slide - " + app.current_file()).c_str());
		std::ifstream ifs(app.current_file());
		std::string text((std::istreambuf_iterator<char>(ifs)),
						 (std::istreambuf_iterator<char>()));
		app.current_text() = text;
		app.deck() = slide::parse(app.current_text());
	}
}

void export_pdf_cmd::execute( App &app, nlohmann::json &json ){
	std::cerr<< "Exporting to PDF" << std::endl;
	char path[PATH_MAX];
	webview_dialog(&app.webview(), WEBVIEW_DIALOG_TYPE_SAVE, 0, "Export PDF...", nullptr, path, sizeof(path) - 1);
	if (strlen(path) != 0) {
		slide::PDF pdf(path, 640, 480);
		for (auto slide : app.deck()) {
			pdf.begin_page();
			slide::render(pdf, slide, app.foreground(), app.background());
			pdf.end_page();
		}
	}
}

void set_preview_size_cmd::execute( App &app, nlohmann::json &json ){
	std::cerr<< "Setting Preview Size" << std::endl;
	app.preview_size().width() = json.at("w").get<int>();
	app.preview_size().height() = json.at("h").get<int>();
	app.renderCurrentSlide();
}

void set_palette_cmd::execute( App &app, nlohmann::json &json ){
	std::cerr << "Setting palette" << std::endl;
	app.foreground() = json.at("fg").get<int>();
	app.background() = json.at("bg").get<int>();
	app.renderCurrentSlide();
}

void set_text_cmd::execute( App &app, nlohmann::json &json ){
	std::cerr<< "Setting Text" << std::endl;
	app.current_text() = json.at("text").get<std::string>();
	app.deck() = slide::parse(app.current_text());
	std::ofstream file(app.current_file());
	file << app.current_text();
}


void set_cursor_cmd::execute( App &app, nlohmann::json &json ){
	std::cerr<< "Setting Cursor" << std::endl;
	auto cursor = json.at("cursor").get<int>();
	app.current_slide() = -1;
	for (int i = 0; app.current_slide() == -1 && i < app.deck().size(); i++) {
		for (auto token : app.deck()[i]) {
			if (token.pos >= cursor) {
				app.current_slide() = i;
				break;
			}
		}
	}
	app.renderCurrentSlide();
}

