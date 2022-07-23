#pragma once

#include <list>
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "RelativePosition.h"
#include "imgui_internal.h"

namespace Cool {

struct Mark {
    RelativePosition position{0.f};
    ImVec4           color{0.f, 0.f, 0.f, 1.f};

    Mark(RelativePosition position, ImVec4 color)
        : position{position}, color{color}
    {}
    friend auto operator==(const Mark& a, const Mark& b) -> bool
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
    std::list<Mark> _marks{};

    void sorted()
    {
        _marks.sort([](const Mark& a, const Mark& b) { return a.position < b.position; });
    }
    Mark* add_mark(Mark mark)
    {
        _marks.push_back(mark);
        Mark* ptr = &_marks.back();
        sorted();
        return ptr;
    }
    void remove_mark(const Mark& mark)
    {
        _marks.remove(mark);
        sorted();
    };
    friend auto operator==(const Marks& a, const Marks& b) -> bool { return a._marks == b._marks; }
};

class GradientMarks {
public:
    GradientMarks();
    ImVec4 get_color_at(float position) const;
    Mark*  add_mark(const Mark& mark)
    {
        return m_marks.add_mark(mark);
    };
    void remove_mark(const Mark& mark)
    {
        m_marks.remove_mark(mark);
    };
    std::list<Mark>& get_list() { return m_marks._marks; }
    Marks&           get_marks() { return m_marks; }
    friend auto      operator==(const GradientMarks& a, const GradientMarks& b) -> bool { return a.m_marks._marks == b.m_marks._marks; }

private:
    ImVec4 compute_color_at(RelativePosition position) const;
    Marks  m_marks{};
};
} // namespace Cool