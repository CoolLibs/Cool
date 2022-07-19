#pragma once
#include <Cool/File/File.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/NfdFileFilter/NfdFileFilter.h>
#include "CommandCore/CommandExecutor_TopLevel_Ref.h"
#include "Commands/Command_FinishedEditingVariable.h"
#include "Commands/Command_SetVariable.h"
#include "Commands/Command_SetVariableMetadata.h"
#include "Input.h"
#include "VariableRegistries.h"

namespace Cool {

class Ui_Ref {
public:
    Ui_Ref(
        VariableRegistries&               registries,
        Lab::CommandExecutor_TopLevel_Ref command_executor,
        SetDirty_Ref                      set_dirty
    )
        : _variable_registries{registries}
        , _command_executor{command_executor}
        , _set_dirty{set_dirty}
    {
    }

    struct WindowParams {
        std::string_view name;
    };

    template<typename UiFunction>
    static void window(const WindowParams& params, UiFunction&& ui_function)
    {
        ImGui::Begin(params.name.data());
        ui_function();
        ImGui::End();
    }

    template<typename T>
    void widget(const Input<T>& input)
    {
        ImGui::PushID(&input);
        // TODO add a way to change the current_variable
        // And show the UI of the current_variable if it is set
        const auto variable = _variable_registries.get().get(input._default_variable_id);
        if (variable)
        {
            widget<T>(input._default_variable_id, *variable);
        }
        ImGui::PopID();
    }

    void widget(Input_File& input)
    {
        input.update(_set_dirty); // TODO shouldn't be in the Ui, should be called all the frames even if we don't render the Ui
        std::string path = input.file_watcher.path().string();
        ImGui::Text("Path : ");
        ImGui::SameLine();
        ImGui::PushID(2132541);
        if (ImGui::InputText("", &path))
        {
            input.file_watcher.set_path(path);
            _set_dirty(input._dirty_flag);
        }
        ImGui::PopID();
        ImGui::SameLine();
        if (Cool::ImGuiExtras::open_file_dialog(&path, Cool::NfdFileFilter::FragmentShader, Cool::File::whithout_file_name(path)))
        {
            input.file_watcher.set_path(path);
            _set_dirty(input._dirty_flag);
        }
    }

    void set_dirty(const DirtyFlag flag) { _set_dirty(flag); }

private:
    template<typename T>
    void widget(const VariableId<T>& id, Cool::Variable<T> variable)
    {
        Cool::imgui(
            variable,
            {
                .on_value_changed =
                    [&]() { _command_executor.execute(
                                Lab::Command_SetVariable<T>{.id = id, .value = variable.value}
                            ); },

                .on_metadata_changed =
                    [&]() { _command_executor.execute(
                                Lab::Command_SetVariableMetadata<T>{.id = id, .metadata = variable.metadata}
                            ); },

                .on_value_editing_finished =
                    [&]() { _command_executor.execute(
                                Lab::Command_FinishedEditingVariable{}
                            ); },
            }
        );
    }

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
    Lab::CommandExecutor_TopLevel_Ref          _command_executor;
    SetDirty_Ref                               _set_dirty;
};

} // namespace Cool