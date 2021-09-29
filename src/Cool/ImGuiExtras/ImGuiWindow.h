#pragma once

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
    void open() { _is_open = true; }
    void close() { _is_open = false; }

private:
    const char* _title;
    bool        _is_open;
};

} // namespace Cool
