#pragma once
#if COOL_OPENGL

#include <Cool/StrongTypes/Angle.h>
#include <Cool/StrongTypes/Camera2D.h>
#include <Cool/StrongTypes/Color.h>
#include <Cool/StrongTypes/ColorAndAlpha.h>
#include <Cool/StrongTypes/ColorPalette.h>
#include <Cool/StrongTypes/Direction2D.h>
#include <Cool/StrongTypes/Gradient.h>
#include <Cool/StrongTypes/Hue.h>
#include <Cool/StrongTypes/Point2D.h>
#include <Cool/StrongTypes/TextureWrapper.h>
#include "ShaderModule.h"
#include "UniqueShader.h"

namespace Cool::OpenGL {

class Shader {
public:
    /**
     * @brief Throws if there is an error while linking the modules.
     */
    template<typename... ShaderModules>
    explicit Shader(ShaderModules&&... shader_modules);

    void bind() const;
    void set_uniform(std::string_view uniform_name, int) const;
    void set_uniform(std::string_view uniform_name, unsigned int) const;
    void set_uniform(std::string_view uniform_name, bool) const;
    void set_uniform(std::string_view uniform_name, float) const;
    void set_uniform(std::string_view uniform_name, const glm::vec2&) const;
    void set_uniform(std::string_view uniform_name, const glm::vec3&) const;
    void set_uniform(std::string_view uniform_name, const glm::vec4&) const;
    void set_uniform(std::string_view uniform_name, const glm::mat2&) const;
    void set_uniform(std::string_view uniform_name, const glm::mat3&) const;
    void set_uniform(std::string_view uniform_name, const glm::mat4&) const;
    void set_uniform(std::string_view uniform_name, Angle) const;
    void set_uniform(std::string_view uniform_name, Color const&) const;
    void set_uniform(std::string_view uniform_name, ColorAndAlpha const&) const;
    void set_uniform(std::string_view uniform_name, Direction2D) const;
    void set_uniform(std::string_view uniform_name, Hue) const;
    void set_uniform(std::string_view uniform_name, const ColorPalette&) const;
    void set_uniform(std::string_view uniform_name, const ImGG::ColorRGBA&) const;
    void set_uniform(std::string_view uniform_name, const Gradient&) const;
    void set_uniform(std::string_view uniform_name, Point2D) const;
    void set_uniform(std::string_view uniform_name, Camera2D const&) const;
    void set_uniform(std::string_view uniform_name, TextureWrapper const&) const;

private:
    GLint uniform_location(std::string_view uniform_name) const;

private:
    UniqueShader                                   _shader;
    mutable std::unordered_map<std::string, GLint> _uniform_locations;

    template<unsigned int, unsigned int, unsigned int>
    friend class ComputeShader;
};

} // namespace Cool::OpenGL

#include "Shader.tpp"

#endif