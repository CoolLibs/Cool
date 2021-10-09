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

    /**
     * @brief Creates and compiles a full shader pipeline made out of a vertex and a fragment shader.
     * Throws if there is an error while compiling the shader source code
     */
    Shader(std::string_view vertex_shader_file_path, std::string_view fragment_shader_file_path);
    ~Shader();
    Shader(Shader&& o) noexcept;
    Shader& operator=(Shader&& o) noexcept;

    /// <summary>
    /// Binds the shader pipeline.
    /// </summary>
    void bind() const;

    void set_uniform(std::string_view uniform_name, int v);
    void set_uniform(std::string_view uniform_name, unsigned int v);
    void set_uniform(std::string_view uniform_name, bool v);
    void set_uniform(std::string_view uniform_name, float v);
    void set_uniform(std::string_view uniform_name, const glm::vec2& v);
    void set_uniform(std::string_view uniform_name, const glm::vec3& v);
    void set_uniform(std::string_view uniform_name, const glm::vec4& v);
    void set_uniform(std::string_view uniform_name, const glm::mat2& mat);
    void set_uniform(std::string_view uniform_name, const glm::mat3& mat);
    void set_uniform(std::string_view uniform_name, const glm::mat4& mat);

private:
    std::unordered_map<const char*, GLint> _uniform_locations;
    GLint                                  uniform_location(const char* uniform_name);

private:
    GLuint _program_id;

    template<unsigned int, unsigned int, unsigned int>
    friend class ComputeShader;
};

} // namespace Cool::OpenGL

#endif