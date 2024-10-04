///
/// We put all our serialization here in a separate .cpp file
/// because including "ser20/archives/json.hpp" slows down
/// compilation by A LOT. So we want to avoid recompiling
/// this file as much as possible.
///

#include <ser20/types/polymorphic.hpp>
#include "Cool/ImGui/StyleEditor.h"
#include "Cool/Nodes/EditorImpl.h"
#include "Cool/Serialization/Serialization.h"
#include "ImStyleEd/ser20_style.hpp"
#include "SPresetManager.h"
#include "SStyleEditor.h"
#include "SWebcamsConfigs.h"

//
#include "ser20/archives/json.hpp"

namespace Cool {

auto do_save(StyleEditor const& editor, std::filesystem::path const& path) -> bool
{
    return Cool::Serialization::save<StyleEditor, ser20::JSONOutputArchive>(editor, path);
}
auto do_load(StyleEditor& editor, std::filesystem::path const& path) -> Cool::OptionalErrorMessage
{
    return Cool::Serialization::load<StyleEditor, ser20::JSONInputArchive>(editor, path);
}

auto do_save(wcam::ResolutionsMap const& configs, Cool::SerializerOnDemand const& serializer) -> bool
{
    return serializer.save<wcam::ResolutionsMap, ser20::JSONOutputArchive>(configs);
}
auto do_load(wcam::ResolutionsMap& configs, Cool::SerializerOnDemand const& serializer) -> Cool::OptionalErrorMessage
{
    return serializer.load<wcam::ResolutionsMap, ser20::JSONInputArchive>(configs);
}

auto do_save(reg::RawOrderedRegistry<Preset> const& data, Cool::SerializerOnDemand const& serializer) -> bool
{
    return serializer.save<reg::RawOrderedRegistry<Preset>, ser20::JSONOutputArchive>(data);
}
auto do_load(reg::RawOrderedRegistry<Preset>& data, Cool::SerializerOnDemand const& serializer) -> Cool::OptionalErrorMessage
{
    return serializer.load<reg::RawOrderedRegistry<Preset>, ser20::JSONInputArchive>(data);
}
void do_load_default_presets(reg::RawOrderedRegistry<Preset>& data, std::filesystem::path const& path)
{
    try
    {
        auto is      = std::ifstream{path};
        auto archive = ser20::JSONInputArchive{is};
        archive(data);
    }
    catch (...)
    {
    }
}

} // namespace Cool

SER20_REGISTER_TYPE(Cool::NodesEditorImpl);                                     /*NOLINT*/
SER20_REGISTER_POLYMORPHIC_RELATION(Cool::INodesEditor, Cool::NodesEditorImpl); /*NOLINT*/