#pragma once
#include <list>
#include "RenderableView.h"

namespace Cool {

class RenderableViewManager {
public:
    RenderableView& make_view(std::string_view name);
    auto            begin() { return _views.begin(); }
    auto            end() { return _views.end(); }
    auto            begin() const { return _views.begin(); }
    auto            end() const { return _views.end(); }

private:
    std::list<RenderableView> _views;
};

} // namespace Cool
