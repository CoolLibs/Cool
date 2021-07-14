#pragma once

namespace Cool {

class Action {
public:
    Action() = default;
    Action(std::function<void()> how_to_apply, std::function<void()> how_to_revert)
        : _apply(how_to_apply), _revert(how_to_revert)
    {
    }

    inline void apply() const { _apply(); }
    inline void revert() const { _revert(); }

private:
    std::function<void()> _apply  = []() {};
    std::function<void()> _revert = []() {};
};

} // namespace Cool