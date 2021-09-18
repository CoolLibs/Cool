#pragma once
#include <list>
#include "RenderableView.h"

namespace Cool {

class RenderableViewManager {
public:
    RenderableView& make_view(std::string_view name);

private:
    std::list<RenderableView> _views;
};

} // namespace Cool
