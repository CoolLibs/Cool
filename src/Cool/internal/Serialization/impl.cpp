///
/// We put all our serialization here in a separate .cpp file
/// because including "cereal/archives/json.hpp" slows down
/// compilation by A LOT. So we want to avoid recompiling
/// this file as much as possible.
///

#include <cereal/types/polymorphic.hpp>
#include "Cool/ImGui/StyleEditor.h"
#include "Cool/Nodes/EditorImpl.h"
#include "Cool/Serialization/Serialization.h"
#include "ImStyleEd/cereal_style.hpp"
#include "SStyleEditor.h"

//
#include "cereal/archives/json.hpp"

namespace Cool {

auto do_serialize(StyleEditor const& editor, std::filesystem::path const& path) -> bool
{
    return Cool::Serialization::save<StyleEditor, cereal::JSONOutputArchive>(editor, path);
}
auto do_deserialize(StyleEditor& editor, std::filesystem::path const& path) -> Cool::OptionalErrorMessage
{
    return Cool::Serialization::load<StyleEditor, cereal::JSONInputArchive>(editor, path);
}

} // namespace Cool

CEREAL_REGISTER_TYPE(Cool::NodesEditorImpl);                                     /*NOLINT*/
CEREAL_REGISTER_POLYMORPHIC_RELATION(Cool::INodesEditor, Cool::NodesEditorImpl); /*NOLINT*/