#pragma once
#include "WebcamCapture.h"

namespace Cool {

struct WebcamRequest {
    WebcamRequest(std::optional<size_t> index, std::string const& webcam_name);

    std::string                    webcam_name;
    bool                           has_been_requested_this_frame{true};
    std::unique_ptr<WebcamCapture> capture; // Optional capture. If the webcam doesn't exist it will be nullptr.
};

} // namespace Cool