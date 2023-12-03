#pragma once

#include <Cool/EventDispatcher/EventDispatcher.h>

namespace Cool {

struct ImGuiWindowConfig {
    bool is_modal   = false;
    bool start_open = false;
};

class ImGuiWindow {
public:
    explicit ImGuiWindow(std::string title, ImGuiWindowConfig const& config = {})
        : _title{std::move(title)}
        , _is_modal{config.is_modal}
        , _is_open{config.start_open}
    {
    }

    /**
     * @brief Shows the windows if it is open
     *
     * @param widgets A function that renders all the desired widgets
     */
    void show(std::function<void()> widgets);
    void open();
    void close();

    void open_close_toggle();

    struct OpenEvent {
    };

    auto on_open() -> EventDispatcher<OpenEvent>& { return _open_event_dispatcher; }

private:
    std::string                _title;
    bool                       _is_modal;
    EventDispatcher<OpenEvent> _open_event_dispatcher;

    bool _is_open;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Is open", _is_open)
        );
    }
};

} // namespace Cool
