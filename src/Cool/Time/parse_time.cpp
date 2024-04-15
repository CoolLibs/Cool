#include "parse_time.h"

namespace Cool {

static auto is_num(char c) -> bool
{
    return ('0' <= c && c <= '9')
           || c == '.'
           || c == '+'
           || c == '-';
}
static auto is_whitespace(char c) -> bool
{
    return c == ' '
           || c == '\t'
           || c == '\n'
           || c == '\r';
}

static auto seconds_multiplier(std::string const& str) -> float
{
    if (false // NOLINT(*simplify-boolean-expr)
        || str == "ms"
        || str == "milli"
        || str == "millis"
        || str == "millisecond"
        || str == "milliseconds"
        //
    )
    {
        return 1.f / 1000.f;
    }
    if (false // NOLINT(*simplify-boolean-expr)
        || str == "m"
        || str == "min"
        || str == "minute"
        || str == "minutes"
        //
    )
    {
        return 60.f;
    }
    if (false // NOLINT(*simplify-boolean-expr)
        || str == "h"
        || str == "hour"
        || str == "hours"
        //
    )
    {
        return 3600.f;
    }
    if (false // NOLINT(*simplify-boolean-expr)
        || str == "d"
        || str == "day"
        || str == "days"
        //
    )
    {
        return 24.f * 3600.f;
    }
    if (false // NOLINT(*simplify-boolean-expr)
        || str == "w"
        || str == "week"
        || str == "weeks"
        //
    )
    {
        return 7.f * 24.f * 3600.f;
    }

    return 1.f;
}

/// Returns time in seconds
auto parse_time(const char* str) -> float
{
    float total_time{0.f};
    while (*str != 0)
    {
        // First, read the string to get a number
        float const current_val = [&]() { // IIFE
            std::string number_token{};
            for (; *str != 0 && (is_num(*str) || is_whitespace(*str)); str++) // NOLINT(*pointer-arithmetic)
                number_token += *str;
            try
            {
                return std::stof(number_token);
            }
            catch (...)
            {
                return 0.f;
            }
        }();
        { // Then, read the string to get a word (minute / hour / etc.)
            std::string work_token{};
            for (; *str != 0 && !is_num(*str); str++) // NOLINT(*pointer-arithmetic)
            {
                if (!is_whitespace(*str))
                    work_token += *str;
            }
            total_time += current_val * seconds_multiplier(work_token);
        }
    }
    return total_time;
}

} // namespace Cool

#if COOL_ENABLE_TESTS
#include <doctest/doctest.h>
TEST_CASE("parse_time()")
{
    CHECK(doctest::Approx{Cool::parse_time("3m 2 s")} == 3 * 60 + 2);
    CHECK(doctest::Approx{Cool::parse_time("3m-2s")} == 3 * 60 - 2);
    CHECK(doctest::Approx{Cool::parse_time("3.78m")} == 3.78f * 60.f);
    // clang-format off
    CHECK(doctest::Approx{Cool::parse_time("4w 8d 13h 47s 318ms")} == 4 * 7 * 24 * 3600
                                                                    + 8     * 24 * 3600
                                                                    + 13         * 3600
                                                                    + 47
                                                                    + 318.f /1000.f);
    // clang-format on

    CHECK(doctest::Approx{Cool::parse_time("999m")} == 999 * 60);
    CHECK(doctest::Approx{Cool::parse_time("999 m")} == 999 * 60);
    CHECK(doctest::Approx{Cool::parse_time("999  m")} == 999 * 60);
    CHECK(doctest::Approx{Cool::parse_time("  999    m   ")} == 999 * 60);
    CHECK(doctest::Approx{Cool::parse_time("+999m")} == 999 * 60);
    CHECK(doctest::Approx{Cool::parse_time("-999m")} == -999 * 60);

    CHECK(doctest::Approx{Cool::parse_time("1ms")} == 1.f / 1000.f);
    CHECK(doctest::Approx{Cool::parse_time("1milli")} == 1.f / 1000.f);
    CHECK(doctest::Approx{Cool::parse_time("1millis")} == 1.f / 1000.f);
    CHECK(doctest::Approx{Cool::parse_time("1millisecond")} == 1.f / 1000.f);
    CHECK(doctest::Approx{Cool::parse_time("1milliseconds")} == 1.f / 1000.f);

    CHECK(doctest::Approx{Cool::parse_time("1s")} == 1);
    CHECK(doctest::Approx{Cool::parse_time("1sec")} == 1);
    CHECK(doctest::Approx{Cool::parse_time("1second")} == 1);
    CHECK(doctest::Approx{Cool::parse_time("1seconds")} == 1);

    CHECK(doctest::Approx{Cool::parse_time("1m")} == 60);
    CHECK(doctest::Approx{Cool::parse_time("1min")} == 60);
    CHECK(doctest::Approx{Cool::parse_time("1minute")} == 60);
    CHECK(doctest::Approx{Cool::parse_time("1minutes")} == 60);

    CHECK(doctest::Approx{Cool::parse_time("1h")} == 3600);
    CHECK(doctest::Approx{Cool::parse_time("1hour")} == 3600);
    CHECK(doctest::Approx{Cool::parse_time("1hours")} == 3600);

    CHECK(doctest::Approx{Cool::parse_time("1d")} == 24 * 3600);
    CHECK(doctest::Approx{Cool::parse_time("1day")} == 24 * 3600);
    CHECK(doctest::Approx{Cool::parse_time("1days")} == 24 * 3600);

    CHECK(doctest::Approx{Cool::parse_time("1w")} == 7 * 24 * 3600);
    CHECK(doctest::Approx{Cool::parse_time("1week")} == 7 * 24 * 3600);
    CHECK(doctest::Approx{Cool::parse_time("1weeks")} == 7 * 24 * 3600);
}
#endif