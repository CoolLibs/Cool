#if defined(COOL_OPENGL)

#include "ShaderModule.h"
#include <Cool/Log/OptionalErrorMessage.h>
#include <exception>
#include "preprocess_shader_source.h"

namespace Cool::OpenGL {

static void validate_shader_module(GLuint id)
{
    int result; // NOLINT
    GLDebug(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        GLsizei length; // NOLINT
        GLDebug(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> error_message;
        error_message.reserve(static_cast<size_t>(length));
        GLDebug(glGetShaderInfoLog(id, length, nullptr, error_message.data()));
        throw std::invalid_argument(std::string{"Compilation failed:\n"} + error_message.data());
    }
}

static auto compile_shader_module(GLuint id, const ShaderDescription& desc) -> std::optional<std::string>
{
    auto preprocessed_source = preprocess_shader_source(desc.source_code);
    if (!preprocessed_source)
        return preprocessed_source.error();

    auto src = preprocessed_source->c_str();
    GLDebug(glShaderSource(id, 1, &src, nullptr));
    GLDebug(glCompileShader(id));
    validate_shader_module(id);
    return {};
}

ShaderModule::ShaderModule(const ShaderDescription& desc)
    : _shader_module{desc.kind}
{
    auto const err = compile_shader_module(_shader_module.id(), desc);
    if (err)
        throw std::runtime_error{*err};
}

} // namespace Cool::OpenGL

#endif