#ifndef SLIDE_APP_H
#define SLIDE_APP_H

#include "color.h"
#include "dimenions.h"
#include "json.hpp"
#include "slide.h"
#include "ui.h"
#include "webview.h"
#include <fstream>
#include <string>

namespace slide {
class CommandInterface;

class App {
public:
  App(void);

  void Run(void);

  void RenderCurrentSlide(void);

  void Render(void);

  struct webview &webview(void) {
    return wview_;
  }

  std::string &CurrentFile(void) { return current_file_; }

  slide::Deck &Deck(void) { return deck_; }

  std::string &CurrentText(void) { return current_text_; }

  Color &Foreground(void) { return foreground_; }

  Color &Background(void) { return background_; }

  Dimensions &PreviewSize(void) { return preview_size_; }

  int &CurrentSlide(void) { return current_slide_; }


	friend std::ostream& operator << (std::ostream& out ,const App& a){

		std::stringstream cmd_names;

		out << "deck_ size: " << a.deck_.size() << std::endl
			<< "current_slide_: " << a.current_slide_ << std::endl
			<< "current_file_: " << a.current_file_ << std::endl
			<< "current_text_ and preview_data_uri_ omitted" << std::endl
			//<< "current_text_: " << a.current_text_ << std::endl
			//<< "preview_data_uri: " << a.preview_data_uri_ << std::endl
			<< "preview_data_size: " << a.preview_size_ << std::endl
			<< "foreground_: " << a.foreground_ << std::endl
			<< "background_: " << a.background_ << std::endl
			<< "wview_: " << a.wview_.title << std::endl
			<< "commands_map_: ";

		for(auto cmd: a.cmds_map_){
			out << cmd.first << ", ";
		}
		return out;
	}

private:
  void HandleCommand(const std::string &data);

  slide::Deck deck_;
  int current_slide_ = -1;
  std::string current_file_;
  std::string current_text_;
  std::string preview_data_uri_;
  Dimensions preview_size_ = {320, 240};
  Color foreground_ = 0xffeeeeee;
  Color background_ = 0xff333333;
  struct webview wview_ = {};
  std::map<std::string, CommandInterface *> cmds_map_;
};

class CommandInterface {
public:
  virtual void Execute(App &app, nlohmann::json &json) = 0;
};

class CreateFileCmd : public CommandInterface {
public:
  void Execute(App &app, nlohmann::json &json);
};

class OpenFileCmd : public CommandInterface {
public:
  void Execute(App &app, nlohmann::json &json);
};

class ExportPdfCmd : public CommandInterface {
public:
  void Execute(App &app, nlohmann::json &json);
};

class SetPreviewSizeCmd : public CommandInterface {
public:
  void Execute(App &app, nlohmann::json &json);
};

class SetPaletteCmd : public CommandInterface {
public:
  void Execute(App &app, nlohmann::json &json);
};

class SetTextCmd : public CommandInterface {
public:
  void Execute(App &app, nlohmann::json &json);
};

class SetCursorCmd : public CommandInterface {
public:
  void Execute(App &app, nlohmann::json &json);
};
} // namespace slide
#endif // SLIDE_APP_H
