#include "slide.h"
#include "styles.h"
#include <iostream>

#include "debug_print.h"

std::pair<int, int> slide::RenderScale(Page &p, Slide &slide,
                                       const Color &color, int xoff, int yoff,
                                       float scale) {
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
  debug_print("slide::Render(slide.size() = " << slide.size() << ")");
  if (slide.empty()) {
    return;
  }
  float scale = 1.f;
  auto size = RenderScale(page, slide, 0, 0, 0, scale);
  scale = (float)std::min(page.Size().Width() * 0.8 / size.first,
                          page.Size().Height() * 0.8 / size.second);
  size = RenderScale(page, slide, 0, 0, 0, scale);

  page.Background(bg);
  RenderScale(page, slide, fg, (page.Size().Width() - size.first) / 2,
              (page.Size().Height() - size.second) / 2, scale);

  debug_print("slide::Render complete");
}

slide::Deck slide::Parse(const std::string &text) {
  Deck deck;
  auto line_num = 0;
  auto begin = text.cbegin();

  debug_print("------------------------");
  debug_print("Parsing text [" << text.length() << "]:\t" << text);

  for (auto it = begin, end = text.cend(); it < end;) {
    // Skip leading newlines
    for (; *it == '\n' && it < end; ++it, ++line_num)
      ;

    // Parse next slide
    Slide slide;

    for (; *it != '\n' && it < end; ++it, ++line_num) {
      std::string previous_text;

      Style::Style current_text_style = Style::Normal;
      debug_print("Top of loop: " << *it);

      switch (*it) {
      case Style::Markers.ImagePath:
        ++it;
        // TODO: parse image path and geometry
        for (; it < end && *it != '\n'; ++it) {
        }
        break;

      case Style::Markers.Heading:
        ++it;
        current_text_style = Style::Header;
        for (; it < end && *it == ' '; ++it)
          ; // advance past whitespace
        break;

      case ' ':
        ++it; // advance past whitespace
        if (it < end && *it == ' ') {
          // doublespace is code
          ++it;
          current_text_style = Style::Monospace;
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

      debug_print("After switch: " << *it);

      bool insert_empty_token = true;

      for (; it < end && *it != '\n'; ++it) {

        if (current_text_style == Style::Normal && *it == Style::Markers.Bold) {
          ++it;
          debug_print("Bold start marker found and it's normal");

          // first char is *
          if (it < end && *it != ' ' && *it != Style::Markers.Bold &&
              *it != '\n') {
            debug_print(
                "Second character isn't whitespace or a bold marker or a "
                "newline\t"
                << *it);

            // not a "**text" or a "* text"
            std::string bold_text;

            for (; it < end && *it != Style::Markers.Bold && *it != '\n';
                 ++it) {
              // Add the character while it's not a '*' or  newline eg
              // "*hello*"
              bold_text += *it;
            }
            if (*it == '\n') {
              debug_print(
                  "Newline found before end of bold capture, so stopping "
                  "bold capture and addding it as normal text");
              previous_text = Style::Markers.Bold + bold_text;
              break;
            }

            debug_print("Text to be bold:\t" << bold_text);
            debug_print("it:\t" << *it);

            // end of a bold marker found
            if (*it == Style::Markers.Bold) {
              debug_print("End of bold marker found");

              if (!previous_text.empty()) {
                // There's already something in the inner_text before this level
                // of bold so add that before adding the bold stuff
                debug_print("Emplacing a non empty string:\t" << previous_text);
                slide.emplace_back(std::distance(begin, it), line_num,
                                   current_text_style, previous_text);
              }
              debug_print("Emplacing a bold text:\t" << bold_text);
              slide.emplace_back(std::distance(begin, it), line_num,
                                 Style::Strong, bold_text);

              insert_empty_token = false;
              previous_text = "";
            } else {
              debug_print("Not end of bold marker found");
              previous_text += Style::Markers.Bold + bold_text;
            }
          } else {
            debug_print("(1) Second character is a bold marker or whitespace:\t"
                        << *it);
            previous_text += Style::Markers.Bold;
            if (*it == '\n') {
              debug_print(
                  "Newline found immediately after a first bold marker so "
                  "starting a new token in the current slide.");
              break;
            }
            previous_text += *it;
            debug_print("(1a): " << previous_text);
          }
        }

        else {
          debug_print(
              "No bold marker found, or currently not processing normal text:\t"
              << *it);
          previous_text += *it;
        }
      }
      if (insert_empty_token || !previous_text.empty()) {
        debug_print("Adding empty token:\t" << previous_text);
        slide.emplace_back(std::distance(begin, it), line_num,
                           current_text_style, previous_text);
      }
    }

    // Skip trailing newlines
    for (; *it == '\n' && it < end; ++it, ++line_num)
      ;

    if (!slide.empty()) {
      debug_print("Adding slide");
      debug_print("------------------------");
      deck.push_back(slide);
    }
  }

  // Need to add a dummy slide so that if there are none the preview will still
  // display a slide with no text
  if (deck.empty()) {
    debug_print("Adding dummy slide");
    Slide s = {Token(0, 0, Style::Normal, "")};
    deck.push_back(s);
  }
  return deck;
}