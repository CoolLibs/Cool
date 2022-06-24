#pragma once

#include <Cool/AppManager/IApp.h>
#include <Cool/Camera/CameraManager.h>
#include <Cool/Exporter/Exporter.h>
#include <Cool/Exporter/internal/Polaroid.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Image/ImageSizeConstraint.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Time/Clock_Realtime.h>
#include <Cool/View/RenderableViewManager.h>
#include <Cool/Window/WindowManager.h>

namespace CoolDefault {

class DefaultApp : public Cool::IApp {
public:
    DefaultApp(Cool::WindowManager& windows, std::function<void(Cool::RenderTarget&, float time)> render_fn);

    void update() override;
    bool inputs_are_allowed() const override;
    bool wants_to_show_menu_bar() const override;
    void imgui_windows() override;
    void imgui_menus() override;

    void on_keyboard_event(const Cool::KeyboardEvent& event) override;
    void on_mouse_button(const Cool::MouseButtonEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_scroll(const Cool::MouseScrollEvent<Cool::WindowCoordinates>& event) override;
    void on_mouse_move(const Cool::MouseMoveEvent<Cool::WindowCoordinates>& event) override;

protected:
    Cool::Polaroid polaroid();

    void menu_preview();
    void menu_windows();
    void menu_export();
    void menu_settings();

private:
    template<typename Event>
    Cool::ViewEvent<Event> view_event(const Event& event, const Cool::RenderableView& view)
    {
        return {event,
                _main_window.glfw(),
                {view.render_target.current_size()}};
    }

protected:
    Cool::Window&               _main_window;
    Cool::CameraManager         _camera;
    Cool::Clock_Realtime        _clock;
    Cool::ImageSizeConstraint   _preview_constraint;
    Cool::RenderableViewManager _views; // Must be before the views because it is used to construct them
    Cool::RenderableView&       _view;
    Cool::Exporter              _exporter;

    std::function<void(Cool::RenderTarget&, float time)> _render_fn;
#if DEBUG
    bool _show_imgui_debug = true;
    bool _show_imgui_demo  = false;
#endif

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Camera Manager", _camera));
    }
};

} // namespace CoolDefault
