#pragma once
#include "WebcamCapture.h"

namespace Cool {

struct WebcamRequest {
    WebcamRequest(std::optional<size_t> index, std::string const& name);

    std::string                    _name;
    bool                           has_been_requested_this_frame = true;
    Cool::MessageId                _iderror_cannot_find_webcam;
    std::unique_ptr<WebcamCapture> _capture;
};

} // namespace Cool