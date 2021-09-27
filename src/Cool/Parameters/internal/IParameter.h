#pragma once

#include <Cool/Gpu/OpenGL/Shader.h> // TODO can't include OpenGL specific things
#include <Cool/History/Action.h>

namespace Cool::Internal {

/**
 * @brief Interface that all parameters should implement
 * 
 */
class IParameter {
public:
    virtual ~IParameter() = default;

    virtual const std::string& name() const = 0;
    virtual bool               imgui(
                      Action on_edit_ended, std::function<void()> on_value_change = []() {}) = 0;
#if defined(__COOL_APP_OPENGL)
    virtual void set_uniform_in_shader(OpenGL::Shader& shader) = 0;
#endif

protected:
    virtual bool imgui_widget() = 0;
};

} // namespace Cool::Internal