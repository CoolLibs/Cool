#pragma once
#include <list>
#include "View.h"

namespace Cool {

class ViewManager {
public:
    View& make_view(std::string_view name);

private:
    std::list<View> _views;
};

} // namespace Cool
