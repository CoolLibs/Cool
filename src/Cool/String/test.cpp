#include "String.h"
#include "doctest/doctest.h"

TEST_CASE("[Cool::String] replace()")
{
    using namespace std::string_literals;
    const std::string                                      text         = "ssdgd ${hello}${world}ssd{c}v"s;
    const std::string                                      text2        = "ssdgd _{{hello}}}_{{world}}}ssd{c}v"s;
    const std::vector<std::pair<std::string, std::string>> replacements = {
        std::make_pair("hello"s, "1"s),
        std::make_pair("world"s, "2"s),
    };
    SUBCASE("replace()")
    {
        CHECK(Cool::String::replace_between_delimiters({text, replacements}) == "ssdgd 12ssd{c}v");
        CHECK(Cool::String::replace_between_delimiters({text2, replacements, "_{{", "}}}"}) == "ssdgd 12ssd{c}v");
    }
    SUBCASE("replace_next()")
    {
        const auto replace_next_res = Cool::String::replace_next({text, replacements}, 0);
        CHECK(replace_next_res.second.has_value());
        CHECK(replace_next_res.first == "ssdgd 1${world}ssd{c}v");
        CHECK(replace_next_res.second.value() == 7);
    }
    SUBCASE("find_replacement()")
    {
        CHECK(Cool::String::find_replacement("hello", replacements).value() == "1");
        CHECK(Cool::String::find_replacement("hell", replacements) == std::nullopt);
    }
}

TEST_CASE("replace_all()")
{
    std::string text = "Effect_intensity";
    Cool::String::replace_all(text, "_", " ");
    CHECK(text == "Effect intensity");
}

TEST_CASE("[Cool::String] replace_at()")
{
    CHECK(Cool::String::replace_at(3, 5, "0123456789", "abcde") == "012abcde56789");
}

TEST_CASE("[Cool::String] find_matching_pair()")
{
    SUBCASE("When there is nested delimiters")
    {
        CHECK(Cool::String::find_matching_pair("abc(de(fg)h)ij") == std::make_pair<size_t, size_t>(3, 11));
        CHECK(Cool::String::find_matching_pair("abc(de(fgh)ij") == std::nullopt);
    }
    SUBCASE("When the delimiters are the first and last characters of the text")
    {
        CHECK(Cool::String::find_matching_pair("(abc)") == std::make_pair<size_t, size_t>(0, 4));
    }
    SUBCASE("When the text is empty")
    {
        CHECK(Cool::String::find_matching_pair("") == std::nullopt);
    }
    SUBCASE("When the first opening delimiter is at the end of the text")
    {
        CHECK(Cool::String::find_matching_pair("abc(") == std::nullopt);
    }
    SUBCASE("When the opening and closing delimiters are the same")
    {
        CHECK(Cool::String::find_matching_pair("ab|cde|fg", '|', '|').value() == std::make_pair<size_t, size_t>(2, 6));
        CHECK(Cool::String::find_matching_pair("ab|cde|fg|hi", '|', '|').value() == std::make_pair<size_t, size_t>(2, 6));
    }
}

TEST_CASE("[Cool::String] split_into_words()")
{
    CHECK(Cool::String::split_into_words("hello wor  ld") == std::vector<std::string>({"hello", "wor", "ld"}));
    CHECK(Cool::String::split_into_words("hello wor  ld ") == std::vector<std::string>({"hello", "wor", "ld"}));
    CHECK(Cool::String::split_into_words(" hello wor  ld ") == std::vector<std::string>({"hello", "wor", "ld"}));
    CHECK(Cool::String::split_into_words(" ") == std::vector<std::string>({}));
    CHECK(Cool::String::split_into_words("   ") == std::vector<std::string>({}));
    CHECK(Cool::String::split_into_words("hello, wor, ,   ld", " ,") == std::vector<std::string>({"hello", "wor", "ld"}));
}

TEST_CASE("Next block")
{
    CHECK(
        Cool::String::next_block("// default (1.f, 3.f)", 10) == "1.f, 3.f"
    );
    CHECK(
        Cool::String::next_block("// min (1.f, 3.f)", 6) == "1.f, 3.f"
    );
    CHECK(
        Cool::String::next_block("// max (1.f, 3.f)", 6) == "1.f, 3.f"
    );
    CHECK(
        Cool::String::next_block("bla // bla max some text (1.f, 3.f) blabla", 1) == "1.f, 3.f"
    );
    CHECK(
        Cool::String::next_block("// ma(x (1.f,) 3.f)", 3) == "x (1.f,"
    );
    CHECK(
        Cool::String::next_block("// max 1.f, 3.f", 3) == "max" // doesn't work
    );
}

TEST_CASE("Parsing an int")
{
    CHECK(
        Cool::String::value_from_string<int>("2.f") ==
        2
    );
    CHECK(
        Cool::String::value_from_string<int>("2.") ==
        2
    );
    CHECK(
        Cool::String::value_from_string<int>("2") ==
        2
    );
    CHECK(
        Cool::String::value_from_string<int>("2.0") ==
        2
    );
    CHECK(
        Cool::String::value_from_string<int>("2.0f") ==
        2
    );
}

TEST_CASE("Parsing a float")
{
    CHECK(
        Cool::String::value_from_string<float>("2.f") ==
        2.f
    );
    CHECK(
        Cool::String::value_from_string<float>("2.") ==
        2.f
    );
    CHECK(
        Cool::String::value_from_string<float>("2") ==
        2.f
    );
    CHECK(
        Cool::String::value_from_string<float>("2.0") ==
        2.f
    );
    CHECK(
        Cool::String::value_from_string<float>("2.0f") ==
        2.f
    );
}

TEST_CASE("Parsing a vec2")
{
    CHECK(
        Cool::String::value_from_string<glm::vec2>("  ( 1   ,  3)   ") ==
        glm::vec2(1.f, 3.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(1 3)") ==
        glm::vec2(1.f, 3.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(1.f, 3.f)") ==
        glm::vec2(1.f, 3.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(1., 3.)") ==
        glm::vec2(1.f, 3.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(1., hello)") ==
        std::nullopt
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(hello, 1.)") ==
        std::nullopt
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("(hello, world)") ==
        std::nullopt
    );
}

TEST_CASE("Parsing a RgbColor")
{
    CHECK(
        Cool::String::value_from_string<Cool::RgbColor>("(1, 3, 4)") ==
        Cool::RgbColor(glm::vec3(1.f, 3.f, 4.f))
    );
}