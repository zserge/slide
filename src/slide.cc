#include "slide.h"
#include <iostream>

std::pair<int, int> slide::RenderScale(Page &p, Slide &slide,
                                       const Color &color, int xoff, int yoff,
                                       float scale) {
  //std::cerr << "render_scale" << std::endl;
  int w = 0;
  int h = 0;
  int line_height = 0;
  int line_width = 0;
  int prev_line = -1;
  for (auto token : slide) {
    if (token.line() != prev_line) {
      h = h + line_height;
      line_width = 0;
      prev_line = token.line();
    }
    line_height = p.TextHeight(token.style(), scale);
    int token_width = p.TextWidth(token.text(), token.style(), scale);
    int x = xoff + line_width;
    int y = yoff + h;
    if (color != 0) {
      p.Text(token.text(), color, x, y, token.style(), scale);
    }
    line_width = line_width + token_width;
    if (line_width > w) {
      w = line_width;
    }
  }
  h = h + line_height;
  return std::make_pair(xoff + w, yoff + h);
}

void slide::Render(Page &page, Slide &slide, const Color &fg, const Color &bg) {
  if (slide.size() == 0) {
    return;
  }
  //std::cerr << "render" << std::endl;
  float scale = 1.f;
  auto size = RenderScale(page, slide, 0, 0, 0, scale);
  scale = std::min(page.Size().Width() * 0.8 / size.first,
                   page.Size().Height() * 0.8 / size.second);
  size = RenderScale(page, slide, 0, 0, 0, scale);

  page.Background(bg);
  RenderScale(page, slide, fg, (page.Size().Width() - size.first) / 2,
              (page.Size().Height() - size.second) / 2, scale);

  //std::cerr << "render complete" << std::endl;
}

slide::Deck slide::Parse(const std::string &text) {
  Deck deck{};
  auto line_num = 0;
  auto begin = text.cbegin();

  for (auto it = begin, end = text.cend(); it < end;) {
    // Skip leading newlines
    for (; *it == '\n' && it < end; ++it, ++line_num)
      ;

    // Parse next slide
    Slide slide;

    for (; *it != '\n' && it < end; ++it, ++line_num) {
      std::string previous_text;

      Style current_text_style = Normal;

      switch (*it) {
      case Markers.ImagePath:
        ++it;
        // TODO: parse image path and geometry
        for (; it < end && *it != '\n'; ++it) {
        }
        break;

      case Markers.Heading:
        ++it;
        current_text_style = Header;
        for (; it < end && *it == ' '; ++it)
          ; // advance past whitespace
        break;

      case ' ':
        ++it; // advance past whitespace
        if (it < end && *it == ' ') {
          // doublespace is code
          ++it;
          current_text_style = Monospace;
        } else {
          previous_text += ' ';
        }
        break;
      case '.':
        ++it;
        break;
      default:
        break;
      }

      bool insert_empty_token = true;

      for (; it < end && *it != '\n'; ++it) {

        if (current_text_style == Normal && *it == Markers.Bold) {
          ++it;

          // first char is *
          if (it < end && *it != ' ' && *it != Markers.Bold) {

            // not a "**text" or a "* text"
            std::string bold_text;

            for (; it < end && *it != Markers.Bold && *it != '\n'; ++it) {
              // Add the character while it's not a '*' or  newline eg
              // "*hello*"
              bold_text += *it;
            }

            // end of a bold marker and
            if (*it == '*') {
              if (!previous_text.empty()) {
                // There's already something in the inner_text before this level
                // of bold so add that before adding the bold stuff
                slide.emplace_back(std::distance(begin, it), line_num,
                                   current_text_style, previous_text);
              }
              slide.emplace_back(std::distance(begin, it), line_num, Strong,
                                 bold_text);
              insert_empty_token = false;
              previous_text = "";
            } else {
              // must be a newline?
              previous_text += Markers.Bold + bold_text;
            }
          } else {
            // is a "**text" or "* text" or a "*\n" or a "\n"
            previous_text += Markers.Bold;
            if (*it != Markers.Bold) {
              // pretty sure here can only be a ' ' or a newline
              previous_text += *it;
            }
          }
        } else {
          previous_text += *it;
        }
      }
      if (insert_empty_token || !previous_text.empty()) {
        slide.emplace_back(std::distance(begin, it), line_num,
                           current_text_style, previous_text);
      }
    }

    // Skip trailing newlines
    for (; *it == '\n' && it < end; ++it, ++line_num)
      ;
    if (!slide.empty()) {
      deck.push_back(slide);
    }
  }
  return deck;
}