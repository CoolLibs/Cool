#pragma once

namespace Cool {

/**
 * @brief A class that dispatches events to everyone that subscribed a callback to it.
 * 
 * @tparam Event Any struct that will be passed to the subscribers.
 */
template<typename Event>
class EventDispatcher {
public:
    void dispatch(const Event& event)
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
    std::vector<std::function<void(const Event&)>> _callbacks{};
};

} // namespace Cool
