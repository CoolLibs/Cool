#include "String.h"
#include "doctest/doctest.h"

namespace doctest {
template<typename T>
doctest::String toString(const std::optional<T>& value)
{
    if (value)
    {
        return "Some(" + toString(*value) + ")";
    }
    else
    {
        return "None";
    }
}
} // namespace doctest

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

TEST_CASE("find_block")
{
    CHECK(
        //                              |
        Cool::String::find_block("// max 3.", 6) == "3."
    );
    CHECK(
        //                              |
        Cool::String::find_block("// max (3., 6., 2.)", 6) ==
        std::make_optional<std::string_view>("(3., 6., 2.)")
    );
    CHECK(
        Cool::String::find_block("// max (1.f, 3.f)", 7) == "(1.f, 3.f)"
    );
    CHECK(
        Cool::String::find_block(")3., 6., 2.(", 0) ==
        std::make_optional<std::string_view>("3.")
    );
    CHECK(
        Cool::String::find_block(") 3., 6., 2.(", 0) ==
        std::make_optional<std::string_view>("3.")
    );
    CHECK(
        Cool::String::find_block("(3., (6., 2.))", 0) ==
        std::make_optional<std::string_view>("(3., (6., 2.))")
    );
    CHECK(
        Cool::String::find_block("(1.f, 3.f)", 0) == "(1.f, 3.f)"
    );
    CHECK(
        Cool::String::find_block("(1.f, 3.f)", 0) == "(1.f, 3.f)"
    );
    CHECK(
        Cool::String::find_block("bla // bla max some text (1.f, 3.f) blabla", 1) == "la"
    );
    CHECK(Cool::String::find_block("ma(x (1.f,) 3.f)", 0) == "ma(x (1.f,) 3.f)");
    CHECK(Cool::String::find_block("ma (x (1.f,) 3.f)", 0) == "ma");
    CHECK(Cool::String::find_block("vec3(0, 1, 0)", 0) == "vec3(0, 1, 0)");
    CHECK(
        Cool::String::find_block("max 1.f, 3.f", 0) == "max"
    );
    CHECK(
        Cool::String::find_block("hello world (lol yo)", 0) == "hello"
    );
    CHECK(
        Cool::String::find_block("(hello world) lol", 0) == "(hello world)"
    );
}

TEST_CASE("find_block_following")
{
    CHECK(
        Cool::String::find_block_following("// default (1.f, 3.f)", "default") ==
        std::make_optional<std::string>("(1.f, 3.f)")
    );
    CHECK(
        Cool::String::find_block_following("// min (1.f, 3.f)", "min") ==
        std::make_optional<std::string>("(1.f, 3.f)")
    );
    CHECK(
        Cool::String::find_block_following("// max (1.f, 3.f)", "max") ==
        std::make_optional<std::string>("(1.f, 3.f)")
    );
    CHECK(
        Cool::String::find_block_following("bla // bla max some text (1.f, 3.f) blabla", "max") ==
        std::make_optional<std::string>("some")
    );
    CHECK(
        Cool::String::find_block_following("// ma(x (1.f,) 3.f)", "max") == std::nullopt
    );
    CHECK(
        Cool::String::find_block_following("// max 1.f, 3.f", "min") == std::nullopt
    );
}

TEST_CASE("find_value_for_given_key")
{
    CHECK(
        Cool::String::find_value_for_given_key<int>("// default 1", "default") ==
        std::make_optional<int>(1)
    );
    CHECK(
        Cool::String::find_value_for_given_key<float>("// min 1.f", "min") ==
        std::make_optional<float>(1.f)
    );
    CHECK(
        Cool::String::find_value_for_given_key<glm::vec2>("// max (1.f, 3.f)", "max") ==
        std::make_optional<glm::vec2>(glm::vec2(1.f, 3.f))
    );
    CHECK(
        Cool::String::find_value_for_given_key<bool>("// max true", "max") ==
        std::make_optional<bool>(true)
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

TEST_CASE("Parsing an bool")
{
    CHECK(
        Cool::String::value_from_string<bool>("true") ==
        true
    );
    CHECK(
        Cool::String::value_from_string<bool>("false") ==
        false
    );
    CHECK(
        Cool::String::value_from_string<bool>("True") ==
        true
    );
    CHECK(
        Cool::String::value_from_string<bool>("FALSE") ==
        false
    );
    CHECK(
        Cool::String::value_from_string<bool>("true false") ==
        std::nullopt
    );
    CHECK(
        Cool::String::value_from_string<bool>("truefalse") ==
        std::nullopt
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
    CHECK(
        Cool::String::value_from_string<glm::vec2>("vec2(3., 2.)") ==
        glm::vec2(3.f, 2.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("vec(3., 2.)") ==
        glm::vec2(3.f, 2.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("hello(3., 2.)") ==
        glm::vec2(3.f, 2.f)
    );
    CHECK(
        Cool::String::value_from_string<glm::vec2>("vec2( 3., 2.)") ==
        glm::vec2(3.f, 2.f)
    );
}

TEST_CASE("Parsing a ivec2")
{
    CHECK(
        Cool::String::value_from_string<glm::ivec2>("  ( 1   ,  3)   ") ==
        glm::ivec2(1, 3)
    );
    CHECK(
        Cool::String::value_from_string<glm::ivec2>("(1 3)") ==
        glm::ivec2(1, 3)
    );
    CHECK(
        Cool::String::value_from_string<glm::ivec2>("(1, 3)") ==
        glm::ivec2(1, 3)
    );
    CHECK(
        Cool::String::value_from_string<glm::ivec2>("(1., 3.)") ==
        glm::ivec2(1, 3)
    );
    CHECK(
        Cool::String::value_from_string<glm::ivec2>("(1., hello)") ==
        std::nullopt
    );
    CHECK(
        Cool::String::value_from_string<glm::ivec2>("(hello, 1.)") ==
        std::nullopt
    );
    CHECK(
        Cool::String::value_from_string<glm::ivec2>("(hello, world)") ==
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

TEST_CASE("Parsing an Angle")
{
    CHECK(
        Cool::String::value_from_string<Cool::Angle>("180") ==
        Cool::Angle{Cool::Radians{Cool::degrees_to_radians(180)}}
    );
}

TEST_CASE("Parsing a Direction2D")
{
    CHECK(
        Cool::String::value_from_string<Cool::Direction2D>("180") ==
        Cool::Direction2D{Cool::Angle{Cool::Radians{Cool::degrees_to_radians(180)}}}
    );
    CHECK(
        Cool::String::value_from_string<Cool::Direction2D>("180") ==
        Cool::Direction2D{Cool::get_angle_from_vector(glm::vec2(-1.f, 0.f))}
    );
}

TEST_CASE("is_commented_out()")
{
    CHECK(Cool::String::is_commented_out("Hello // World") == false);
    CHECK(Cool::String::is_commented_out("/ Hello") == false);
    CHECK(Cool::String::is_commented_out("/ Hello //") == false);
    CHECK(Cool::String::is_commented_out("a / b") == false);
    CHECK(Cool::String::is_commented_out("a // b") == false);
    CHECK(Cool::String::is_commented_out("//") == true);
    CHECK(Cool::String::is_commented_out("//Hello") == true);
    CHECK(Cool::String::is_commented_out("// Hello") == true);
    CHECK(Cool::String::is_commented_out("  //") == true);
    CHECK(Cool::String::is_commented_out("  // Hello") == true);
    CHECK(Cool::String::is_commented_out("      //") == true);
    CHECK(Cool::String::is_commented_out("      // Hello") == true);
}