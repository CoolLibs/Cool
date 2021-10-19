#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include "Parameter.h"

namespace Cool::Internal {

class Dir3Angles : public Internal::Parameter<glm::vec2> {
public:
    Dir3Angles(const ParameterDesc<glm::vec2>& base_desc = {})
        : Parameter{base_desc}
    {
    }

    bool imgui_widget() override
    {
        return ImGuiExtras::direction_3d(name().c_str(), &_value.x, &_value.y);
    }
};

} // namespace Cool::Internal

namespace Cool::Parameter {

class Dir3 : public Cool::Internal::IParameter {
public:
    Dir3(const ParameterDesc<glm::vec2>& base_desc = {})
        : _angles{base_desc}
    {
    }

    inline const glm::vec3 operator*() const { return compute_direction(); }

    glm::vec3 compute_direction() const
    {
        return glm::vec3(
            cos(_angles->y) * cos(_angles->x),
            -cos(_angles->y) * sin(_angles->x),
            -sin(_angles->y));
    }

    bool imgui(
        Action on_edit_ended, std::function<void()> on_value_change = []() {}) override
    {
        return _angles.imgui(on_edit_ended, on_value_change);
    }

#if defined(COOL_OPENGL)
    void set_uniform_in_shader(const OpenGL::Shader& shader) const override
    {
        shader.set_uniform(name(), operator*());
    }
#endif

    inline const std::string& name() const override
    {
        return _angles.name();
    }

protected:
    bool imgui_widget() override
    {
        return _angles.imgui_widget();
    }

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(_angles);
    }

private:
    Internal::Dir3Angles _angles;
};

} // namespace Cool::Parameter