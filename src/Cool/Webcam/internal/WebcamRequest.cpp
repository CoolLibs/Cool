#include "WebcamRequest.h"
#include "Cool/Webcam/WebcamsConfigs.h"

namespace Cool {

WebcamRequest::WebcamRequest(std::optional<size_t> index, std::string const& webcam_name)
    : webcam_name{webcam_name}
{
    if (index.has_value())
    {
        capture = std::make_unique<WebcamCapture>(
            *index,
            WebcamsConfigs::instance().selected_resolution(webcam_name)
        );
    }
}

} // namespace Cool