#include "StyleEditor.h"
#include <imgui.h>
#include <cereal/archives/json.hpp>
#include "Cool/Path/Path.h"
#include "Cool/Serialization/as_json.h"
#include "ImStyleEd/cereal_style.hpp"

namespace Cool {

StyleEditor::StyleEditor()
{
    std::ignore = Cool::Serialization::from_json<ImGuiStyle, cereal::JSONInputArchive>(ImGui::GetStyle(), Cool::Path::root() / "style.json");
}

StyleEditor::~StyleEditor()
{
    Cool::Serialization::to_json<ImGuiStyle, cereal::JSONOutputArchive>(ImGui::GetStyle(), Cool::Path::root() / "style.json");
}

} // namespace Cool