#if defined(COOL_OPENGL)

#include "Shader.h"
#include <Cool/File/File.h>
#include "ShaderModule.h"

namespace Cool::OpenGL {

#if defined(DEBUG)
#define ASSERT_SHADER_IS_BOUND              \
    GLint id;                               \
    glGetIntegerv(GL_CURRENT_PROGRAM, &id); \
    assert(id == _program_id && "The shader must be bound before setting any uniform");
#else
#define ASSERT_SHADER_IS_BOUND
#endif

Shader::Shader(const std::vector<ShaderDescription>& shader_descriptions)
{
    GLDebug(_program_id = glCreateProgram());
    // Compile shaders
    std::vector<ShaderModule> shader_modules;
    shader_modules.reserve(shader_descriptions.size());
    for (const auto& shader_desc : shader_descriptions) {
        shader_modules.emplace_back(shader_desc);
        GLDebug(glAttachShader(_program_id, shader_modules.back().id()));
    }
    // Link
    GLDebug(glLinkProgram(_program_id));
    GLDebug(glValidateProgram(_program_id));
}

Shader::Shader(std::string_view vertex_shader_file_path, std::string_view fragment_shader_file_path)
    : Shader({ShaderDescription{std::string{vertex_shader_file_path}, ShaderKind::Vertex},
              ShaderDescription{std::string{fragment_shader_file_path}, ShaderKind::Fragment}})
{
}

Shader::Shader(Shader&& o) noexcept
    : _program_id(o._program_id)
{
    o._program_id = decltype(o._program_id){};
}

Shader& Shader::operator=(Shader&& o) noexcept
{
    if (&o != this) {
        _program_id   = o._program_id;
        o._program_id = decltype(o._program_id){};
    }
    return *this;
}

Shader::~Shader()
{
    glDeleteProgram(_program_id);
}

void Shader::bind() const
{
    GLDebug(glUseProgram(_program_id));
}

GLint Shader::uniform_location(const char* uniform_name)
{
    if (_uniform_locations.find(uniform_name) != _uniform_locations.end()) {
        return _uniform_locations[uniform_name];
    }
    GLDebug(GLint location = glGetUniformLocation(_program_id, uniform_name));
    _uniform_locations[uniform_name] = location;
    return location;
}

void Shader::set_uniform(std::string_view uniform_name, int v)
{
    ASSERT_SHADER_IS_BOUND;
    GLDebug(glUniform1i(uniform_location(uniform_name.data()), v));
}
void Shader::set_uniform(std::string_view uniform_name, unsigned int v)
{
    set_uniform(uniform_name, static_cast<int>(v));
}
void Shader::set_uniform(std::string_view uniform_name, bool v)
{
    set_uniform(uniform_name, v ? 1 : 0);
}
void Shader::set_uniform(std::string_view uniform_name, float v)
{
    ASSERT_SHADER_IS_BOUND;
    GLDebug(glUniform1f(uniform_location(uniform_name.data()), v));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec2& v)
{
    ASSERT_SHADER_IS_BOUND;
    GLDebug(glUniform2f(uniform_location(uniform_name.data()), v.x, v.y));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec3& v)
{
    ASSERT_SHADER_IS_BOUND;
    GLDebug(glUniform3f(uniform_location(uniform_name.data()), v.x, v.y, v.z));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec4& v)
{
    ASSERT_SHADER_IS_BOUND;
    GLDebug(glUniform4f(uniform_location(uniform_name.data()), v.x, v.y, v.z, v.w));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat2& mat)
{
    ASSERT_SHADER_IS_BOUND;
    GLDebug(glUniformMatrix2fv(uniform_location(uniform_name.data()), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat3& mat)
{
    ASSERT_SHADER_IS_BOUND;
    GLDebug(glUniformMatrix3fv(uniform_location(uniform_name.data()), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat4& mat)
{
    ASSERT_SHADER_IS_BOUND;
    GLDebug(glUniformMatrix4fv(uniform_location(uniform_name.data()), 1, GL_FALSE, glm::value_ptr(mat)));
}

} // namespace Cool::OpenGL

#endif