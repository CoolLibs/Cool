#include "StyleEditor.h"
#include <imgui.h>
#include "Cool/Path/Path.h"
#include "Cool/Serialization/as_json.h"
#include "ImStyleEd/cereal_style.hpp"

namespace Cool {

StyleEditor::StyleEditor()
{
    std::ignore = Cool::Serialization::from_json(ImGui::GetStyle(), Cool::Path::root() / "style.json");
}

StyleEditor::~StyleEditor()
{
    Cool::Serialization::to_json(ImGui::GetStyle(), Cool::Path::root() / "style.json");
}

} // namespace Cool