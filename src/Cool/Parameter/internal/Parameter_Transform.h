#pragma once

#include "Cool/Guizmo3D/GuizmoGlobalState.h"
#include "Parameter_Base.h"

namespace Cool::Parameter {

struct TransformDesc {
    using Value         = glm::mat4;
    using InternalValue = glm::mat4;

    std::string name;
    glm::mat4   default_value = glm::mat4{1.f};

    glm::mat4 value(const glm::mat4& rep) const { return rep; }

    bool imgui(glm::mat4& value) const
    {
        GGS.insert_or_assign((void*)this, Cool::GuizmoState{std::reference_wrapper<glm::mat4>{value},
                                                            ImGuizmo::OPERATION::TRANSLATE,
                                                            true});
        return Cool::GGS.at((void*)this).has_changed;
    }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name));
    }
};

using Transform = Parameter_Base<TransformDesc>;

} // namespace Cool::Parameter