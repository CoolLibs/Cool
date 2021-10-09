#pragma once
#if defined(COOL_OPENGL)

#include <Cool/Gpu/ShaderDescription.h>

namespace Cool::OpenGL {

class Shader {
public:
    /**
     * @brief Throws if there is an error while compiling the shader source code.
     */
    explicit Shader(const std::vector<ShaderDescription>& shader_descriptions);
    ~Shader();
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& o) noexcept;
    Shader& operator=(Shader&& o) noexcept;

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
    GLuint                                         _program_id;
    mutable std::unordered_map<std::string, GLint> _uniform_locations;

    template<unsigned int, unsigned int, unsigned int>
    friend class ComputeShader;
};

} // namespace Cool::OpenGL

#endif