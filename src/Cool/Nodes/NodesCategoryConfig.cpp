#include <filesystem>
#include "NodesCategoryConfig.h"
#include "Cool/Serialization/as_json.h"
#include "Cool/Path/Path.h"
#include "imgui.h"

auto NodesCategoryConfig::set_color(std::string category_name) -> void
{
    std::filesystem::path const url = Cool::Path::root() / "Nodes" / category_name / "category_config.json";
    // std::filesystem::remove(url);
    Cool::Serialization::to_json(this->color, url, "Color");
}

auto NodesCategoryConfig::get_color() -> Cool::Color
{
    return this->color;
}

auto NodesCategoryConfig::serialize_from_json(std::filesystem::path url) -> void
{
    Cool::Serialization::from_json(this->color, url);
}

auto NodesCategoryConfig::popup() -> bool
{
    bool color_modified = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (imgui_widget("Color :", this->color, ImGuiColorEditFlags_PickerHueBar))
        {
            color_modified = true;
        }
        ImGui::EndPopup();
    }
    return color_modified;
}