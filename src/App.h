#ifndef SLIDE_APP_H
#define SLIDE_APP_H

#include "colour_t.h"
#include "dimenions.h"
#include "json.hpp"
#include "slide.hpp"
#include "ui.hpp"
#include "webview.h"
#include <fstream>
#include <string>

class command_b;

class App {
public:
  App(void);
  void run(void);
  void renderCurrentSlide(void);
  void render(void);

  struct webview &webview(void) {
    return _wview;
  }
  std::string &current_file(void) { return _current_file; }
  slide::Deck &deck(void) { return _deck; }
  std::string &current_text(void) { return _current_text; }
  colour_t &foreground(void) { return _fg; }
  colour_t &background(void) { return _bg; }
  dimensions &preview_size(void) { return _preview_size; }
  int &current_slide(void) { return _current_slide; }

private:
  void handleCommand(const std::string &data);

  slide::Deck _deck;
  int _current_slide = -1;
  std::string _current_file;
  std::string _current_text;
  std::string _preview_data_uri;
  dimensions _preview_size = {320, 240};
  colour_t _fg = 0xffeeeeee;
  colour_t _bg = 0xff333333;
  struct webview _wview = {};
  std::map<std::string, command_b *> _cmds_map;
};

class command_b {
public:
  virtual void execute(App &app, nlohmann::json &json) = 0;
};

class create_file_cmd : public command_b {
public:
  void execute(App &app, nlohmann::json &json);
};

class open_file_cmd : public command_b {
public:
  void execute(App &app, nlohmann::json &json);
};

class export_pdf_cmd : public command_b {
public:
  void execute(App &app, nlohmann::json &json);
};

class set_preview_size_cmd : public command_b {
public:
  void execute(App &app, nlohmann::json &json);
};

class set_palette_cmd : public command_b {
public:
  void execute(App &app, nlohmann::json &json);
};

class set_text_cmd : public command_b {
public:
  void execute(App &app, nlohmann::json &json);
};

class set_cursor_cmd : public command_b {
public:
  void execute(App &app, nlohmann::json &json);
};

#endif // SLIDE_APP_H
