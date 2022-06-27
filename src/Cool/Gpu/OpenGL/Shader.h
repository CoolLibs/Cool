#pragma once
#if defined(COOL_OPENGL)

#include "Cool/StrongTypes/Angle.h"
#include "Cool/StrongTypes/RgbColor.h"
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
    void set_uniform(std::string_view uniform_name, RgbColor) const;

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