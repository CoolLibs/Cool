namespace Cool {

template<typename Desc>
bool Parameter_Base<Desc>::imgui(Action on_edit_ended, std::function<void()> on_value_change)
{
    bool b = _desc.imgui(_value);
    ImGui::PushID(this);
    if (ImGui::BeginPopupContextItem(_desc.name.c_str())) {
        if (ImGui::Selectable("Reset parameter")) {
            _value = _desc.default_value;
            push_change_in_history(on_edit_ended, on_value_change);
        }
        ImGui::EndPopup();
    }
    ImGui::PopID();
    push_change_in_history_if_edit_ended(on_edit_ended, on_value_change);
    if (b) {
        on_value_change();
    }
    return b;
}

template<typename Desc>
void Parameter_Base<Desc>::push_change_in_history_if_edit_ended(Action on_edit_ended, std::function<void()> on_value_change)
{
    if (ImGui::IsItemDeactivatedAfterEdit()) {
        push_change_in_history(on_edit_ended, on_value_change);
    }
}

template<typename Desc>
void Parameter_Base<Desc>::push_change_in_history(Action on_edit_ended, std::function<void()> on_value_change)
{
    ParametersHistory::get().begin_undo_group();
    const auto val      = _value;
    const auto prev_val = _value_before_edit;
    ParametersHistory::get().add_action({[&, val, on_value_change]() {
                                             _value = val;
                                             on_value_change();
                                         },
                                         [&, prev_val, on_value_change]() {
                                             _value = prev_val;
                                             on_value_change();
                                         }});
    ParametersHistory::get().add_action(on_edit_ended);
    ParametersHistory::get().end_undo_group();
    on_edit_ended.apply();
    _value_before_edit = _value; // ready for next edit
}

} // namespace Cool