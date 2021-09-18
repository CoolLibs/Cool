#include "ViewManager.h"

namespace Cool {

View& ViewManager::make_view(std::string_view name)
{
    return _views.emplace_back(name);
}

} // namespace Cool
