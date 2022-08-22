#if defined(COOL_OPENGL)

#include "FullscreenPipeline.h"
#include <Cool/File/File.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Path/Path.h>

namespace Cool::OpenGL {

std::optional<ShaderModule>& vertex_module()
{
    static std::optional<ShaderModule> shader_module = ShaderModule{{
        File::to_string(Path::cool_res() + "/shaders/fullscreen.vert"),
        ShaderKind::Vertex,
        "Cool's fullscreen vertex shader",
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
        return {"Shader is empty."};
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
        return OptionalErrorMessage{
            e.what() +
            std::string{"\nThe source code we tried to compile was:\n"} +
            std::string{fragment_shader_source_code}};
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