#include "copy_tex_pipeline.hpp"

namespace Cool {

static auto make_copy_tex_pipeline() -> Cool::OpenGL::FullscreenPipeline
{
    auto       instance = Cool::OpenGL::FullscreenPipeline{};
    auto const res      = instance.compile(R"GLSL(
#version 410
#include "_COOL_RES_/shaders/shader-utils.glsl" 
out vec4 out_Color;
uniform sampler2D tex_to_copy;
void main()
{
    out_Color = texture(tex_to_copy, _uv);
}
    )GLSL");
    res.send_error_if_any(
        [&](std::string const& message) {
            return Cool::Message{
                .category = "Internal",
                .message  = "Failed to create shader to copy texture:\n" + message,
                .severity = Cool::MessageSeverity::Error,
            };
        },
        Cool::Log::ToUser::console()
    );

    return instance;
}

auto copy_tex_pipeline() -> Cool::OpenGL::FullscreenPipeline&
{
    static auto instance = make_copy_tex_pipeline();
    return instance;
}

} // namespace Cool