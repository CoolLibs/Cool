#pragma once

#include <list>
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"

namespace Cool {
class RelativePosition { // TODO(ASG) Move to its own file in Cool/StrongTypes
public:
    RelativePosition(float position)
        : value(position)
    {
        IM_ASSERT(0.f <= position && position <= 1.f);
    }

    auto   get() const -> float { return value; }
    float& get() { return value; }
    void   set(const float pos) { value = pos; }

    friend auto operator<(const RelativePosition& a, const RelativePosition& b) -> bool { return a.get() < b.get(); }
    friend auto operator>(const RelativePosition& a, const RelativePosition& b) -> bool { return a.get() > b.get(); }
    friend auto operator==(const RelativePosition& a, const RelativePosition& b) -> bool { return a.get() == b.get(); }
    friend auto operator!=(const RelativePosition& a, const RelativePosition& b) -> bool { return !(a == b); }

private:
    float value{};
};

struct ImGradientMark {
    RelativePosition position{0.f};
    ImVec4           color{0.f, 0.f, 0.f, 1.f};

    ImGradientMark(RelativePosition position, ImVec4 color)
        : position{position}, color{color}
    {}
    friend auto operator==(const ImGradientMark& a, const ImGradientMark& b) -> bool
    {
        return (a.position == b.position) &&
               (a.color.x == b.color.x) &&
               (a.color.y == b.color.y) &&
               (a.color.z == b.color.z) &&
               (a.color.w == b.color.w);
    };
};

struct Marks {
    // Use std::list instead of vector because it is easier to remove a mark when we do not know the index
    std::list<ImGradientMark> _marks{};

    void sorted()
    {
        _marks.sort([](const ImGradientMark& a, const ImGradientMark& b) { return a.position < b.position; });
    }
    ImGradientMark* add_mark(ImGradientMark mark)
    {
        _marks.push_back(mark);
        ImGradientMark* ptr = &_marks.back();
        sorted();
        return ptr;
    }
    void remove_mark(const ImGradientMark& mark)
    {
        _marks.remove(mark);
        sorted();
    };
    friend auto operator==(const Marks& a, const Marks& b) -> bool { return a._marks == b._marks; }
};

class ImGradient {
public:
    ImGradient();
    ImVec4          get_color_at(float position) const;
    ImGradientMark* add_mark(const ImGradientMark& mark)
    {
        return m_marks.add_mark(mark);
    };
    void remove_mark(const ImGradientMark& mark)
    {
        m_marks.remove_mark(mark);
    };
    std::list<ImGradientMark>& get_list() { return m_marks._marks; }
    Marks&                     get_marks() { return m_marks; }
    friend auto                operator==(const ImGradient& a, const ImGradient& b) -> bool { return a.m_marks._marks == b.m_marks._marks; }

private:
    ImVec4 compute_color_at(RelativePosition position) const;
    Marks  m_marks{};
};
} // namespace Cool