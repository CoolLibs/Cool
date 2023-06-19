#include "View.h"
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>
#include <img/src/SizeU.h>
#include "Cool/Input/MouseCoordinates.h"
#include "Cool/Log/Debug.h"
#include "Cool/Log/Message.h"
#include "Cool/Log/ToUser.h"

namespace Cool {

static auto create_alpha_checkerboard_pipeline() -> FullscreenPipeline
{
    auto       pipeline = FullscreenPipeline{};
    auto const err      = pipeline.compile(R"STR(#version 410
out vec4 out_Color;
layout(location = 0) in vec2 _uv;
uniform float _aspect_ratio;

void main()
{
    vec2 uv = _uv - 0.5;
    uv.x *= _aspect_ratio;
    ivec2 gid = ivec2(floor(uv * 20.));
    float grey = ((gid.x + gid.y) % 2 == 0) ? 0.25 : 0.5;
    out_Color = vec4(vec3(grey), 1.);
}
)STR");
#if DEBUG
    err.send_error_if_any(
        [&](std::string const& message) {
            return Message{
                .category = "Alpha Checkerboard Shader",
                .message  = message,
                .severity = MessageSeverity::Error,
            };
        },
        Cool::Log::Debug::console()
    );
#else
    (void)err;
#endif

    return pipeline;
}

static auto alpha_checkerboard_pipeline() -> FullscreenPipeline const&
{
    static auto pipeline = create_alpha_checkerboard_pipeline();
    return pipeline;
}

void View::imgui_window(ViewWindowParams const& params)
{
    if (!_is_open)
    {
        _window_size.reset();
        _window_is_hovered = false;
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f}); // TODO add a parameter in the UI to control the padding specifically for the views
    {                                                             // Begin window, maybe in fullscreen
        bool* const            p_open = _is_closable ? &_is_open : nullptr;
        ImGuiWindowFlags const flags  = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        if (params.fullscreen)
            ImGuiExtras::begin_fullscreen(_name.c_str(), p_open, flags);
        else
            ImGui::Begin(_name.c_str(), p_open, flags);
    }
    ImGui::PopStyleVar();
    ImGui::PushStyleColor(ImGuiCol_NavHighlight, {0.f, 0.f, 0.f, 0.f});             // Hack because when escaping view's fullscreen with the ESCAPE key it gets nav-highlighted.
    ImGui::BeginChild("##ChildWindow", {0.f, 0.f}, false, ImGuiWindowFlags_NoMove); // Hack to emulate `ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;` for this window only. Since we can drag a camera in a View we don't want the window to move at the same time.
    {
        store_window_size();
        store_window_position();
        _window_is_hovered = ImGui::IsWindowHovered();

        display_image(get_image_texture_id(), get_image_size());

        params.extra_widgets();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();
}

void View::imgui_open_close_toggle()
{
    ImGuiExtras::toggle(_name.c_str(), &_is_open);
}

auto View::contains(ViewCoordinates pos) const -> bool
{
    if (!_window_is_hovered
        || !_window_size
        || ImGui::GetMouseCursor() != ImGuiMouseCursor_Arrow // HACK: We don't dispatch events if the cursor is over the border of the window and click+drag would start resizing the window
    )
    {
        return false;
    }

    auto const aspect_ratio = img::SizeU::aspect_ratio(get_image_size());
    return -1.f < pos.y && pos.y < 1.f
           && -aspect_ratio < pos.x && pos.x < aspect_ratio;
}

auto View::to_view_coordinates(ImGuiCoordinates const pos) const -> ViewCoordinates
{
    if (!_window_size)
        return ViewCoordinates{};
    auto const window_size = glm::vec2{_window_size->width(), _window_size->height()};

    auto res = glm::vec2{pos};

    res -= _window_position + window_size / 2.f;

    auto const img_size = img::SizeU::fit_into(*_window_size, get_image_size());
    res /= img_size.height() / 2.f;
    res.y *= -1.f;

    return ViewCoordinates{res};
}

void View::dispatch_mouse_move_event(MouseMoveEvent<ImGuiCoordinates> const& event)
{
    auto const pos = to_view_coordinates(event.position);
    _mouse_event_dispatcher.drag().dispatch_mouse_move_event({pos});
    _mouse_event_dispatcher.move_event().dispatch({pos});

    if (DebugOptions::log_mouse_position_in_view())
    {
        Log::ToUser::console().send(
            _log_position_message_id,
            Message{
                .category = _name,
                .message  = fmt::format("Mouse at ({:.2f}, {:.2f})", pos.x, pos.y),
                .severity = MessageSeverity::Info,
            }
        );
    }
}

void View::dispatch_mouse_scroll_event(MouseScrollEvent<ImGuiCoordinates> const& event)
{
    auto const pos = to_view_coordinates(event.position);
    if (!contains(pos))
        return;

    _mouse_event_dispatcher.scroll_event().dispatch({
        .position = pos,
        .dx       = event.dx,
        .dy       = event.dy,
    });
}

void View::dispatch_mouse_button_event(MouseButtonEvent<ImGuiCoordinates> const& event)
{
    auto const pos          = to_view_coordinates(event.position);
    bool const contains_pos = contains(pos);
    auto const new_event    = MouseButtonEvent<ViewCoordinates>{
           .position = pos,
           .button   = event.button,
           .action   = event.action,
    };
    _mouse_event_dispatcher.drag().dispatch_mouse_button_event(new_event, contains_pos);
    if (contains_pos)
    {
        _mouse_event_dispatcher.button_event().dispatch(new_event);
    }
}

void View::store_window_size()
{
    const auto size = ImGui::GetContentRegionAvail();
    if (size.x >= 1.f && size.y >= 1.f)
    {
        const auto new_size = std::make_optional(img::Size{
            static_cast<img::Size::DataType>(size.x),
            static_cast<img::Size::DataType>(size.y),
        });

        const bool has_changed = new_size != _window_size;
        _window_size           = new_size;
        // if (has_changed)
        // {
        //     resize_event().dispatch({});
        // }
    }
    else
    {
        _window_size.reset();
    }
}

void View::store_window_position()
{
    _window_position = ImGuiCoordinates{ImGui::GetCursorScreenPos()};
}

static auto as_imvec2(img::SizeT<float> size) -> ImVec2
{
    return {size.width(), size.height()};
}

static void rerender_alpha_checkerboard_ifn(img::Size size, RenderTarget& render_target)
{
    if (size == render_target.current_size())
        return;

    render_target.set_size(size);
    render_target.render([&]() {
        alpha_checkerboard_pipeline().shader()->bind();
        alpha_checkerboard_pipeline().shader()->set_uniform("_aspect_ratio", img::SizeU::aspect_ratio(size));
        alpha_checkerboard_pipeline().draw();
    });

    if (Cool::DebugOptions::log_when_rendering_alpha_checkerboard_background())
        Cool::Log::ToUser::info("Alpha Checkerboard", "Rendered");
}

void View::display_image(ImTextureID image_texture_id, img::Size image_size)
{
    if (!_window_size.has_value())
        return;

    auto const size       = img::SizeU::fit_into(*_window_size, image_size);
    _has_vertical_margins = img::SizeU::aspect_ratio(size) < img::SizeU::aspect_ratio(*_window_size);

    rerender_alpha_checkerboard_ifn(img::Size{size}, _render_target);

    // Alpha checkerboard background
    ImGuiExtras::image_centered(_render_target.imgui_texture_id(), as_imvec2(size));
    // Actual image. It needs to use straight alpha as this is what ImGui expects.
    ImGuiExtras::image_centered(image_texture_id, as_imvec2(size));
}

} // namespace Cool