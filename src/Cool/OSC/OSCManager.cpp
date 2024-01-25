#include "OSCManager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <mutex>
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/OSC/OSCConnectionEndpoint.h"
#include "ip/UdpSocket.h"
#include "osc/OscPacketListener.h"

namespace Cool {

OSCManager::OSCManager()
    : _config_window{
        Cool::icon_fmt("OSC", ICOMOON_CONNECTION),
        Cool::ImGuiWindowConfig{
            .is_modal   = false,
            .start_open = false,
        }
    }
{}

OSCManager::~OSCManager()
{
    stop_listening();
}
// TODO(OSC) rerender when new OSC value arrives, if we depend on that value

auto OSCManager::get_value(OSCChannel const& channel) const -> float
{
    std::lock_guard lock{_mutex};
    for (auto const& pair : _values)
    {
        if (pair.first == channel.name)
            return pair.second;
    }
    return 0.f;
}

void OSCManager::set_port(int port)
{
    set_connection_endpoint({port, _endpoint.ip_address});
}

void OSCManager::set_ip_address(std::string ip_address)
{
    set_connection_endpoint({_endpoint.port, std::move(ip_address)});
}

void OSCManager::set_connection_endpoint(OSCConnectionEndpoint endpoint)
{
    _endpoint = std::move(endpoint);
    // Recreate a listener using the new port and address
    stop_listening();
    _values.clear(); // No need to lock since the thread that might modify it has been stopped by stop_listening().
    start_listening();
}

auto OSCManager::get_connection_endpoint() const -> OSCConnectionEndpoint
{
    return _endpoint;
}

// TODO(OSC) Error message when an OSC node tries to use a channel name that does not exist

auto OSCManager::imgui_channel_widget(const char* label, OSCChannel& channel) const -> bool
{
    bool b = false;
    if (channel.name.empty() && !_values.empty())
    {
        channel.name = _values[0].first;
        b            = true;
    }
    // TODO(OSC) ImGuiExtras widget that does this input text + dropdown automatically
    ImGui::SetNextItemWidth(ImGuiExtras::calc_custom_dropdown_input_width());
    b |= ImGui::InputText("##Channel name", &channel.name, ImGuiInputTextFlags_None, nullptr, nullptr, ImDrawFlags_RoundCornersLeft);
    ImGui::SameLine(0.f, 0.f);
    if (ImGui::BeginCombo(label, channel.name.c_str(),
                          ImGuiComboFlags_NoPreview | ImGuiComboFlags_PopupAlignLeft, // Draw just the arrow of the dropdown
                          ImDrawFlags_RoundCornersRight))
    {
        std::lock_guard lock{_mutex};
        for (auto const& [name, _] : _values)
        {
            bool const is_selected = name == channel.name;
            if (ImGui::Selectable(name.c_str(), is_selected))
            {
                channel.name = name;
                b            = true;
            }
        }
        ImGui::EndCombo();
    }

    return b;
}

void OSCManager::imgui_window_config()
{
    _config_window.show([&]() {
        imgui_select_connection_endpoint();
        imgui_show_all_values();
        // TODO(OSC) UI to choose address
        // TODO(OSC) reset _values when changing address
        // TODO(OSC) button to reset _values
    });
}

void OSCManager::imgui_show_all_values()
{
    std::lock_guard       lock{_mutex};
    static constexpr auto table_flags = 0
                                        // | ImGuiTableFlags_Resizable
                                        | ImGuiTableFlags_RowBg
                                        | ImGuiTableFlags_Borders
                                        // | ImGuiTableFlags_HighlightHoveredColumn
                                        | ImGuiTableFlags_NoHostExtendX
                                        //
                                        | ImGuiTableFlags_SizingFixedFit
        // | ImGuiTableFlags_Resizable
        // | ImGuiTableFlags_BordersOuter
        // | ImGuiTableFlags_BordersV
        // | ImGuiTableFlags_ContextMenuInBody
        //
        ;
    if (ImGui::BeginTable("OSC Values", 2, table_flags))
    {
        // ImGui::TableSetupColumn("Address");
        // ImGui::TableSetupColumn("Value");
        // ImGui::TableHeadersRow();
        ImGui::PushFont(Cool::Font::monospace());
        for (auto const& pair : _values)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::BeginGroup();
            ImGui::TextUnformatted(pair.first.c_str());
            // ImGui::Selectable(pair.first.c_str(), false, ImGuiSelectableFlags_Disabled, ImVec2(0, ImGui::GetTextLineHeight()));
            ImGui::Dummy({ImGui::GetContentRegionAvail().x, 0.f});
            ImGui::EndGroup();
            if (ImGui::BeginPopupContextItem(pair.first.c_str()))
            {
                if (ImGui::Button("Copy to clipboard"))
                {
                    ImGui::SetClipboardText(pair.first.c_str());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            // int const column = 0;
            // ImGui::PushID(column);
            // if (ImGui::BeginPopup(pair.first.c_str()))
            // {
            //     if (ImGui::Button("Copy to clipboard"))
            //     {
            //         ImGui::SetClipboardText(pair.first.c_str());
            //         ImGui::CloseCurrentPopup();
            //     }
            //     ImGui::EndPopup();
            // }
            // ImGui::PopID();
            ImGui::TableSetColumnIndex(1);
            ImGui::BeginGroup();
            // ImGui::Text("%.3f", pair.second);
            ImGui::TextUnformatted(fmt::format("{}", pair.second).c_str());
            // ImGui::Selectable(pair.first.c_str(), false, ImGuiSelectableFlags_Disabled, ImVec2(0, ImGui::GetTextLineHeight()));
            ImGui::Dummy({ImGui::GetContentRegionAvail().x, 0.f});
            ImGui::EndGroup();
            // if (ImGui::BeginItemTooltip())
            // {
            //     ImGui::TextUnformatted(fmt::format("{}", pair.second).c_str());
            //     ImGui::EndTooltip();
            // }
            if (ImGui::BeginPopupContextItem(&pair.second))
            {
                if (ImGui::Button("Copy to clipboard"))
                {
                    ImGui::SetClipboardText(fmt::format("{}", pair.second).c_str());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::PopFont();
        ImGui::EndTable();
    }
}

void OSCManager::imgui_select_connection_endpoint()
{
    if (!_endpoint.is_valid())
    {
        ImGui::PushFont(Font::italic());
        ImGui::TextUnformatted("OSC listener is OFF. Select a valid port to start listening to OSC messages.");
        ImGui::PopFont();
    }
    // Port
    ImGui::PushItemWidth(ImGui::CalcTextSize("00000").x + ImGui::GetStyle().FramePadding.x); // Enough width for any 5-digit number.
    if (ImGui::InputInt("Port", &_endpoint.port, -1, -1, ImGuiInputTextFlags_AutoSelectAll))
        set_port(_endpoint.port);
    ImGui::PopItemWidth();
    // Address
}

namespace {
class OSCMessagesHandler : public osc::OscPacketListener {
public:
    OSCMessagesHandler(std::vector<std::pair<std::string, float>>& values, std::mutex& mutex)
        : _values{values}
        , _mutex{mutex}
    {}

private:
    void ProcessMessage(osc::ReceivedMessage const& m, IpEndpointName const&) override
    {
        std::lock_guard lock{_mutex};
        size_t const    index{find_index(m.AddressPattern())};
        for (auto arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg)
        {
            if (arg->IsFloat())
                _values[index].second = arg->AsFloatUnchecked();
            if (arg->IsDouble())
                _values[index].second = static_cast<float>(arg->AsDoubleUnchecked());
            if (arg->IsInt32())
                _values[index].second = static_cast<float>(arg->AsInt32Unchecked());
            if (arg->IsInt64())
                _values[index].second = static_cast<float>(arg->AsInt64Unchecked());
            if (arg->IsChar())
                _values[index].second = static_cast<float>(arg->AsCharUnchecked());
            if (arg->IsBool())
                _values[index].second = arg->AsBoolUnchecked() ? 1.f : 0.f;
        }
    }

    auto find_index(std::string const& name) -> size_t
    {
        for (size_t i = 0; i < _values.size(); ++i)
        {
            if (_values[i].first == name)
                return i;
        }
        _values.emplace_back(name, 0.f);
        return _values.size() - 1;
    }

private:
    std::vector<std::pair<std::string, float>>& _values;
    std::mutex&                                 _mutex;
};
} // namespace
// TODO(OSC) Crash when changing port too fast

static auto get_ip_endpoint_name(OSCConnectionEndpoint const& endpoint) -> IpEndpointName
{
    return endpoint.ip_address == OSC_EVERY_AVAILABLE_ADDRESS
               ? IpEndpointName(IpEndpointName::ANY_ADDRESS, endpoint.port)
               : IpEndpointName(endpoint.ip_address.c_str(), endpoint.port);
}

void OSCManager::start_listening()
{
    if (!_endpoint.is_valid())
        return;

    _thread = std::thread([&]() {
        auto listener = OSCMessagesHandler{_values, _mutex};
        auto s        = UdpListeningReceiveSocket{get_ip_endpoint_name(_endpoint), &listener};
        _stop_thread  = [&]() {
            s.AsynchronousBreak();
        };
        s.Run();
    });
}

void OSCManager::stop_listening()
{
    if (!_thread.has_value())
        return;
    _stop_thread();
    if (_thread->joinable())
        _thread->join();
    _thread.reset();
}

} // namespace Cool