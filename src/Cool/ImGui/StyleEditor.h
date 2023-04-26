#pragma once

#include "Cool/ImGui/ImGuiExtrasStyle.h"
namespace Cool {

class StyleEditor {
public:
    StyleEditor();
    ~StyleEditor();

    void imgui();

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("ImGui Style", ImGui::GetStyle()),
            cereal::make_nvp("ImGuiExtras Style", ImGuiExtras::GetStyle())
        );
    }
};

} // namespace Cool