#include "AspectRatio.h"
#include <smart/smart.hpp>

namespace Cool {

static float make_valid_ratio(float ratio)
{
    return smart::keep_above(0.001f, ratio);
}

AspectRatio::AspectRatio(float aspect_ratio)
    : _ratio{make_valid_ratio(aspect_ratio)}
    , _imgui_current_ratio_item{-1}
{}

auto AspectRatio::imgui() -> bool
{
    bool b = false;
    if (ImGui::Combo("##aspect_ratio_dropdown", &_imgui_current_ratio_item, " 16/9\0 3/2\0 4/3\0 1/1\0 9/16\0 2/3\0 3/4\0\0"))
    {
        switch (_imgui_current_ratio_item)
        {
        case 0:
            _ratio = 16.f / 9.f;
            break;
        case 1:
            _ratio = 3.f / 2.f;
            break;
        case 2:
            _ratio = 4.f / 3.f;
            break;
        case 3:
            _ratio = 1.f;
            break;
        case 4:
            _ratio = 9.f / 16.f;
            break;
        case 5:
            _ratio = 2.f / 3.f;
            break;
        case 6:
            _ratio = 3.f / 4.f;
            break;
        }
        b = true;
    }
    if (ImGui::SliderFloat("##aspect_ratio_slider", &_ratio, 0.5f, 2.f))
    {
        _ratio                    = make_valid_ratio(_ratio);
        _imgui_current_ratio_item = -1;
        b                         = true;
    }
    return b;
}

} // namespace Cool