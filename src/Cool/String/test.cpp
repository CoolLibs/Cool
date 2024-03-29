#include <vector>
#include "Cool/String/String.h"
#if COOL_ENABLE_TESTS

#include <stringify/stringify.hpp>
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

doctest::String toString(const std::pair<size_t, size_t>& values)
{
    std::stringstream output{};
    output << values.first << ", " << values.second;
    return doctest::String{output.str().c_str()};
}

// template<typename Container>
// doctest::String toString(const Container& values)
// {
//     std::stringstream output{};
//     for (const auto& value : values)
//     {
//         output << value << ", ";
//     }
//     return doctest::String{output.str().c_str()};
// }
} // namespace doctest

TEST_CASE("contains")
{
    CHECK(Cool::String::contains("abc", "a") == true);
    CHECK(Cool::String::contains("abc", "b") == true);
    CHECK(Cool::String::contains("abc", "c") == true);
    CHECK(Cool::String::contains("ab    c  ", "c") == true);
    CHECK(Cool::String::contains("abc", "d") == false);
    CHECK(Cool::String::contains("fse èç_à 68o gb", "èç_") == true);
    CHECK(Cool::String::contains("fse èç_à 68o gb", "67") == false);
}

TEST_CASE("to_lower")
{
    CHECK(Cool::String::to_lower("ABC") == "abc");
    CHECK(Cool::String::to_lower("aBcd") == "abcd");
    CHECK(Cool::String::to_lower("abcd") == "abcd");
    CHECK(Cool::String::to_lower("") == "");
    CHECK(Cool::String::to_lower("42") == "42");
}

TEST_CASE("to_string")
{
    CHECK(Cool::String::to_string(123, 3) == "123");
    CHECK(Cool::String::to_string<double>(12.3, 9) == "12.300000");
    CHECK(Cool::String::to_string<float>(12.3f, 9) == "12.300000");
}

TEST_CASE("[Cool::String] replace()")
{
    using namespace std::string_literals;
    const std::string                                      text         = "ssdgd ${hello}${world}ssd{c}v"s;
    const std::string                                      text2        = "ssdgd _{{hello}}}_{{world}}}ssd{c}v"s;
    const std::vector<std::pair<std::string, std::string>> replacements = {
        std::make_pair("hello"s, "1"s),
        std::make_pair("world"s, "2"s),
    };
    SUBCASE("replace_all()")
    {
        CHECK(Cool::String::replace_all("Effect_intensity", "_", " ") == "Effect intensity");
    }
    SUBCASE("replace_all_words()")
    {
        CHECK(Cool::String::replace_all_words("Test helloworld hello world hello something", "hello", "plop") == "Test helloworld plop world plop something");
        // When `to` contains `from`
        CHECK(Cool::String::replace_all_words("hello world", "hello", "hello plop") == "hello plop world");
        // When `to` contains `from`
        CHECK(Cool::String::replace_all_words("hello world", "hello", "plop hello") == "plop hello world");
        // When `to` contains `from`
        CHECK(Cool::String::replace_all_words("hello world", "hello", "plop hello pouet") == "plop hello pouet world");
    }
    SUBCASE("replace_between_delimiters()")
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
    SUBCASE("[Cool::String] replace_at()")
    {
        CHECK(Cool::String::replace_at(3, 5, "0123456789", "abcde") == "012abcde56789");
    }
}

TEST_CASE("[Cool::String] find_beginning_of_word()")
{
    CHECK(Cool::String::find_beginning_of_word("ell", "Hello World") == std::string::npos);
    CHECK(Cool::String::find_beginning_of_word("Hell", "Hello World") == 0);
    CHECK(Cool::String::find_beginning_of_word("Hello", "Hello World") == 0);
    CHECK(Cool::String::find_beginning_of_word("Hello Wor", "Hello World") == 0);
    CHECK(Cool::String::find_beginning_of_word("Wor", "Hello World") == 6);
}

