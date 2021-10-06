#pragma once

#include "ImageSizeInsideView.h"

namespace Cool {

template<typename Event>
struct ViewEvent {
    Event               event;
    GLFWwindow*         window;
    ImageSizeInsideView image_size;
};

} // namespace Cool
