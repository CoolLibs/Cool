#if defined(COOL_OPENGL)
#include "ShaderModule.h"
#include "Cool/Exception/Exception.h"
#include "Cool/Log/ErrorMessage.hpp"
#include "preprocess_shader_source.h"
#include "tl/expected.hpp"

namespace Cool::OpenGL {

[[nodiscard]] static auto validate_shader_module(GLuint id) -> std::optional<std::string>
{
    {
        int result; // NOLINT
        GLDebug(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
        if (result)
            return {}; // Compilation successful
    }

    // Handle error:
    GLsizei length; // NOLINT
    GLDebug(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    std::vector<GLchar> error_message;
    error_message.resize(static_cast<size_t>(length));
    GLDebug(glGetShaderInfoLog(id, length, nullptr, error_message.data()));
    return error_message.data();
}

static auto compile_shader_module(GLuint id, const ShaderDescription& desc) -> tl::expected<void, ErrorMessage>
{
    auto const preprocessed_source = preprocess_shader_source(desc.source_code);
    if (!preprocessed_source)
        return tl::make_unexpected(ErrorMessage{
            fmt::format("Preprocessing failed:\n{}\nThe source code we tried to preprocess was:\n{}", preprocessed_source.error(), desc.source_code),
            std::vector<ClipboardContent>{
                {.title = "shader code", .content = desc.source_code},
                {.title = "error message", .content = preprocessed_source.error()},
            }
        });

    char const* src = preprocessed_source->c_str();
    GLDebug(glShaderSource(id, 1, &src, nullptr));
    GLDebug(glCompileShader(id));
    auto const err = validate_shader_module(id);
    if (err)
    {
        return tl::make_unexpected(ErrorMessage{
            fmt::format("Compilation failed:\n{}\nThe source code we tried to compile was:\n{}", *err, *preprocessed_source),
            std::vector<ClipboardContent>{
                {.title = "shader code", .content = *preprocessed_source},
                {.title = "error message", .content = *err},
            }
        });
    }
    return {};
}

ShaderModule::ShaderModule(const ShaderDescription& desc)
    : _shader_module{desc.kind}
{
    auto const result = compile_shader_module(_shader_module.id(), desc);
    if (!result.has_value())
        throw Cool::Exception{result.error()};
}

} // namespace Cool::OpenGL

#endif