TEST_CASE("[Cool::String] find_matching_pair()")
{
    SUBCASE("When there is nested delimiters")
    {
        CHECK(Cool::String::find_matching_pair({"abc(de(fg)h)ij"}) == std::make_pair<size_t, size_t>(3, 11));
        CHECK(Cool::String::find_matching_pair({"abc(de(fgh)ij"}) == std::nullopt);
    }
    SUBCASE("When the delimiters are the first and last characters of the text")
    {
        CHECK(Cool::String::find_matching_pair({"(abc)"}) == std::make_pair<size_t, size_t>(0, 4));
    }
    SUBCASE("When the text is empty")
    {
        CHECK(Cool::String::find_matching_pair({""}) == std::nullopt);
    }
    SUBCASE("When the first opening delimiter is at the end of the text")
    {
        CHECK(Cool::String::find_matching_pair({"abc("}) == std::nullopt);
    }
    SUBCASE("When the opening and closing delimiters are the same")
    {
        CHECK(Cool::String::find_matching_pair({.text = "ab|cde|fg", .opening = '|', .closing = '|'}).value() == std::make_pair<size_t, size_t>(2, 6));
        CHECK(Cool::String::find_matching_pair({.text = "ab|cde|fg|hi", .opening = '|', .closing = '|'}).value() == std::make_pair<size_t, size_t>(2, 6));
    }
    SUBCASE("Test offset parameter")
    {
        CHECK(Cool::String::find_matching_pair({"fes(svrsnv)", 2, '(', ')'}).value() == std::make_pair<size_t, size_t>(3, 10));
        CHECK(Cool::String::find_matching_pair({"fes(svrsnv)  (gregre)", 6, '(', ')'}).value() == std::make_pair<size_t, size_t>(13, 20));
        CHECK(Cool::String::find_matching_pair({"fes(svrs(nv)  (greg)re)", 6, '(', ')'}).value() == std::make_pair<size_t, size_t>(8, 11));
        CHECK(Cool::String::find_matching_pair({"// max (3., 6., 2.)", 6, '(', ')'}).value() == std::make_pair<size_t, size_t>(7, 18));
    }
}

