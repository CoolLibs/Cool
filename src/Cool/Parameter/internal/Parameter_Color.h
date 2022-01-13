#pragma once

#include "Parameter_Base.h"

namespace Cool::Parameter {

struct ColorDesc {
    using Value         = glm::vec3;
    using InternalValue = glm::vec3;

    std::string name;
    glm::vec3   default_value = glm::vec3{0.5f};

    glm::vec3 value(const glm::vec3& rep) const { return rep; }

    bool imgui(glm::vec3& value) const
    {
        ImGui::PushID(this);
        bool colImg = ImGui::ColorEdit3(name.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs);
        ImGui::PopID();
        return colImg;
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

using Color = Parameter_Base<ColorDesc>;

} // namespace Cool::Parameter