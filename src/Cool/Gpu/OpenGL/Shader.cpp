#include "Cool/StrongTypes/ColorAndAlpha.h"
#include "imgui.h"
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

static auto as_glm(ImVec4 const& v) -> glm::vec4
{
    return {v.x, v.y, v.z, v.w};
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

static auto operator*(const ImVec4& lhs, const float rhs) -> ImVec4
{
    return ImVec4{lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs};
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
void Shader::set_uniform(std::string_view uniform_name, Color const& color) const
{
    set_uniform(uniform_name, color.as_srgb());
}
void Shader::set_uniform(std::string_view uniform_name, ColorAndAlpha const& color) const
{
    set_uniform(uniform_name, color.as_srgb_straight());
}
void Shader::set_uniform(std::string_view uniform_name, Direction2D direction) const
{
    set_uniform(uniform_name, direction.as_unit_vec2());
}
void Shader::set_uniform(std::string_view uniform_name, Hue hue) const
{
    set_uniform(uniform_name, hue.from_0_to_1());
}
void Shader::set_uniform(std::string_view uniform_name, const ColorPalette& palette) const
{
    int idx = 0;
    for (auto const& color : palette.value)
    {
        set_uniform(fmt::format("{}[{}]", Cool::internal::color_palette_array_name(uniform_name), idx), color);
        idx++;
    }
}
void Shader::set_uniform(std::string_view uniform_name, const ImGG::ColorRGBA& v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform4f(uniform_location(uniform_name), v.x, v.y, v.z, v.w));
}
void Shader::set_uniform(std::string_view uniform_name, const Gradient& gradient) const
{
    int idx = 0;
    for (ImGG::Mark const& mark : gradient.value.gradient().get_marks())
    {
        set_uniform(
            fmt::format("{}[{}].pos", Cool::internal::gradient_marks_array_name(uniform_name), idx),
            mark.position.get()
        );
        set_uniform(
            fmt::format("{}[{}].col", Cool::internal::gradient_marks_array_name(uniform_name), idx),
            ColorAndAlpha::from_srgb_straight_alpha(as_glm(mark.color)).as_cielab_premultiplied()
        );
        idx++;
    }
}
void Shader::set_uniform(std::string_view uniform_name, Point2D point2D) const
{
    set_uniform(uniform_name, point2D.value);
}

} // namespace Cool::OpenGL

#endif