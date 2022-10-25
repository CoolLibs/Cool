#if defined(COOL_OPENGL)

#include "FullscreenPipeline.h"
#include <Cool/File/File.h>
#include <Cool/Gpu/OpenGL/preprocess_shader_source.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Path/Path.h>

namespace Cool::OpenGL {

std::optional<ShaderModule>& vertex_module()
{
    static std::optional<ShaderModule> shader_module = ShaderModule{{
        *File::to_string(Path::cool_res() / "shaders/fullscreen.vert")
             .map_error([](const std::string& error_message) {
                 Cool::Log::ToUser::error(
                     "FullscreenPipeline::vertex_module()",
                     "Couldn't load fullscreen shader. Please fix this and then restart the app.\n" + error_message
                 );
                 return tl::expected<std::string, std::string>{"VERTEX SHADER FILE NOT FOUND"};
             }),
        ShaderKind::Vertex,
    }};
    return shader_module;
}

void FullscreenPipeline::shut_down()
{
    vertex_module().reset();
}

auto FullscreenPipeline::compile(std::string_view fragment_shader_source_code) -> OptionalErrorMessage
{
    const auto on_error = [&]() {
        _shader.reset();
    };

    if (fragment_shader_source_code.empty())
    {
        on_error();
        return OptionalErrorMessage{"Shader is empty."};
    }

    try
    {
        _shader = Shader{
            *vertex_module(),
            ShaderModule{{
                std::string{fragment_shader_source_code},
                ShaderKind::Fragment,
            }}};
        return {};
    }
    catch (const std::exception& e)
    {
        on_error();
        const auto preprocessed_source = preprocess_shader_source(fragment_shader_source_code);
        return OptionalErrorMessage{
            e.what()
            + (preprocessed_source
                   ? std::string{"\nThe source code we tried to compile was:\n"} + *preprocessed_source
                   : "" // Log nothing because we know the exception already contains the error message from the preprocessing failure.
            )};
    }
}

void FullscreenPipeline::draw() const
{
    if (_shader.has_value())
    {
        // We use a smart trick to render fullscreen, as explained here : https://stackoverflow.com/a/59739538
        _shader->bind();
        GLDebug(glBindVertexArray(_dummy_vao.id()));
        GLDebug(glDrawArrays(GL_TRIANGLES, 0, 3));
    }
}

} // namespace Cool::OpenGL

#endif