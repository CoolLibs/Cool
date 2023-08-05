#include "WebcamRequest.h"
#include "Cool/Webcam/WebcamsConfigs.h"

namespace Cool {

WebcamRequest::WebcamRequest(std::optional<size_t> index, std::string const& name)
    : _name(name)
{
    if (index.has_value())
    {
        _capture = std::make_unique<WebcamCapture>(
            *index,
            WebcamsConfigs::instance().selected_resolution(name)
        );
    }
}

} // namespace Cool