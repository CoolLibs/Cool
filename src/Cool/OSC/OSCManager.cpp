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
// TODO(OSC) rerender when new midi value arrives, if we depend on that value

auto OSCManager::get_value(OSCChannel const& channel) const -> float
{
    std::lock_guard lock{_s.values_mutex};
    for (auto const& pair : _s.values)
    {
        if (pair.first == channel.name)
            return pair.second;
    }
    return 0.f;
}

void OSCManager::for_each_channel_that_has_changed(std::function<void(OSCChannel const&)> const& callback)
{
    std::lock_guard lock{_s.channels_that_have_changed_mutex};
    for (auto const& channel : _s.channels_that_have_changed)
        callback(channel);
    _s.channels_that_have_changed.clear();
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
    _s.values.clear(); // No need to lock since the thread that might modify it has been stopped by stop_listening().
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
    if (channel.name.empty() && !_s.values.empty())
    {
        channel.name = _s.values[0].first;
        b            = true;
    }
    b |= ImGuiExtras::input_text_with_dropdown(label, &channel.name, [&](auto&& with_dropdown_entry) {
        std::lock_guard lock{_s.values_mutex};
        for (auto const& [name, _] : _s.values)
            with_dropdown_entry(name);
    });

    return b;
}

void OSCManager::imgui_window_config()
{
    _config_window.show([&]() {
        imgui_select_connection_endpoint();
        imgui_show_all_values();
        // TODO(OSC) UI to choose address
        // TODO(OSC) reset _s.values when changing address
        // TODO(OSC) button to reset _values
    });
}

void OSCManager::imgui_show_all_values()
{
    std::lock_guard       lock{_s.values_mutex};
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
        for (auto const& pair : _s.values)
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
    if (ImGuiExtras::input_text_with_dropdown("IP Address", &_endpoint.ip_address, [&](auto&& with_dropdown_entry) {
            with_dropdown_entry(OSC_EVERY_AVAILABLE_ADDRESS);
            with_dropdown_entry("localhost");
        }))
    {
        set_ip_address(_endpoint.ip_address); // TODO(OSC) Check that we don't crash when inputting invalid addresses
    }
}

namespace {
class OSCMessagesHandler : public osc::OscPacketListener {
public:
    explicit OSCMessagesHandler(internal::SharedWithThread& s)
        : _s{s}
    {}

private:
    void ProcessMessage(osc::ReceivedMessage const& m, IpEndpointName const&) override
    {
        std::lock_guard lock{_s.values_mutex};
        size_t const    index{find_index(m.AddressPattern())};
        bool            has_set_value{false};
        auto const      set_value = [&](float value) {
            _s.values[index].second = value;
            has_set_value           = true;
        };
        for (auto arg = m.ArgumentsBegin(); arg != m.ArgumentsEnd(); ++arg)
        {
            if (arg->IsFloat())
                set_value(arg->AsFloatUnchecked());
            if (arg->IsDouble())
                set_value(static_cast<float>(arg->AsDoubleUnchecked()));
            if (arg->IsInt32())
                set_value(static_cast<float>(arg->AsInt32Unchecked()));
            if (arg->IsInt64())
                set_value(static_cast<float>(arg->AsInt64Unchecked()));
            if (arg->IsChar())
                set_value(static_cast<float>(arg->AsCharUnchecked()));
            if (arg->IsBool())
                set_value(arg->AsBoolUnchecked() ? 1.f : 0.f);
        }
        if (has_set_value)
        {
            std::lock_guard lock2{_s.channels_that_have_changed_mutex};
            _s.channels_that_have_changed.emplace(m.AddressPattern());
        }
    }

    auto find_index(std::string const& name) -> size_t
    {
        for (size_t i = 0; i < _s.values.size(); ++i)
        {
            if (_s.values[i].first == name)
                return i;
        }
        _s.values.emplace_back(name, 0.f);
        return _s.values.size() - 1;
    }

private:
    internal::SharedWithThread& _s;
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
        auto listener = OSCMessagesHandler{_s};
        auto socket   = UdpListeningReceiveSocket{get_ip_endpoint_name(_endpoint), &listener};
        _stop_thread  = [&]() {
            socket.AsynchronousBreak();
        };
        socket.Run();
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