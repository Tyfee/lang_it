#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "../src/lang_it.h"


TEST_CASE("PT_EN") {
    CHECK(translate("eu como arroz", "pt", "en", 0) == "i eat rice");
    CHECK(translate("eu não como arroz", "pt", "en", 0) == "i don't eat rice");
 
}