#include "StyleEditor.h"
#include <imgui.h>
#include <cereal/archives/json.hpp>
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/Serialization.h"
#include "ImStyleEd/cereal_style.hpp"

namespace Cool {

StyleEditor::StyleEditor()
{
    std::ignore = Cool::Serialization::load<ImGuiStyle, cereal::JSONInputArchive>(ImGui::GetStyle(), Cool::Path::root() / "style.json");
}

StyleEditor::~StyleEditor()
{
    Cool::Serialization::save<ImGuiStyle, cereal::JSONOutputArchive>(ImGui::GetStyle(), Cool::Path::root() / "style.json");
}

void StyleEditor::imgui()
{
    ImGuiExtras::GetStyle().imgui();
}

} // namespace Cool