TEST_CASE("[Cool::String] rfind_matching_pair()")
{
    CHECK(Cool::String::rfind_matching_pair({")"}) == std::nullopt);
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

TEST_CASE("remove_whitespaces")
{
    CHECK(Cool::String::remove_whitespaces("AB C") == "ABC");
    CHECK(Cool::String::remove_whitespaces("a    Bcd") == "aBcd");
    CHECK(Cool::String::remove_whitespaces("a bc    d") == "abcd");
}

TEST_CASE("is_commented_out")
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

TEST_CASE("substring")
{
    SUBCASE("Substring with 2 arguments for position")
    {
        CHECK(Cool::String::substring(std::string{"Hello World"}, 3, 3) == "");
        CHECK(Cool::String::substring(std::string{"Hello World"}, 0, 3) == "Hel");
        CHECK(Cool::String::substring(std::string{"Hello World"}, 2, 7) == "llo W");
    }
    SUBCASE("Substring with pair of positions")
    {
        CHECK(Cool::String::substring(std::string{"Hello World"}, std::make_pair(3, 3)) == "");
        CHECK(Cool::String::substring(std::string{"Hello World"}, std::make_pair(0, 3)) == "Hel");
        CHECK(Cool::String::substring(std::string{"Hello World"}, std::make_pair(2, 7)) == "llo W");
    }
}

TEST_CASE("find_next_word_position")
{
    CHECK(Cool::String::find_next_word_position("abcdef ghij klmno", 6) == std::make_optional(std::make_pair<size_t, size_t>(7, 11)));
    CHECK(Cool::String::find_next_word_position("abcdef ghij klmno", 7) == std::make_optional(std::make_pair<size_t, size_t>(7, 11)));
    CHECK(Cool::String::find_next_word_position("abcdef ghij klmno", 3) == std::make_optional(std::make_pair<size_t, size_t>(3, 6)));
    CHECK(Cool::String::find_next_word_position("abcdef ghij klmno", 7, "i") == std::make_optional(std::make_pair<size_t, size_t>(7, 9)));
    CHECK(Cool::String::find_next_word_position("ab(cd)ef ghij klmno", 0) == std::make_optional(std::make_pair<size_t, size_t>(0, 2)));
}

TEST_CASE("find_previous_word_position")
{
    CHECK(Cool::String::find_previous_word_position("Hello    World", 6) == std::make_optional(std::make_pair<size_t, size_t>(0, 5)));
    CHECK(Cool::String::find_previous_word_position("Hello World   HI", 12) == std::make_optional(std::make_pair<size_t, size_t>(6, 11)));
}

TEST_CASE("next_word")
{
    CHECK(Cool::String::next_word("abcdef ghij klmno", 6) == "ghij");
    CHECK(Cool::String::next_word("abcdef ghij klmno", 7) == "ghij");
    CHECK(Cool::String::next_word("abcdef ghij klmno", 3) == "def");
    CHECK(Cool::String::next_word("abcdef ghij klmno", 7, "i") == "gh");
    CHECK(Cool::String::next_word("ab(cd)ef ghij klmno", 0) == "ab");
}

TEST_CASE("all_words")
{
    CHECK(Cool::String::all_words("abcdef ghij klmno") == std::vector{"abcdef"s, "ghij"s, "klmno"s});
    CHECK(Cool::String::all_words("  abcdef ghij klmno") == std::vector{"abcdef"s, "ghij"s, "klmno"s});
    CHECK(Cool::String::all_words("abcdef ghij klmno  ") == std::vector{"abcdef"s, "ghij"s, "klmno"s});
    CHECK(Cool::String::all_words("  abcdef ghij klmno  ") == std::vector{"abcdef"s, "ghij"s, "klmno"s});
}

TEST_CASE("find_block")
{
    CHECK(
        //                              |
        Cool::String::find_block("// max 3.", 6) == "3."
    );
    CHECK(
        //                              |
        Cool::String::find_block("// max (3., 6., 2.)", 6) == std::make_optional<std::string_view>("(3., 6., 2.)")
    );
    CHECK(
        Cool::String::find_block("// max (1.f, 3.f)", 7) == "(1.f, 3.f)"
    );
    CHECK(
        Cool::String::find_block(")3., 6., 2.(") == std::make_optional<std::string_view>("3.")
    );
    CHECK(
        Cool::String::find_block(") 3., 6., 2.(") == std::make_optional<std::string_view>("3.")
    );
    CHECK(
        Cool::String::find_block("(3., (6., 2.))") == std::make_optional<std::string_view>("(3., (6., 2.))")
    );
    CHECK(
        Cool::String::find_block("(1.f, 3.f)") == "(1.f, 3.f)"
    );
    CHECK(
        Cool::String::find_block("(1.f, 3.f)") == "(1.f, 3.f)"
    );
    CHECK(
        Cool::String::find_block("bla // bla max some text (1.f, 3.f) blabla", 1) == "la"
    );
    CHECK(Cool::String::find_block("ma(x (1.f,) 3.f)") == "ma(x (1.f,) 3.f)");
    CHECK(Cool::String::find_block("ma (x (1.f,) 3.f)") == "ma");
    CHECK(Cool::String::find_block("vec3(0, 1, 0)") == "vec3(0, 1, 0)");
    CHECK(
        Cool::String::find_block("max 1.f, 3.f") == "max"
    );
    CHECK(
        Cool::String::find_block("hello world (lol yo)") == "hello"
    );
    CHECK(
        Cool::String::find_block("(hello world) lol") == "(hello world)"
    );
    CHECK(
        Cool::String::find_block("( )") == "( )"
    );
    CHECK(
        Cool::String::find_block("( , hello)") == "( , hello)"
    );
    CHECK(
        Cool::String::find_block("vec2  (1., 0.5)") == "vec2"
    );
    CHECK(
        Cool::String::find_block("   vec2(1., 0.5)") == "vec2(1., 0.5)"
    );
    CHECK(
        Cool::String::find_block("   vec2(  1., 0.5)") == "vec2(  1., 0.5)"
    );
}

TEST_CASE("find_block_following")
{
    CHECK(
        Cool::String::find_block_following("// default (1.f, 3.f)", "default") == std::make_optional<std::string>("(1.f, 3.f)")
    );
    CHECK(
        Cool::String::find_block_following("// min (1.f, 3.f)", "min") == std::make_optional<std::string>("(1.f, 3.f)")
    );
    CHECK(
        Cool::String::find_block_following("// max (1.f, 3.f)", "max") == std::make_optional<std::string>("(1.f, 3.f)")
    );
    CHECK(
        Cool::String::find_block_following("bla // bla max some text (1.f, 3.f) blabla", "max") == std::make_optional<std::string>("some")
    );
    CHECK(
        Cool::String::find_block_following("// ma(x (1.f,) 3.f)", "max") == std::nullopt
    );
    CHECK(
        Cool::String::find_block_following("// max 1.f, 3.f", "min") == std::nullopt
    );
}

TEST_CASE("contains_word()")
{
    CHECK(Cool::String::contains_word("Hello", "Hello World") == true);
    CHECK(Cool::String::contains_word("World", "Hello World") == true);
    CHECK(Cool::String::contains_word("Hell", "Hello World") == false);
    CHECK(Cool::String::contains_word("orld", "Hello World") == false);
    CHECK(Cool::String::contains_word("worldo", "Hello World") == false);
    CHECK(Cool::String::contains_word("world", "Hello World") == false);
    CHECK(Cool::String::contains_word("", "Hello World") == false);
    CHECK(Cool::String::contains_word("", "") == false);
    CHECK(Cool::String::contains_word("Hello World", "Hello World") == true);
    CHECK(Cool::String::contains_word("_time", "_delta_time _time") == true);
}

TEST_CASE("remove_comments()")
{
    CHECK(Cool::String::remove_comments("hello") == "hello");

    CHECK(Cool::String::remove_comments(R"STR(
hello// something
)STR") == R"STR(
hello
)STR");

    CHECK(Cool::String::remove_comments(R"STR(
hello// something)STR")
          == R"STR(
hello)STR");

    CHECK(Cool::String::remove_comments(R"STR(
hello// something // else
)STR") == R"STR(
hello
)STR");

    CHECK(Cool::String::remove_comments(R"STR(
hel/*lo
wor*/ld
)STR") == R"STR(
helld
)STR");

    CHECK(Cool::String::remove_comments(R"STR(
he//l/*lo
wor*/ld
)STR") == R"STR(
he
wor*/ld
)STR");
}

#endif