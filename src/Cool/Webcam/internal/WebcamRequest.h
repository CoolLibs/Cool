#pragma once
#include "WebcamCapture.h"

namespace Cool {

struct WebcamRequest {
    explicit WebcamRequest(std::optional<size_t> const& index, std::string name)
        : _name(name)
    {
        if (index.has_value())
            create_capture(*index);
    }

    void create_capture(size_t index);

    std::string                    _name;
    bool                           has_been_requested_this_frame = true;
    Cool::MessageId                _iderror_cannot_find_webcam;
    std::unique_ptr<WebcamCapture> _capture;
};

} // namespace Cool