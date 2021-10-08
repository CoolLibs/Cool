#if defined(COOL_OPENGL)

#include "../Shader.h"
#include <Cool/File/File.h>

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
    create_program(shader_descriptions);
}

Shader::Shader(std::string_view vertex_shader_file_path, std::string_view fragment_shader_file_path)
    : Shader({ShaderDescription{ShaderSource{vertex_shader_file_path}, ShaderKind::Vertex},
              ShaderDescription{ShaderSource{fragment_shader_file_path}, ShaderKind::Fragment}})
{
}

Shader::Shader(Shader&& o) noexcept
    : _program_id(o._program_id)
{
    o._program_id = 0;
}

Shader& Shader::operator=(Shader&& o) noexcept
{
    if (&o != this) {
        _program_id   = o._program_id;
        o._program_id = 0;
    }
    return *this;
}

Shader::~Shader()
{
    glDeleteProgram(_program_id);
}

void Shader::create_program(const std::vector<ShaderDescription>& shader_descriptions)
{
    // Create program
    if (_program_id != 0) {
        GLDebug(glDeleteProgram(_program_id));
        _uniform_locations.clear();
    }
    GLDebug(_program_id = glCreateProgram());
    // Compile shaders
    std::vector<GLuint> shader_ids;
    shader_ids.reserve(shader_descriptions.size());
    for (const auto& shader_desc : shader_descriptions) {
        shader_ids.push_back(CreateShader(shader_desc));
        GLDebug(glAttachShader(_program_id, shader_ids.back()));
    }
    // Link
    GLDebug(glLinkProgram(_program_id));
    GLDebug(glValidateProgram(_program_id));
    // Delete shaders
    for (auto shader_id : shader_ids) {
        glDeleteShader(shader_id);
    }
}

void Shader::create_program(std::string_view vertex_shader_file_path, std::string_view fragment_shader_file_path)
{
    create_program({ShaderDescription{ShaderSource{File::to_string(vertex_shader_file_path)}, ShaderKind::Vertex},
                    ShaderDescription{ShaderSource{File::to_string(fragment_shader_file_path)}, ShaderKind::Fragment}});
}

GLuint Shader::CreateShader(const ShaderDescription& shader_description)
{
    // Get shader type
    const GLenum shader_type = [&]() {
        switch (shader_description.kind) {
        case ShaderKind::Vertex:
            return GL_VERTEX_SHADER;
        case ShaderKind::Fragment:
            return GL_FRAGMENT_SHADER;
        case ShaderKind::Geometry:
            return GL_GEOMETRY_SHADER;
        case ShaderKind::Compute:
            return GL_COMPUTE_SHADER;
        default:
            Log::error("Unknown shader type !");
            return 0;
        }
    }();
    // Create
    GLDebug(GLuint shader_id = glCreateShader(shader_type));
    // Compile
    const char* src = shader_description.source_code.glsl_source().data();
    GLDebug(glShaderSource(shader_id, 1, &src, nullptr));
    GLDebug(glCompileShader(shader_id));
// Debug
#if defined(DEBUG)
    int result; // NOLINT
    GLDebug(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length; // NOLINT
        GLDebug(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)); // NOLINT
        GLDebug(glGetShaderInfoLog(shader_id, length, &length, message));
        Log::error("Shader compilation failed :\n{}", message);
        GLDebug(glDeleteShader(shader_id));
    }
#endif
    return shader_id;
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