#include "Pin.h"
#include <imgui-node-editor/imgui_node_editor.h>
#include "UniqueImNodeContext.h"

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
    ImNodes::BeginPin(ImNodes::PinId{this}, ImNodes::PinKind::Input);
    show_impl();
    ImNodes::EndPin();
}

void OutputPin::show() const
{
    ImNodes::BeginPin(ImNodes::PinId{this}, ImNodes::PinKind::Output);
    show_impl();
    ImNodes::EndPin();
}

} // namespace Cool
