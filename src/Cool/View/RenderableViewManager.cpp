#include "RenderableViewManager.h"

namespace Cool {

RenderableView& RenderableViewManager::make_view(std::string_view name)
{
    return _views.emplace_back(name);
}

} // namespace Cool
