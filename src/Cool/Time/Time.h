#pragma once
#include <chrono>
#include "TimeSpeed.h"

namespace Cool {

template<typename T>
concept StdChronoDuration = std::derived_from<T, std::chrono::duration<typename T::rep, typename T::period>>;

class Time {
public:
    Time() = default;
    Time(StdChronoDuration auto const& time) // NOLINT(*explicit*)
    {
        *this = Time::nanoseconds(std::chrono::nanoseconds{time}.count());
    }
    static auto nanoseconds(int64_t) -> Time;
    static auto seconds(double) -> Time;

    [[nodiscard]] auto as_seconds_double() const -> double;
    [[nodiscard]] auto as_seconds_float() const -> float;
    [[nodiscard]] auto as_nanoseconds_double() const -> double;
    [[nodiscard]] auto as_nanoseconds_float() const -> float;
    [[nodiscard]] auto as_nanoseconds_int64_t() const -> int64_t;

    friend auto operator+(Time, Time) -> Time;
    friend auto operator-(Time, Time) -> Time;
    void        operator+=(Time);
    void        operator-=(Time);

    friend auto operator*(Time, TimeSpeed) -> Time;
    friend auto operator*(TimeSpeed, Time) -> Time;

    friend auto operator<=>(Time, Time) = default;

private:
    int64_t _nanoseconds{0};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Nanoseconds", _nanoseconds)
        );
    }
};

} // namespace Cool