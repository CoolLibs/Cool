// https://gist.github.com/galloscript/8a5d179e432e062550972afcd1ecf112

// TODO(ASG) Is there a good reason to depend on the rest of Cool or can we create a standalone library
/* TODO(ASG) Replace this with our own documentation

 Usage:

 ::GRADIENT DATA::
 ImGradient gradient;

 ::BUTTON::
 if(ImGui::GradientButton(&gradient))
 {
    //set show editor flag to true/false
 }

 ::EDITOR::
 static ImGradientMark* draggingMark = nullptr;
 static ImGradientMark* selectedMark = nullptr;

 bool updated = ImGui::GradientEditor(&gradient, draggingMark, selectedMark);

 ::GET A COLOR::
 float color[3];
 gradient.getColorAt(0.3f, color); //position from 0 to 1

 ::MODIFY GRADIENT WITH CODE::
 gradient.getMarks().clear();
 gradient.add_mark(0.0f, ImColor(0.2f, 0.1f, 0.0f));
 gradient.add_mark(0.7f, ImColor(120, 200, 255));

 ::WOOD BROWNS PRESET::
 gradient.getMarks().clear();
 gradient.add_mark(0.0f, ImColor(0xA0, 0x79, 0x3D));
 gradient.add_mark(0.2f, ImColor(0xAA, 0x83, 0x47));
 gradient.add_mark(0.3f, ImColor(0xB4, 0x8D, 0x51));
 gradient.add_mark(0.4f, ImColor(0xBE, 0x97, 0x5B));
 gradient.add_mark(0.6f, ImColor(0xC8, 0xA1, 0x65));
 gradient.add_mark(0.7f, ImColor(0xD2, 0xAB, 0x6F));
 gradient.add_mark(0.8f, ImColor(0xDC, 0xB5, 0x79));
 gradient.add_mark(1.0f, ImColor(0xE6, 0xBF, 0x83));

 */

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

    auto get() const -> float { return value; }
    void set(const float pos) { value = pos; }

    friend auto operator<(const RelativePosition& a, const RelativePosition& b) -> bool { return a.get() < b.get(); }
    friend auto operator>(const RelativePosition& a, const RelativePosition& b) -> bool { return a.get() > b.get(); }
    friend auto operator==(const RelativePosition& a, const RelativePosition& b) -> bool { return a.get() == b.get(); }
    friend auto operator!=(const RelativePosition& a, const RelativePosition& b) -> bool { return !(a == b); }

private:
    float value{};
};

struct ImGradientMark {
    ImVec4           color{0.f, 0.f, 0.f, 1.f};
    RelativePosition position{0.f};

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
namespace ImGuiExtras {

class ImGradientWidget {
public:
    ImGradientWidget() = default;
    const ImGradient& get_gradient() const { return gradient; }
    ImGradient&       get_gradient() { return gradient; }

    bool        gradient_editor();
    friend auto operator==(const ImGradientWidget& a, const ImGradientWidget& b) -> bool { return a.get_gradient() == b.get_gradient(); }

private:
    ImGradient      gradient{};
    ImGradientMark* dragging_mark{};
    ImGradientMark* selected_mark{};
};

bool gradient_button(ImGradient* gradient);

} // namespace ImGuiExtras

} // namespace Cool