#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <Slide.h>

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
  REQUIRE(deck[0][0].text_ == "Header 1");
  REQUIRE(deck[0][1].text_ == "Foo");
  REQUIRE(deck[0][2].text_ == "Bar");
  REQUIRE(deck[0][0].style_ == slide::Style::Header);
  REQUIRE(deck[0][1].style_ == slide::Style::Normal);
  REQUIRE(deck[0][2].style_ == slide::Style::Normal);
  REQUIRE(deck[0][0].line_ == 1);
  REQUIRE(deck[0][1].line_ == 2);
  REQUIRE(deck[0][2].line_ == 3);

  REQUIRE(deck[1].size() == 9);
  REQUIRE(deck[1][0].text_ == "Header 2");

  REQUIRE(deck[2].size() == 1);
  REQUIRE(deck[2][0].text_ == "Header 3");

  REQUIRE(deck[3].size() == 11);
  REQUIRE(deck[3][0].text_ == "Header 4");
  REQUIRE(deck[3][1].text_ == "");
  REQUIRE(deck[3][2].text_ == "# normal line");
  REQUIRE(deck[3][3].text_ == "  normal line");
  REQUIRE(deck[3][4].text_ == "Кириллица. € ß");
  REQUIRE(deck[3][5].text_ == "Some ");
  REQUIRE(deck[3][6].text_ == "bold");
  REQUIRE(deck[3][6].style_ == slide::Style::Strong);
  REQUIRE(deck[3][7].text_ == " text");
  REQUIRE(deck[3][8].text_ == "Some ");
  REQUIRE(deck[3][9].text_ == "other bold");
  REQUIRE(deck[3][9].style_ == slide::Style::Strong);
  REQUIRE(deck[3][10].text_ == " text");
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

  REQUIRE(deck[0][0].text_ == "foo ");
  REQUIRE(deck[0][0].style_ == slide::Style::Normal);
  REQUIRE(deck[0][1].text_ == "bar");
  REQUIRE(deck[0][1].style_ == slide::Style::Strong);
  REQUIRE(deck[0][2].text_ == " baz");
  REQUIRE(deck[0][2].style_ == slide::Style::Normal);

  REQUIRE(deck[0][3].text_ == "foo bar");
  REQUIRE(deck[0][3].style_ == slide::Style::Strong);
  REQUIRE(deck[0][4].text_ == " baz");
  REQUIRE(deck[0][4].style_ == slide::Style::Normal);

  REQUIRE(deck[0][5].text_ == "foo ");
  REQUIRE(deck[0][5].style_ == slide::Style::Normal);
  REQUIRE(deck[0][6].text_ == "bar baz");
  REQUIRE(deck[0][6].style_ == slide::Style::Strong);

  REQUIRE(deck[0][7].text_ == "foo *bar baz");
  REQUIRE(deck[0][7].style_ == slide::Style::Normal);

  REQUIRE(deck[0][8].text_ == "* foo bar baz*");
  REQUIRE(deck[0][8].style_ == slide::Style::Normal);
}

TEST_CASE("Render", "[slide::render]") {
  auto deck = slide::Parse(R"(
# Hello
foo *bar* baz
)");
  class MockPage : public Page {
  public:
    MockPage(void) : Page() { /* Intentionally Blank */
    }
    int width(void) const { return 100; }
    int height(void) const { return 50; }
    int text_height(slide::Style style, float scale) {
      this->called["text_height"]++;
      return scale * 10;
    }
    int text_width(const std::string &s, slide::Style style, float scale) {
      this->called["text_width"]++;
      return s.size() * scale * 5;
    }
    void bg(const Color &c) {
      this->called["_bg"]++;
      REQUIRE(c == 0xff00ff00);
    }
    void text(const std::string &s, const Color &c, int x, int y, slide::Style,
              float scale) {
      this->called["text::" + s]++;
      REQUIRE((s == "Hello" || s == "foo " || s == "bar" || s == " baz"));
      REQUIRE((scale == 1.f || (scale > 1.44f && scale < 1.46f)));
    }
    void check() {
      REQUIRE(this->called["_bg"] > 0);
      REQUIRE(this->called["text_width"] > 0);
      REQUIRE(this->called["text_height"] > 0);
      REQUIRE(this->called["text::Hello"] > 0);
      REQUIRE(this->called["text::foo "] > 0);
      REQUIRE(this->called["text::bar"] > 0);
      REQUIRE(this->called["text:: baz"] > 0);
    }

  private:
    std::map<std::string, int> called;
  };

  MockPage mockPage;
  slide::Render(mockPage, deck[0], 0xffff0000, 0xff00ff00);
  mockPage.check();
}
