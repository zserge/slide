#ifndef SLIDE_STYLES_H
#define SLIDE_STYLES_H

#include <array>
#include <string>
namespace slide {

/**
 * @brief The different style of text which can appear on the slide
 */
namespace Style {
enum Style { Normal, Strong, Header, Monospace };

const std::string &to_str(const Style &s);

/**
 * @brief The format specifiers for the different styles
 */
constexpr struct {
public:
  char Bold = '*';
  char Heading = '#';
  char ImagePath = '@';
} Markers;
}; // namespace Style
}; // namespace slide

#endif // SLIDE_STYLES_H
