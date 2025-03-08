#if defined(COOL_OPENGL)
#include "FullscreenPipeline.h"
#include "Cool/Exception/Exception.h"
#include "Cool/File/File.h"
#include "Cool/Path/Path.h"
#include "tl/expected.hpp"

namespace Cool::OpenGL {

static auto vertex_module() -> std::optional<ShaderModule>&
{
    static std::optional<ShaderModule> shader_module = ShaderModule{{
        .kind        = ShaderKind::Vertex,
        .source_code = *File::to_string(Path::cool_res() / "shaders/fullscreen.vert")
                            .map_error([](std::string const& error_message) {
                                Log::internal_error(
                                    "FullscreenPipeline::vertex_module()",
                                    "Couldn't load fullscreen shader. Please fix this and then restart the app.\n" + error_message
                                );
                                return tl::expected<std::string, std::string>{"VERTEX SHADER FILE NOT FOUND"};
                            }),
    }};
    return shader_module;
}

auto FullscreenPipeline::compile(std::string_view fragment_shader_source_code) -> tl::expected<void, ErrorMessage>
{
    const auto on_error = [&]() {
        _shader.reset();
    };

    if (fragment_shader_source_code.empty())
    {
        on_error();
        return tl::make_unexpected(ErrorMessage{"Shader is empty."});
    }

    try
    {
        _shader = Shader{
            *vertex_module(),
            ShaderModule{{
                ShaderKind::Fragment,
                std::string{fragment_shader_source_code},
            }}
        };
        return {};
    }
    catch (Cool::Exception const& e)
    {
        on_error();
        return tl::make_unexpected(ErrorMessage{e.error_message()});
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