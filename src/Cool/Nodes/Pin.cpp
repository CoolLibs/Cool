#include "Pin.h"

namespace Cool {

Pin::Pin(std::string_view name)
    : _name{name}
    , _id{reg::internal::generate_uuid()}
{
}

void Pin::show_impl() const
{
    ImGui::TextUnformatted(_name.c_str());
}

void InputPin::show() const
{
    ImNodes::BeginInputAttribute(id());
    show_impl();
    ImNodes::EndInputAttribute();
}

void OutputPin::show() const
{
    ImNodes::BeginOutputAttribute(id());
    show_impl();
    ImNodes::EndOutputAttribute();
}

} // namespace Cool
