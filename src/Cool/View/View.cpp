#include "View.h"
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/Image/ImageSizeU.h>
#include <img/src/SizeU.h>
#include "Cool/Log/Debug.h"

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

void View::imgui_window(ImTextureID image_texture_id, ImageSizeInsideView image_size_inside_view, ViewWindowParams const& params)
{
    if (!_is_open)
    {
        _size.reset();
        _window_is_hovered = false;
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f}); // TODO add a parameter in the UI to control the padding specifically for the views
    { // Begin window, maybe in fullscreen
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

        display_image(image_texture_id, image_size_inside_view);

        params.extra_widgets();
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();
}

void View::imgui_open_close_checkbox()
{
    ImGuiExtras::toggle(_name.c_str(), &_is_open);
}

static auto window_to_screen(WindowCoordinates position, GLFWwindow* window) -> ScreenCoordinates
{
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        return position.as_screen_coordinates(window);
    }
    else
    {
        return ScreenCoordinates{position}; // We trick ImGui because if viewports are disabled, ImGui functions that pretend to return screen coordinates actually return window coordinates (this is a temporary measure because I know that ImGui plans on fixing this)
    }
}

static auto screen_to_view(ScreenCoordinates position, ScreenCoordinates window_position, float height) -> ViewCoordinates
{
    const auto pos = ViewCoordinates{position - window_position};
    return ViewCoordinates{
        pos.x,
        height - pos.y}; // Make y-axis point up
}

auto View::to_view_space(WindowCoordinates position, GLFWwindow* window) -> ViewCoordinates
{
    return screen_to_view(
        window_to_screen(position, window),
        _position,
        _size ? static_cast<float>(_size->height()) : 0.f
    );
}

auto View::contains(ViewCoordinates pos, ImageSizeInsideView image_size) -> bool
{
    if (!_window_is_hovered
        || ImGui::GetMouseCursor() != ImGuiMouseCursor_Arrow) // HACK: We don't dispatch events if the cursor is over the border of the window and click+drag would start resizing the window
    {
        return false;
    }

    if (!_size)
        return false;

    const auto img_size = image_size.fit_into(*_size);

    const auto pos_in_img = pos + glm::vec2{
                                (img_size.width() - static_cast<float>(_size->width())) * 0.5f,
                                (img_size.height() - static_cast<float>(_size->height())) * 0.5f,
                            };
    return pos_in_img.x >= 0.f && pos_in_img.x <= img_size.width()
           && pos_in_img.y >= 0.f && pos_in_img.y <= img_size.height();
}

void View::dispatch_mouse_move_event(const ViewEvent<MouseMoveEvent<WindowCoordinates>>& event)
{
    const auto pos = to_view_space(event.event.position, event.window);
    _mouse_event_dispatcher.drag().dispatch_mouse_move_event({pos});
    if (contains(pos, event.image_size))
    {
        _mouse_event_dispatcher.move_event().dispatch({pos});
    }
}

void View::dispatch_mouse_scroll_event(const ViewEvent<MouseScrollEvent<WindowCoordinates>>& event)
{
    const auto pos = to_view_space(event.event.position, event.window);
    if (contains(pos, event.image_size))
    {
        _mouse_event_dispatcher.scroll_event().dispatch({
            .position = pos,
            .dx       = event.event.dx,
            .dy       = event.event.dy,
        });
    }
}

void View::dispatch_mouse_button_event(const ViewEvent<MouseButtonEvent<WindowCoordinates>>& event)
{
    const auto pos          = to_view_space(event.event.position, event.window);
    const bool contains_pos = contains(pos, event.image_size);
    const auto new_event    = MouseButtonEvent<ViewCoordinates>{
           .position = pos,
           .button   = event.event.button,
           .action   = event.event.action,
           .mods     = event.event.mods};
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

        const bool has_changed = new_size != _size;
        _size                  = new_size;
        if (has_changed)
        {
            resize_event().dispatch({});
        }
    }
    else
    {
        _size.reset();
    }
}

void View::store_window_position()
{
    const auto pos = ImGui::GetCursorScreenPos();
    _position      = ScreenCoordinates{pos.x, pos.y};
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

void View::display_image(ImTextureID image_texture_id, ImageSizeInsideView image_size_inside_view)
{
    if (!_size.has_value())
        return;

    auto const size = image_size_inside_view.fit_into(*_size);
    _has_vertical_margins = img::SizeU::aspect_ratio(size) < img::SizeU::aspect_ratio(*_size);

    rerender_alpha_checkerboard_ifn(img::Size{size}, _render_target);

    // Alpha checkerboard background
    ImGuiExtras::image_centered(_render_target.imgui_texture_id(), as_imvec2(size));
    // Actual image. It needs to use straight alpha as this is what ImGui expects.
    ImGuiExtras::image_centered(image_texture_id, as_imvec2(size));
}

} // namespace Cool