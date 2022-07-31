#include "DefaultApp.h"
#include <Cool/Input/Input.h>
#include <Cool/Log/ToUser.h>
#include <Cool/Parameter/ParametersHistory.h>
#include <Cool/Time/ClockU.h>

namespace CoolDefault {

DefaultApp::DefaultApp(Cool::WindowManager& windows, std::function<void(Cool::RenderTarget&, float time)> render_fn)
    : _main_window{windows.main_window()}
    , _view{_views.make_view("View")}
    , _render_fn{render_fn}
{
    _camera.hook_events(_view.view.mouse_events());
}

void DefaultApp::update()
{
    if (!_exporter.is_exporting())
    {
        _clock.update();
        for (auto& view : _views)
        {
            view.update_size(_preview_constraint);
        }
        polaroid().render(_clock.time());
    }
    else
    {
        _exporter.update(polaroid());
    }
}

Cool::Polaroid DefaultApp::polaroid()
{
    return {
        .render_target = _view.render_target,
        .render_fn     = _render_fn};
}

bool DefaultApp::inputs_are_allowed() const
{
    return !_exporter.is_exporting();
}

bool DefaultApp::wants_to_show_menu_bar() const
{
    return !_exporter.is_exporting();
}

void DefaultApp::imgui_windows()
{
    // Views
    for (auto& view : _views)
    {
        view.imgui_window();
    }
    // Exporter
    _exporter.imgui_windows(polaroid(), _clock.time());
    // Consoles
    Cool::Log::ToUser::console().imgui_window();
#if DEBUG
    Cool::Log::Debug::console().imgui_window();
#endif
    //
    if (inputs_are_allowed())
    {
        // Time
        ImGui::Begin("Time");
        Cool::ClockU::imgui_timeline(_clock);
        ImGui::End();
        // Camera
        ImGui::Begin("Camera");
        _camera.imgui();
        ImGui::End();
#if DEBUG
        if (_show_imgui_debug)
        {
            ImGui::Begin("Debug", &_show_imgui_debug);
            ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
            _main_window.imgui_cap_framerate();
            ImGui::Checkbox("Show Demo Window", &_show_imgui_demo);
            ImGui::End();
        }
        if (_show_imgui_demo)
        { // Show the big demo window (Most of the sample code is
            // in ImGui::ShowDemoWindow()! You can browse its code
            // to learn more about Dear ImGui!).
            ImGui::ShowDemoWindow(&_show_imgui_demo);
        }
#endif
    }
}

void DefaultApp::menu_preview()
{
    if (ImGui::BeginMenu("Preview"))
    {
        _preview_constraint.imgui();
        ImGui::EndMenu();
    }
}

void DefaultApp::menu_windows()
{
    if (ImGui::BeginMenu("Windows"))
    {
        for (auto& view : _views)
        {
            view.view.imgui_open_close_checkbox();
        }
#if DEBUG
        ImGui::Separator();
        ImGui::Checkbox("Debug", &_show_imgui_debug);
#endif
        ImGui::EndMenu();
    }
}

void DefaultApp::menu_export()
{
    if (ImGui::BeginMenu("Export"))
    {
        _exporter.imgui_menu_items();
        ImGui::EndMenu();
    }
}

void DefaultApp::imgui_menus()
{
    menu_preview();
    menu_windows();
    menu_export();
}

void DefaultApp::on_keyboard_event(const Cool::KeyboardEvent& event)
{
    if (event.action == GLFW_RELEASE)
    {
        if (Cool::UserInput::matches_char("s", event.key) && event.mods.ctrl())
        {
            _exporter.image_export_window().open();
        }
        if (Cool::UserInput::matches_char("e", event.key) && event.mods.ctrl())
        {
            _exporter.video_export_window().open();
        }
    }
    if (event.action == GLFW_PRESS || event.action == GLFW_REPEAT)
    {
        if (Cool::UserInput::matches_char("z", event.key) && event.mods.ctrl())
        {
            Cool::ParametersHistory::get().move_backward();
        }
        if (Cool::UserInput::matches_char("y", event.key) && event.mods.ctrl())
        {
            Cool::ParametersHistory::get().move_forward();
        }
    }
}

void DefaultApp::on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views)
    {
        view.view.dispatch_mouse_button_event(view_event(event, view));
    }
}

void DefaultApp::on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views)
    {
        view.view.dispatch_mouse_scroll_event(view_event(event, view));
    }
}

void DefaultApp::on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event)
{
    for (auto& view : _views)
    {
        view.view.dispatch_mouse_move_event(view_event(event, view));
    }
}

} // namespace CoolDefault
