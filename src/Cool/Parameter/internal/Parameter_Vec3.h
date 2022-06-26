#pragma once

#include "Parameter_Base.h"

namespace Cool::Parameter {

struct Vec3Desc {
    using Value         = glm::vec3;
    using InternalValue = glm::vec3;

    std::string name;
    glm::vec3   default_value = glm::vec3{0.f};
    glm::vec3   min_value     = glm::vec3{0.f};
    glm::vec3   max_value     = glm::vec3{1.f};

    glm::vec3 value(const glm::vec3& rep) const { return rep; }

    bool imgui(glm::vec3& value) const
    {
        ImGui::PushItemWidth(150);
        ImGui::PushID(this);
        bool b = ImGui::SliderFloat("##1", &value.x, min_value.x, max_value.x);
        ImGui::SameLine();
        b |= ImGui::SliderFloat("##2", &value.y, min_value.y, max_value.y);
        ImGui::SameLine();
        b |= ImGui::SliderFloat(name.c_str(), &value.z, min_value.z, max_value.z);
        ImGui::PopID();
        ImGui::PopItemWidth();

        return b;
    }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", name),
            cereal::make_nvp("Default", default_value),
            cereal::make_nvp("Min", min_value),
            cereal::make_nvp("Max", max_value)
        );
    }
};

using Vec3 = Parameter_Base<Vec3Desc>;

} // namespace Cool::Parameter