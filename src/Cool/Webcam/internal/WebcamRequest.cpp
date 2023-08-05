#include "WebcamRequest.h"

namespace Cool {

void WebcamRequest::create_capture(const size_t index)
{
    _capture = std::make_unique<WebcamCapture>(index);
}

} // namespace Cool