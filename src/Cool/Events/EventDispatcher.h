#pragma once

namespace Cool {

template<typename Event>
class EventDispatcher {
public:
    void receive(const Event& event)
    {
        for (const auto& callback : _callbacks) {
            callback(event);
        }
    }

    void subscribe(std::function<void(const Event&)> callback)
    {
        _callbacks.push_back(callback);
    }

private:
    std::vector<std::function<void(const Event&)>> _callbacks = {};
};

} // namespace Cool
