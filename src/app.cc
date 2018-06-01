#include "app.h"
#include "debug_print.h"

#include <array>
namespace slide {
App::App(void) {
  wview_.title = "Slide";
  wview_.url = html_data_uri;
  wview_.width = 480;
  wview_.height = 320;
  wview_.resizable = 1;
  wview_.userdata = this;

  // The callback for when a webview event is invoked is the handleCommand
  // method
  wview_.external_invoke_cb = [](struct webview *w, const char *data) {
    App *app = static_cast<App *>(w->userdata);
    app->HandleCommand(data);
  };

  // Command Pattern used.
  // These must be static to prevent segfault
  // If new commands are required, just make it here, then
  // add it's command string to the map as done here.
  static CreateFileCmd create;
  static OpenFileCmd open;
  static ExportPdfCmd pdf;
  static SetPreviewSizeCmd prev;
  static SetTextCmd text;
  static SetPaletteCmd pal;
  static SetCursorCmd cur;

  cmds_map_["create_file"] = &create;
  cmds_map_["open_file"] = &open;
  cmds_map_["export_pdf"] = &pdf;
  cmds_map_["set_preview_size"] = &prev;
  cmds_map_["set_palette"] = &pal;
  cmds_map_["set_text"] = &text;
  cmds_map_["set_cursor"] = &cur;
}

void App::Run(void) {
  webview_init(&wview_);
  webview_dispatch(&wview_,
                   [](struct webview *w, void *arg) {
                     App *app = static_cast<App *>(w->userdata);
                     webview_eval(w, CssInject(styles_css).c_str());
                     webview_eval(w, picodom_js);
                     webview_eval(w, app_js);
                   },
                   nullptr);

  while (webview_loop(&wview_, 1) == 0) { /* Intentionally Blank */
  }
  webview_exit(&wview_);
}

void App::HandleCommand(const std::string &data) {

  auto json = nlohmann::json::parse(data);
  auto cmd = json.at("cmd").get<std::string>();

  // If the command isn't found, but we try to access it the command_map will
  // add the key with a null value, so before accessing confirm it's in the map
  if (cmds_map_.find(cmd) == cmds_map_.end()) {
    debug_print(cmd << " is not a valid command");
    return;
  }

  cmds_map_[cmd]->Execute(*this, json);

  Render();
}

void App::RenderCurrentSlide(void) {
  if (current_slide_ != -1) {
    debug_print("App::RenderCurrentSlide()");
    debug_print("app instance details: " << *this);
    PNG png(preview_size_.Width(), preview_size_.Height());
    slide::Render(png, deck_[current_slide_], foreground_, background_);
    debug_print("App::RenderCurrentSlide() render complete");
    preview_data_uri_ = png.DataUri();
  }
}

void App::Render(void) {
  auto json = nlohmann::json({});
  json["text"] = current_text_;
  json["previewDataURI"] = preview_data_uri_;
  // std::cerr << "JSON Dump: " << json.dump() << std::endl;
  webview_eval(
      &wview_,
      ("window.app.state=" + json.dump() + "; window.render()").c_str());
}

//----------- Below here is the command implementations for each key in the
// command map.
// If adding a new command simply add it here.

void CreateFileCmd::Execute(App &app, nlohmann::json &json) {
  // std::cerr << "Creating File" << std::endl;
  std::array<char, PATH_MAX> path;

  webview_dialog(&app.webview(), WEBVIEW_DIALOG_TYPE_SAVE, 0,
                 "New presentation...", nullptr, path, PATH_MAX - 1);
  if (std::string(path).length() != 0) {
    app.CurrentFile() = std::string(path);
    webview_set_title(&app.webview(), ("Slide - " + app.CurrentFile()).c_str());
  }
}

void OpenFileCmd::Execute(App &app, nlohmann::json &json) {
  // std::cerr << "Opening File" << std::endl;

  std::array<char, PATH_MAX> path;
  webview_dialog(&app.webview(), WEBVIEW_DIALOG_TYPE_OPEN, 0,
                 "Open presentation...", nullptr, path, sizeof(path) - 1);
  if (std::string(path).length() != 0) {
    app.CurrentFile() = path;
    webview_set_title(&app.webview(), ("Slide - " + app.CurrentFile()).c_str());
    std::ifstream ifs(app.CurrentFile());
    std::string text((std::istreambuf_iterator<char>(ifs)),
                     (std::istreambuf_iterator<char>()));
    app.CurrentText() = text;
    app.Deck() = slide::Parse(app.CurrentText());
  }
}

void ExportPdfCmd::Execute(App &app, nlohmann::json &json) {
  // std::cerr << "Exporting to PDF" << std::endl;
  std::array<char, PATH_MAX> path;
  webview_dialog(&app.webview(), WEBVIEW_DIALOG_TYPE_SAVE, 0, "Export PDF...",
                 nullptr, path, sizeof(path) - 1);
  if (strlen(path) != 0) {
    std::string path_str(path);
    const std::string extension(".pdf");
    if (path_str.length() <= extension.length() ||
        path_str.compare(path_str.length() - extension.length(),
                         extension.length(), extension)) {
      path_str += extension;
    }
    // std::cerr << "writing to " << path_str << std::endl;
    PDF pdf(path_str, 640, 480);
    for (auto slide : app.Deck()) {
      pdf.BeginPage();
      slide::Render(pdf, slide, app.Foreground(), app.Background());
      pdf.EndPage();
    }
  }
}

void SetPreviewSizeCmd::Execute(App &app, nlohmann::json &json) {
  // std::cerr << "Setting Preview Size" << std::endl;
  app.PreviewSize().Width() = json.at("w").get<int>();
  app.PreviewSize().Height() = json.at("h").get<int>();
  app.RenderCurrentSlide();
}

void SetPaletteCmd::Execute(App &app, nlohmann::json &json) {
  // std::cerr << "Setting palette" << std::endl;
  app.Foreground() = json.at("fg").get<int>();
  app.Background() = json.at("bg").get<int>();
  app.RenderCurrentSlide();
}

void SetTextCmd::Execute(App &app, nlohmann::json &json) {
  // std::cerr << "Setting Text" << std::endl;
  app.CurrentText() = json.at("text").get<std::string>();
  app.Deck() = slide::Parse(app.CurrentText());
  std::ofstream file(app.CurrentFile());
  file << app.CurrentText();
  app.RenderCurrentSlide();
}

void SetCursorCmd::Execute(App &app, nlohmann::json &json) {
  // std::cerr << "Setting Cursor" << std::endl;
  auto cursor = json.at("cursor").get<int>();
  app.CurrentSlide() = -1;
  for (int i = 0; app.CurrentSlide() == -1 && i < app.Deck().size(); i++) {
    for (auto token : app.Deck()[i]) {
      if (token.position() >= cursor) {
        app.CurrentSlide() = i;
        break;
      }
    }
  }
  app.RenderCurrentSlide();
}
} // namespace slide