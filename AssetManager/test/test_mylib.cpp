#include "catch.hpp"
#include "mylib/mylib.h"

TEST_CASE("add function works correctly", "[mylib]") {
    REQUIRE(mylib::add(2, 3) == 5);
    REQUIRE(mylib::add(-1, 1) == 0);
    REQUIRE(mylib::add(0, 0) == 0);
    REQUIRE(mylib::add(-5, -3) == -8);
}
