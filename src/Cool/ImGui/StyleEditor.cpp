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
    std::ignore = Cool::Serialization::load<StyleEditor, cereal::JSONInputArchive>(*this, Cool::Path::root() / "style.json");
}

StyleEditor::~StyleEditor()
{
    Cool::Serialization::save<StyleEditor, cereal::JSONOutputArchive>(*this, Cool::Path::root() / "style.json");
}

void StyleEditor::imgui()
{
    ImGuiExtras::GetStyle().imgui();
}

} // namespace Cool