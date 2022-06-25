#pragma once

#include "AnyParameter.h"
#include "ParametersHistory.h"
#include "internal/Presets.h"

namespace Cool {

/**
 * @brief Wraps a struct made of Parameter::xxx and handles presets for that struct
 *
 * @tparam ParameterValues This struct must implement a "bool imgui(Action on_edit_ended, std::function<void()> on_value_change)" method as well as the cereal serialization
 */
template<typename ParameterValues>
class ParametersWithPresets {
public:
    /**
     * @brief
     *
     * @param file_extension The common identifier for the files storing the presets
     * @param folder_path The folder where the presets will be stored
     * @param on_value_change A callback that will be trigerred whenever a value changes
     */
    ParametersWithPresets(
        const std::string& file_extension, const std::string& folder_path, std::function<void()> on_value_change = []() {}
    )
        : _presets(file_extension, folder_path)
        , _on_value_change(on_value_change)
    {
    }

    /**
     * @brief Displays the imgui widgets for all the parameters, as well as the presets menu
     *
     * @return true iff a value was modified from the UI. /!\ You should rely on the callback passed in the constructor to check for value changes, because this boolean will not be set to true when you move around the history, but the callback will be triggered properly
     */
    bool imgui()
    {
        const auto   uuid   = _presets.last_uuid();
        const Action action = {
            [&]() {
                _presets.set_to_placeholder_setting();
                _presets.discard_last_uuid();
            },
            [&, uuid]() {
                _presets.compute_current_preset_idx(uuid);
                _presets.set_last_uuid(uuid);
            },
        };
        bool b = false;
        if (b |= _values.imgui(action, _on_value_change))
        {
            _presets.set_to_placeholder_setting();
        }
        ImGui::Separator();
        if (b |= _presets.imgui(&_values, _on_value_change))
        {
            _on_value_change();
        }
        return b;
    }
    inline const ParameterValues&       operator*() const { return _values; }
    inline const ParameterValues* const operator->() const { return &_values; }

private:
    ParameterValues          _values;
    Presets<ParameterValues> _presets;
    std::function<void()>    _on_value_change;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            _values,
            _presets
        );
    }
};

} // namespace Cool