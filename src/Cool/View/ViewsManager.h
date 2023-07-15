#pragma once
#include <list>
#include <memory>
#include "View.h"

namespace Cool {

/// Contains all the Views of the application.
class ViewsManager {
public:
    template<typename ViewT, typename... Args>
    auto make_view(Args... args) -> ViewT&
    {
        return static_cast<ViewT&>(*_views.emplace_back(std::make_unique<ViewT>(std::forward<Args>(args)...)));
    }

    [[nodiscard]] auto begin() { return _views.begin(); } // TODO(JF) Provide an iterator that returns a View& instead of std::unique_ptr<View>&
    [[nodiscard]] auto end() { return _views.end(); }
    [[nodiscard]] auto begin() const { return _views.begin(); }
    [[nodiscard]] auto end() const { return _views.end(); }

private:
    std::list<std::unique_ptr<View>> _views;
};

} // namespace Cool
