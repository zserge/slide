#include <fstream>

#include <json.hpp>
#include <webview.h>

#include <slide.hpp>

#include "ui.hpp"

class App {
public:
  App() {
    this->webview.title = "Slide";
    this->webview.url = html_data_uri;
    this->webview.width = 480;
    this->webview.height = 320;
    this->webview.resizable = 1;
    this->webview.userdata = this;
    this->webview.external_invoke_cb = [](struct webview *w, const char *data) {
      App *app = static_cast<App *>(w->userdata);
      app->handleCommand(data);
    };
  }

  void run() {
    webview_init(&this->webview);
    webview_dispatch(&this->webview,
                     [](struct webview *w, void *arg) {
                       App *app = static_cast<App *>(w->userdata);
                       webview_eval(w, css_inject(styles_css).c_str());
                       webview_eval(w, picodom_js);
                       webview_eval(w, app_js);
                     },
                     nullptr);
    while (webview_loop(&this->webview, 1) == 0) {
    }
    webview_exit(&this->webview);
  }

private:
  void handleCommand(const std::string &data) {
    char path[PATH_MAX];
    auto json = nlohmann::json::parse(data);
    auto cmd = json.at("cmd").get<std::string>();
    if (cmd == "create_file") {
      webview_dialog(&this->webview, WEBVIEW_DIALOG_TYPE_SAVE, 0,
                     "New presentation...", nullptr, path, sizeof(path) - 1);
      if (strlen(path) != 0) {
        this->current_file = path;
        webview_set_title(&this->webview,
                          ("Slide - " + this->current_file).c_str());
      }
    } else if (cmd == "open_file") {
      webview_dialog(&this->webview, WEBVIEW_DIALOG_TYPE_OPEN, 0,
                     "Open presentation...", nullptr, path, sizeof(path) - 1);
      if (strlen(path) != 0) {
        this->current_file = path;
        webview_set_title(&this->webview,
                          ("Slide - " + this->current_file).c_str());
        std::ifstream ifs(this->current_file);
        std::string text((std::istreambuf_iterator<char>(ifs)),
                         (std::istreambuf_iterator<char>()));
        this->current_text = text;
        this->deck = slide::parse(this->current_text);
      }
    } else if (cmd == "export_pdf") {
      webview_dialog(&this->webview, WEBVIEW_DIALOG_TYPE_SAVE, 0,
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

  void renderCurrentSlide() {
    if (this->current_slide != -1) {
      slide::PNG png(this->preview_width, this->preview_height);
      slide::render(png, this->deck[this->current_slide], this->fg, this->bg);
      this->preview_data_uri = png.dataURI();
    }
  }

  void render() {
    auto json = nlohmann::json({});
    json["text"] = this->current_text;
    json["previewDataURI"] = this->preview_data_uri;
    webview_eval(
        &this->webview,
        ("window.app.state=" + json.dump() + "; window.render()").c_str());
  }

  slide::Deck deck;
  int current_slide = -1;
  std::string current_file;
  std::string current_text;
  std::string preview_data_uri;
  int preview_width = 320;
  int preview_height = 240;
  slide::Color fg = 0xffeeeeee;
  slide::Color bg = 0xff333333;
  struct webview webview = {};
};

int main() {
  App app;
  app.run();
  return 0;
}
