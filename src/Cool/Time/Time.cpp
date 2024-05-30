#include "Time.h"
#include "Cool/Time/time_formatted_hms.h"
#include "parse_time.h"
#include "time_formatted_hms.h"

namespace Cool {

auto Time::nanoseconds(int64_t ns) -> Time
{
    auto t         = Time{};
    t._nanoseconds = ns;
    return t;
}

auto Time::seconds(double s) -> Time
{
    double const floor = std::floor(s);
    double const fract = s - floor;

    return Time::nanoseconds(
        static_cast<int64_t>(floor) * 1'000'000'000
        + static_cast<int64_t>(fract * 1'000'000'000.)
    );
}

auto Time::as_seconds_float() const -> float
{
    return static_cast<float>(as_seconds_double());
}
auto Time::as_seconds_double() const -> double
{
    return static_cast<double>(_nanoseconds / 1'000'000'000) // NOLINT(bugprone-integer-division)
           + static_cast<double>(_nanoseconds % 1'000'000'000) / static_cast<double>(1'000'000'000);
}
auto Time::as_nanoseconds_float() const -> float
{
    return static_cast<float>(_nanoseconds);
}
auto Time::as_nanoseconds_double() const -> double
{
    return static_cast<double>(_nanoseconds);
}
auto Time::as_nanoseconds_int64_t() const -> int64_t
{
    return _nanoseconds;
}

auto operator+(Time a, Time b) -> Time
{
    return Time::nanoseconds(a._nanoseconds + b._nanoseconds);
}
auto operator-(Time a, Time b) -> Time
{
    return Time::nanoseconds(a._nanoseconds - b._nanoseconds);
}
void Time::operator+=(Time o)
{
    _nanoseconds += o._nanoseconds;
}
void Time::operator-=(Time o)
{
    _nanoseconds -= o._nanoseconds;
}
auto operator*(Time time, TimeSpeed speed) -> Time
{
    return operator*(speed, time);
}
auto operator*(TimeSpeed speed, Time time) -> Time
{
    if (speed.value == 1.) // This is the most common case, and we don't want to waste precision by converting back and forth between double and int64_t.
        return time;
    return Time::nanoseconds(static_cast<int64_t>(time.as_nanoseconds_double() * speed.value));
}

auto imgui_drag_time(const char* label, Time* value, imgui_drag_time_params const& p) -> bool
{
    double seconds = value->as_seconds_double();

    bool const b = ImGui::DragScalar(label, ImGuiDataType_Double, &seconds, p.drag_speed, &p.min, &p.max, time_formatted_hms(*value, p.show_milliseconds).c_str(), 0, [](const char* buf, void* data_p) {
        *(static_cast<double*>(data_p)) = parse_time(buf);
    });
    if (b)
        *value = Time::seconds(seconds);

    return b;
}

auto to_string(Time time) -> std::string
{
    return time_formatted_hms(time, true);
}

} // namespace Cool