#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include "Parameter.h"

namespace Cool::Parameter {

struct Dir3Desc {
    using Rep = glm::vec2;
    using Out = glm::vec3;

    std::string name;
    glm::vec2   default_value = glm::vec2{0.f};

    glm::vec3 value(glm::vec2 rep) const
    {
        return glm::vec3{
            +cos(rep.y) * cos(rep.x),
            -cos(rep.y) * sin(rep.x),
            -sin(rep.y)};
    }

    bool imgui(Rep& value) const
    {
        return ImGuiExtras::direction_3d(name.c_str(), &value.x, &value.y);
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

using Dir3 = Parameter_Base<Dir3Desc>;

} // namespace Cool::Parameter