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
#include "SPresetManager.h"
#include "SStyleEditor.h"
#include "SWebcamsConfigs.h"

//
#include "cereal/archives/json.hpp"

namespace Cool {

auto do_save(StyleEditor const& editor, std::filesystem::path const& path) -> bool
{
    return Cool::Serialization::save<StyleEditor, cereal::JSONOutputArchive>(editor, path);
}
auto do_load(StyleEditor& editor, std::filesystem::path const& path) -> Cool::OptionalErrorMessage
{
    return Cool::Serialization::load<StyleEditor, cereal::JSONInputArchive>(editor, path);
}

auto do_save(WebcamsConfigsMap const& configs, Cool::SerializerOnDemand const& serializer) -> bool
{
    return serializer.save<WebcamsConfigsMap, cereal::JSONOutputArchive>(configs);
}
auto do_load(WebcamsConfigsMap& configs, Cool::SerializerOnDemand const& serializer) -> Cool::OptionalErrorMessage
{
    return serializer.load<WebcamsConfigsMap, cereal::JSONInputArchive>(configs);
}

auto do_save(reg::RawOrderedRegistry<Preset> const& data, Cool::SerializerOnDemand const& serializer) -> bool
{
    return serializer.save<reg::RawOrderedRegistry<Preset>, cereal::JSONOutputArchive>(data);
}
auto do_load(reg::RawOrderedRegistry<Preset>& data, Cool::SerializerOnDemand const& serializer) -> Cool::OptionalErrorMessage
{
    return serializer.load<reg::RawOrderedRegistry<Preset>, cereal::JSONInputArchive>(data);
}
void do_load_default_presets(reg::RawOrderedRegistry<Preset>& data, std::filesystem::path const& path)
{
    try
    {
        auto is      = std::ifstream{path};
        auto archive = cereal::JSONInputArchive{is};
        archive(data);
    }
    catch (...)
    {
    }
}

} // namespace Cool

CEREAL_REGISTER_TYPE(Cool::NodesEditorImpl);                                     /*NOLINT*/
CEREAL_REGISTER_POLYMORPHIC_RELATION(Cool::INodesEditor, Cool::NodesEditorImpl); /*NOLINT*/