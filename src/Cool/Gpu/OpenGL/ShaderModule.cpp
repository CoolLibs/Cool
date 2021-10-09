#include "ShaderModule.h"
#include <exception>

namespace Cool::OpenGL {

static GLenum opengl_shader_kind(ShaderKind kind)
{
    switch (kind) {
    case ShaderKind::Vertex:
        return GL_VERTEX_SHADER;
    case ShaderKind::Fragment:
        return GL_FRAGMENT_SHADER;
    case ShaderKind::Geometry:
        return GL_GEOMETRY_SHADER;
    case ShaderKind::Compute:
        return GL_COMPUTE_SHADER;
    default:
        Log::error("[opengl_shader_kind] Unknown enum value");
        return 0;
    }
}

static void validate_shader_module(GLuint id)
{
    int result; // NOLINT
    GLDebug(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length; // NOLINT
        GLDebug(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> error_message;
        error_message.reserve(length);
        GLDebug(glGetShaderInfoLog(id, length, &length, error_message.data()));
        GLDebug(glDeleteShader(id));
        throw std::invalid_argument(std::string{"Compilation failed:\n"} + error_message.data());
    }
}

static GLuint make_shader_module(const ShaderDescription& desc)
{
    GLDebug(GLuint id = glCreateShader(opengl_shader_kind(desc.kind)));
    auto src = desc.source_code.c_str();
    GLDebug(glShaderSource(id, 1, &src, nullptr));
    GLDebug(glCompileShader(id));
    validate_shader_module(id);
    return id;
}

ShaderModule::ShaderModule(const ShaderDescription& desc)
    : _id{make_shader_module(desc)}
{
}

ShaderModule::ShaderModule(ShaderModule&& rhs) noexcept
    : _id{rhs._id}
{
    rhs._id = decltype(rhs._id){};
}

ShaderModule& ShaderModule::operator=(ShaderModule&& rhs) noexcept
{
    if (&rhs != this) {
        _id     = rhs._id;
        rhs._id = decltype(rhs._id){};
    }
    return *this;
}

ShaderModule::~ShaderModule()
{
    GLDebug(glDeleteShader(_id));
}

} // namespace Cool::OpenGL
