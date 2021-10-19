#pragma once

#include "Parameter.h"

namespace Cool::Parameter {

struct ColorDesc {
    using Rep = glm::vec3;
    using Out = glm::vec3;

    std::string name;
    glm::vec3   default_value = glm::vec3{0.5f};

    glm::vec3 value(const glm::vec3& rep) const { return rep; }

    bool imgui(Rep& value) const
    {
        return ImGui::ColorEdit3(name.c_str(), glm::value_ptr(value), ImGuiColorEditFlags_NoInputs);
    }

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name),
                cereal::make_nvp("Default value", default_value));
    }
};

using Color = Parameter_Base<ColorDesc>;

} // namespace Cool::Parameter