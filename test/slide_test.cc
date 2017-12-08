#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <slide.h>
namespace slide {
TEST_CASE("Empty _deck", "[slide::parse]") {
  slide::Deck deck;

  deck = slide::Parse("");
  REQUIRE(deck.size() == 0);
  deck = slide::Parse("\n");
  REQUIRE(deck.size() == 0);
  deck = slide::Parse("\n\n");
  REQUIRE(deck.size() == 0);
}

TEST_CASE("Slide boundaries", "[slide::parse]") {
  slide::Deck deck;

  // Single-slide decks
  deck = slide::Parse("foo");
  REQUIRE(deck.size() == 1);
  deck = slide::Parse("foo\n");
  REQUIRE(deck.size() == 1);
  deck = slide::Parse("foo\n\n\n");
  REQUIRE(deck.size() == 1);
  deck = slide::Parse("\nfoo");
  REQUIRE(deck.size() == 1);
  deck = slide::Parse("\n\nfoo\n");
  REQUIRE(deck.size() == 1);
  deck = slide::Parse("\n\nfoo\n\n\n");
  REQUIRE(deck.size() == 1);

  // Two-slide decks
  deck = slide::Parse("foo\n\nbar");
  REQUIRE(deck.size() == 2);
  deck = slide::Parse("foo\n\nbar\n");
  REQUIRE(deck.size() == 2);
  deck = slide::Parse("foo\n\nbar\n\n");
  REQUIRE(deck.size() == 2);
  deck = slide::Parse("\nfoo\n\nbar\n\n");
  REQUIRE(deck.size() == 2);
  deck = slide::Parse("\nfoo\nbar\n\nbaz\n\n");
  REQUIRE(deck.size() == 2);
}

TEST_CASE("Slide tokens", "[slide::parse]") {
  slide::Deck deck;
  deck = slide::Parse(R"(
# Header 1
Foo
Bar



#Header 2
  
  #include <stdio._h>
  
  int main() {
    int n; /* comment */
    int *i = &n; *i = 0;
    return 0;
  }

# Header 3

# Header 4
.
.# normal line
.  normal line
Кириллица. € ß
Some *bold* text
Some *other bold* text)");

  REQUIRE(deck.size() == 4);

  REQUIRE(deck[0].size() == 3);
  REQUIRE(deck[0][0].text() == "Header 1");
  REQUIRE(deck[0][1].text() == "Foo");
  REQUIRE(deck[0][2].text() == "Bar");
  REQUIRE(deck[0][0].style() == Header);
  REQUIRE(deck[0][1].style() == Normal);
  REQUIRE(deck[0][2].style() == Normal);
  REQUIRE(deck[0][0].line() == 1);
  REQUIRE(deck[0][1].line() == 2);
  REQUIRE(deck[0][2].line() == 3);

  REQUIRE(deck[1].size() == 9);
  REQUIRE(deck[1][0].text() == "Header 2");

  REQUIRE(deck[2].size() == 1);
  REQUIRE(deck[2][0].text() == "Header 3");

  REQUIRE(deck[3].size() == 11);
  REQUIRE(deck[3][1].text() == "");
  REQUIRE(deck[3][0].text() == "Header 4");
  REQUIRE(deck[3][2].text() == "# normal line");
  REQUIRE(deck[3][3].text() == "  normal line");
  REQUIRE(deck[3][4].text() == "Кириллица. € ß");
  REQUIRE(deck[3][5].text() == "Some ");
  REQUIRE(deck[3][6].text() == "bold");
  REQUIRE(deck[3][6].style() == Strong);
  REQUIRE(deck[3][7].text() == " text");
  REQUIRE(deck[3][8].text() == "Some ");
  REQUIRE(deck[3][9].text() == "other bold");
  REQUIRE(deck[3][9].style() == Strong);
  REQUIRE(deck[3][10].text() == " text");
}

TEST_CASE("Asterisks", "[slide::parse]") {
  slide::Deck deck;
  deck = slide::Parse(R"(
foo *bar* baz
*foo bar* baz
foo *bar baz*
foo **bar baz
* foo bar baz*
)");
  REQUIRE(deck[0].size() == 9);

  REQUIRE(deck[0][0].text() == "foo ");
  REQUIRE(deck[0][0].style() == Normal);
  REQUIRE(deck[0][1].text() == "bar");
  REQUIRE(deck[0][1].style() == Strong);
  REQUIRE(deck[0][2].text() == " baz");
  REQUIRE(deck[0][2].style() == Normal);

  REQUIRE(deck[0][3].text() == "foo bar");
  REQUIRE(deck[0][3].style() == Strong);
  REQUIRE(deck[0][4].text() == " baz");
  REQUIRE(deck[0][4].style() == Normal);

  REQUIRE(deck[0][5].text() == "foo ");
  REQUIRE(deck[0][5].style() == Normal);
  REQUIRE(deck[0][6].text() == "bar baz");
  REQUIRE(deck[0][6].style() == Strong);

  REQUIRE(deck[0][7].text() == "foo *bar baz");
  REQUIRE(deck[0][7].style() == Normal);

  REQUIRE(deck[0][8].text() == "* foo bar baz*");
  REQUIRE(deck[0][8].style() == Normal);
}

TEST_CASE("Render", "[slide::render]") {
  auto deck = slide::Parse(R"(
# Hello
foo *bar* baz
)");
  class MockPage : public Page {
  public:
    MockPage(void) : Page(100, 50) { /* Intentionally Blank */
    }

    int TextHeight(Style style, float scale) {
      called_["text_height"]++;
      return scale * 10;
    }

    int TextWidth(const std::string &s, Style style, float scale) {
      called_["text_width"]++;
      return s.size() * scale * 5;
    }

    void Background(const Color &c) {
      called_["bg"]++;
      REQUIRE(c == 0xff00ff00);
    }

    void Text(const std::string &s, const Color &c, int x, int y, slide::Style,
              float scale) {
      called_["text::" + s]++;
      REQUIRE((s == "Hello" || s == "foo " || s == "bar" || s == " baz"));
      REQUIRE((scale == 1.f || (scale > 1.44f && scale < 1.46f)));
    }

    void Check(void) {
      REQUIRE(called_["bg"] > 0);
      REQUIRE(called_["text_width"] > 0);
      REQUIRE(called_["text_height"] > 0);
      REQUIRE(called_["text::Hello"] > 0);
      REQUIRE(called_["text::foo "] > 0);
      REQUIRE(called_["text::bar"] > 0);
      REQUIRE(called_["text:: baz"] > 0);
    }

  private:
    std::map<std::string, int> called_;
    void InitialiseContext(void) {
      // Do nothing
    }
  };

  MockPage mockPage;
  Render(mockPage, deck[0], 0xffff0000, 0xff00ff00);
  mockPage.Check();
}
} // namespace slide
