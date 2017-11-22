#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <slide.hpp>

TEST_CASE("slide::parse", "empty") {
    auto deck = slide::parse("");
    REQUIRE(deck.size() == 0);
}
