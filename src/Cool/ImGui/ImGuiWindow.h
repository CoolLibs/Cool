#pragma once

#include <Cool/EventDispatcher/EventDispatcher.h>

namespace Cool {

class ImGuiWindow {
public:
    explicit ImGuiWindow(const char* title, bool is_open = true)
        : _title{title}, _is_open{is_open}
    {
    }

    /**
     * @brief Shows the windows if it is open
     * 
     * @param widgets A function that renders all the desired widgets
     */
    void show(std::function<void()> widgets);
    void open()
    {
        _is_open = true;
        on_open().dispatch({});
    }
    void close() { _is_open = false; }

    void open_close_checkbox();

    struct OpenEvent {
    };

    auto on_open() -> EventDispatcher<OpenEvent>& { return _open_event_dispatcher; }

private:
    const char*                _title;
    bool                       _is_open;
    EventDispatcher<OpenEvent> _open_event_dispatcher;
};

} // namespace Cool
