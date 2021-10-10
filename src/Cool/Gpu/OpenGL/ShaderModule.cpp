#if defined(COOL_OPENGL)

#include "ShaderModule.h"
#include <exception>
#include "preprocess_shader_source.h"

namespace Cool::OpenGL {

static void validate_shader_module(GLuint id, const std::string& name)
{
    int result; // NOLINT
    GLDebug(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        GLsizei length; // NOLINT
        GLDebug(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> error_message;
        error_message.reserve(length);
        GLDebug(glGetShaderInfoLog(id, length, nullptr, error_message.data()));
        throw std::invalid_argument(std::string{name + "\nCompilation failed:\n"} + error_message.data());
    }
}

static void compile_shader_module(GLuint id, const ShaderDescription& desc)
{
    auto preprocessed_source = preprocess_shader_source(desc.source_code);
    auto src                 = preprocessed_source.c_str();
    GLDebug(glShaderSource(id, 1, &src, nullptr));
    GLDebug(glCompileShader(id));
    validate_shader_module(id, desc.name);
}

ShaderModule::ShaderModule(const ShaderDescription& desc)
    : _shader_module{desc.kind}
{
    compile_shader_module(_shader_module.id(), desc);
}

} // namespace Cool::OpenGL

#endif