#if defined(COOL_OPENGL)

#include "Shader.h"
#include "ShaderModule.h"

namespace Cool::OpenGL {

static void assert_shader_is_bound(GLuint id)
{
#if defined(DEBUG)
    GLint current_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_id);
    assert(current_id == id && "The shader must be bound before setting any uniform.");
#endif
}

void validate_shader(GLuint id)
{
    GLDebug(glValidateProgram(id));
    GLint result;
    GLDebug(glGetProgramiv(id, GL_VALIDATE_STATUS, &result));
    if (result == GL_FALSE) {
        GLsizei length;
        GLDebug(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> error_message;
        error_message.reserve(length);
        GLDebug(glGetProgramInfoLog(id, length, nullptr, error_message.data()));
        GLDebug(glDeleteProgram(id));
        throw std::invalid_argument(std::string{"Linking failed:\n"} + error_message.data());
    }
}

static GLuint make_shader(const std::vector<ShaderDescription>& shader_descriptions)
{
    GLDebug(GLuint id = glCreateProgram());
    for (const auto& shader_desc : shader_descriptions) {
        GLDebug(glAttachShader(id, ShaderModule{shader_desc}.id()));
    }
    GLDebug(glLinkProgram(id));
    validate_shader(id);
    return id;
}

Shader::Shader(const std::vector<ShaderDescription>& shader_descriptions)
    : _program_id{make_shader(shader_descriptions)}
{
}

Shader::Shader(Shader&& rhs) noexcept
    : _program_id(rhs._program_id)
{
    rhs._program_id = decltype(rhs._program_id){};
}

Shader& Shader::operator=(Shader&& rhs) noexcept
{
    if (&rhs != this) {
        _program_id     = rhs._program_id;
        rhs._program_id = decltype(rhs._program_id){};
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

GLint Shader::uniform_location(const char* uniform_name) const
{
    auto it = _uniform_locations.find(uniform_name);
    if (it != _uniform_locations.end()) {
        return it->second;
    }
    else {
        GLDebug(GLint location = glGetUniformLocation(_program_id, uniform_name));
        _uniform_locations[uniform_name] = location;
        return location;
    }
}

void Shader::set_uniform(std::string_view uniform_name, int v) const
{
    assert_shader_is_bound(_program_id);
    GLDebug(glUniform1i(uniform_location(uniform_name.data()), v));
}
void Shader::set_uniform(std::string_view uniform_name, unsigned int v) const
{
    set_uniform(uniform_name, static_cast<int>(v));
}
void Shader::set_uniform(std::string_view uniform_name, bool v) const
{
    set_uniform(uniform_name, v ? 1 : 0);
}
void Shader::set_uniform(std::string_view uniform_name, float v) const
{
    assert_shader_is_bound(_program_id);
    GLDebug(glUniform1f(uniform_location(uniform_name.data()), v));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec2& v) const
{
    assert_shader_is_bound(_program_id);
    GLDebug(glUniform2f(uniform_location(uniform_name.data()), v.x, v.y));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec3& v) const
{
    assert_shader_is_bound(_program_id);
    GLDebug(glUniform3f(uniform_location(uniform_name.data()), v.x, v.y, v.z));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec4& v) const
{
    assert_shader_is_bound(_program_id);
    GLDebug(glUniform4f(uniform_location(uniform_name.data()), v.x, v.y, v.z, v.w));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat2& mat) const
{
    assert_shader_is_bound(_program_id);
    GLDebug(glUniformMatrix2fv(uniform_location(uniform_name.data()), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat3& mat) const
{
    assert_shader_is_bound(_program_id);
    GLDebug(glUniformMatrix3fv(uniform_location(uniform_name.data()), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat4& mat) const
{
    assert_shader_is_bound(_program_id);
    GLDebug(glUniformMatrix4fv(uniform_location(uniform_name.data()), 1, GL_FALSE, glm::value_ptr(mat)));
}

} // namespace Cool::OpenGL

#endif