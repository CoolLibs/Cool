#include "OSCManager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <mutex>
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/icon_fmt.h"
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
{
    start_listening(); // TODO(OSC) Only start listening when really necessary
}
// TODO(OSC) rerender when new OSC value arrives, if we depend on that value

auto OSCManager::get_value(OSCChannel const& channel) const -> float
{
    std::lock_guard lock{_mutex};
    for (auto const& pair : _values)
    {
        if (pair.first == channel.route)
            return pair.second;
    }
    return 0.f;
}

auto OSCManager::imgui_channel_widget(const char* label, OSCChannel& channel) const -> bool
{
    bool b = false;
    // TODO(OSC) ImGuiExtras widget that does this input text + dropdown automatically
    ImGui::SetNextItemWidth(ImGuiExtras::calc_custom_dropdown_input_width());
    b |= ImGui::InputText("##Route name", &channel.route, ImGuiInputTextFlags_None, nullptr, nullptr, ImDrawFlags_RoundCornersLeft);
    ImGui::SameLine(0.f, 0.f);
    if (ImGui::BeginCombo(label, channel.route.c_str(),
                          ImGuiComboFlags_NoPreview | ImGuiComboFlags_PopupAlignLeft, // Draw just the arrow of the dropdown
                          ImDrawFlags_RoundCornersRight))
    {
        std::lock_guard lock{_mutex};
        for (auto const& [name, _] : _values)
        {
            bool const is_selected = name == channel.route;
            if (ImGui::Selectable(name.c_str(), is_selected))
            {
                channel.route = name;
                b             = true;
            }
        }
        ImGui::EndCombo();
    }

    return b;
}

void OSCManager::imgui_window_config()
{
    _config_window.show([&]() {
        imgui_show_all_values();
        // TODO(OSC) UI to choose port number, and address
        // TODO(OSC) reset _values when changing port or address
        // TODO(OSC) button to reset _values
    });
}

// TODO(OSC) ImGui function to choose which route to use for a specific OSC node. Dropdown that shows all the routes that we have received so far, + input text to choose your own route

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

namespace {
class PacketListener : public osc::OscPacketListener {
public:
    PacketListener(std::vector<std::pair<std::string, float>>& values, std::mutex& mutex)
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

void OSCManager::start_listening()
{
    _thread = std::thread([&]() {
        auto listener = PacketListener{_values, _mutex};
        auto s        = UdpListeningReceiveSocket{IpEndpointName(IpEndpointName::ANY_ADDRESS, 7000), &listener};
        s.Run(); // TODO(OSC) Listen for stop requests
    });
}

void OSCManager::stop_listening() // TODO(OSC) Call this in destructor
{
    if (!_thread.has_value())
        return;
    if (_thread->joinable())
        _thread->join();
    _thread.reset();
}

} // namespace Cool