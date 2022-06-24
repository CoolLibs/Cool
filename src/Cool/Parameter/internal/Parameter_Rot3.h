#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include "Parameter_Base.h"

namespace Cool::Parameter {

struct Rot3Desc {
    using Value         = glm::vec3;
    using InternalValue = glm::vec3;

    std::string name;
    glm::vec3   default_value = glm::vec3(0.f);

    glm::vec3 value(const glm::vec3& rep) const { return rep; }

    bool imgui(glm::vec3& value) const
    {
        ImGui::PushID(this);
        bool b = ImGuiExtras::angle_wheel("Angle X", &value.x);
        b |= ImGuiExtras::angle_wheel("Angle Y", &value.y);
        b |= ImGuiExtras::angle_wheel("Angle Z", &value.z);
        ImGui::PopID();

        return b;
    }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name),
                cereal::make_nvp("Default", default_value));
    }
};

using Rot3 = Parameter_Base<Rot3Desc>;

} // namespace Cool::Parameter
