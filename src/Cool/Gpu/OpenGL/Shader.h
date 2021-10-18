#pragma once
#if defined(COOL_OPENGL)

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
    void set_uniform(std::string_view uniform_name, int v) const;
    void set_uniform(std::string_view uniform_name, unsigned int v) const;
    void set_uniform(std::string_view uniform_name, bool v) const;
    void set_uniform(std::string_view uniform_name, float v) const;
    void set_uniform(std::string_view uniform_name, const glm::vec2& v) const;
    void set_uniform(std::string_view uniform_name, const glm::vec3& v) const;
    void set_uniform(std::string_view uniform_name, const glm::vec4& v) const;
    void set_uniform(std::string_view uniform_name, const glm::mat2& mat) const;
    void set_uniform(std::string_view uniform_name, const glm::mat3& mat) const;
    void set_uniform(std::string_view uniform_name, const glm::mat4& mat) const;

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