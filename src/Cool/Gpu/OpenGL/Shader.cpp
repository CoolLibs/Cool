#if defined(COOL_OPENGL)

#include "Shader.h"
#include "ShaderModule.h"

namespace Cool::OpenGL {

static void assert_shader_is_bound(GLuint id)
{
#if DEBUG
    GLint current_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_id);
    assert(static_cast<GLuint>(current_id) == id && "The shader must be bound before setting any uniform.");
#else
    (void)id;
#endif
}

void Shader::bind() const
{
    GLDebug(glUseProgram(_shader.id()));
}

GLint Shader::uniform_location(std::string_view uniform_name) const
{
    const auto name = std::string{uniform_name};
    const auto it   = _uniform_locations.find(name);
    if (it != _uniform_locations.end())
    {
        return it->second;
    }
    else
    {
        GLDebug(GLint location = glGetUniformLocation(_shader.id(), name.c_str()));
        _uniform_locations[name] = location;
        return location;
    }
}

void Shader::set_uniform(std::string_view uniform_name, int v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform1i(uniform_location(uniform_name), v));
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
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform1f(uniform_location(uniform_name), v));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec2& v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform2f(uniform_location(uniform_name), v.x, v.y));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec3& v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform3f(uniform_location(uniform_name), v.x, v.y, v.z));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec4& v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform4f(uniform_location(uniform_name), v.x, v.y, v.z, v.w));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat2& mat) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniformMatrix2fv(uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat3& mat) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniformMatrix3fv(uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat4& mat) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniformMatrix4fv(uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, Angle angle) const
{
    set_uniform(uniform_name, angle.as_radians());
}

void Shader::set_uniform(std::string_view uniform_name, RgbColor color) const
{
    set_uniform(uniform_name, color.value);
}

void Shader::set_uniform(std::string_view uniform_name, Direction2D direction) const
{
    set_uniform(uniform_name, direction.as_unit_vec2());
}

void Shader::set_uniform(std::string_view uniform_name, Hue hue) const
{
    set_uniform(uniform_name, hue.get());
}

void Shader::set_uniform(std::string_view uniform_name, ColorPalette palette) const
{
    set_uniform(uniform_name, palette.value);
}

} // namespace Cool::OpenGL

#endif