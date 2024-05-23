#pragma once
#include "Cool/StrongTypes/TimeSpeed.h"
#if COOL_OPENGL

#include <Cool/Midi/MidiChannel.h>
#include <Cool/OSC/OSCChannel.h>
#include <Cool/StrongTypes/Angle.h>
#include <Cool/StrongTypes/Camera2D.h>
#include <Cool/StrongTypes/Color.h>
#include <Cool/StrongTypes/ColorAndAlpha.h>
#include <Cool/StrongTypes/ColorPalette.h>
#include <Cool/StrongTypes/Direction2D.h>
#include <Cool/StrongTypes/Gradient.h>
#include <Cool/StrongTypes/Hue.h>
#include <Cool/StrongTypes/Point2D.h>
#include <Cool/TextureSource/TextureDescriptor.h>
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

    auto id() const -> GLuint { return _shader.id(); }

    void bind() const;
    void set_uniform(std::string_view uniform_name, int) const;
    void set_uniform(std::string_view uniform_name, unsigned int) const;
    void set_uniform(std::string_view uniform_name, bool) const;
    void set_uniform(std::string_view uniform_name, float) const;
    void set_uniform(std::string_view uniform_name, const glm::vec2&) const;
    void set_uniform(std::string_view uniform_name, const glm::vec3&) const;
    void set_uniform(std::string_view uniform_name, const glm::vec4&) const;
    void set_uniform(std::string_view uniform_name, const glm::uvec2&) const;
    void set_uniform(std::string_view uniform_name, const glm::uvec3&) const;
    void set_uniform(std::string_view uniform_name, const glm::uvec4&) const;
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
    void set_uniform(std::string_view uniform_name, TextureDescriptor const&) const;
    void set_uniform(std::string_view uniform_name, Texture const&, TextureSamplerDescriptor const& = {}) const;
    void set_uniform_texture(std::string_view uniform_name, GLuint texture_id, TextureSamplerDescriptor const& = {}) const;
    void set_uniform_texture1D(std::string_view uniform_name, GLuint texture_id) const;
    void set_uniform(std::string_view uniform_name, MidiChannel const&) const;
    void set_uniform(std::string_view uniform_name, OSCChannel const&) const;
    void set_uniform(std::string_view uniform_name, TimeSpeed const&) const;

private:
    GLint uniform_location(std::string_view uniform_name) const;

private:
    UniqueShader                                   _shader;
    mutable std::unordered_map<std::string, GLint> _uniform_locations;
};

} // namespace Cool::OpenGL

#include "Shader.tpp"

#endif