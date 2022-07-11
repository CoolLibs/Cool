

// namespace Cool {

// static auto button_size() -> float
// {
//     return ImGui::GetFrameHeight();
// }

// auto must_be_in_same_line(size_t index) -> bool
// {
//     const auto palette_width = std::max(static_cast<size_t>(1u), static_cast<size_t>((ImGui::CalcItemWidth() - button_size()) / button_size()));
//     return (index % palette_width != 0);
// }

// auto imgui_color_palette_widget(std::string_view name, Cool::ColorPalette& palette, ImGuiColorEditFlags flags) -> bool
// {
//     static std::string debug             = "";
//     bool               value_has_changed = false;

//     ImGui::Text("%s", name.data());
//     ImGui::BeginGroup();

//     std::optional<size_t> new_index = std::nullopt;
//     std::optional<size_t> old_index = std::nullopt;

//     for (size_t i = 0; i < palette.value.size(); i++)
//     {
//         ImGui::PushID(static_cast<int>(i));
//         if (must_be_in_same_line(i))
//         {
//             ImGui::SameLine();
//         }
//         value_has_changed |= ImGui::ColorEdit3(
//             "##1",
//             glm::value_ptr(palette.value[i].value),
//             ImGuiColorEditFlags_NoInputs | flags
//         );
//         if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle) &&
//             ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
//         {
//             palette.remove_color(i);
//         }

//         if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
//         {
//             old_index = i;
//             ImGui::SetDragDropPayload("##2", &old_index, sizeof(old_index));
//             ImGui::EndDragDropSource();
//         }

//         if (ImGui::BeginDragDropTarget() && !ImGui::IsMouseReleased(ImGuiMouseButton_Left))
//         {
//             if (ImGui::AcceptDragDropPayload("##2", ImGuiDragDropFlags_AcceptBeforeDelivery))
//             {
//                 new_index = i;
//                 ImGui::SetDragDropPayload("##2", &new_index, sizeof(new_index));
//                 if (new_index.has_value() && old_index.has_value())
//                 {
//                     palette.move_color(*old_index, *new_index);
//                     old_index         = new_index;
//                     new_index         = std::nullopt;
//                     value_has_changed = true;
//                 }
//             }
//             ImGui::EndDragDropTarget();
//         }

//         ImGui::PopID();
//     }

//     if (new_index.has_value() && old_index.has_value())
//     {
//         ImGui::Text("%llu", *new_index);
//         ImGui::Text("%llu", *old_index);
//     }

//     ImGui::EndGroup();
//     ImGui::SameLine();
//     if (ImGui::Button("+", ImVec2(button_size(), button_size())))
//     {
//         palette.add_color();
//         ImGui::OpenPopup("picker");
//         value_has_changed = true;
//     }
//     if (ImGui::BeginPopup("picker"))
//     {
//         ImGui::SetNextItemWidth(button_size() * 12.0f);
//         value_has_changed |= ImGui::ColorPicker3("##picker", glm::value_ptr(palette.value[palette.value.size() - 1].value), flags);
//         ImGui::EndPopup();
//     }
//     Cool::ImGuiExtras::tooltip("Add a color");
//     Cool::ImGuiExtras::tooltip("Remove a color by middle click on it");

//     ImGui::Separator();
//     const auto* ptr = ImGui::GetDragDropPayload();
//     if (ptr)
//     {
//         const auto drag_index_i = *reinterpret_cast<const size_t*>(ptr->Data);
//         ImGui::Text("%llu", drag_index_i);
//     }
//     else
//     {
//         ImGui::Text(".");
//     }
//     if (ImGui::Button("Clear"))
//     {
//         debug = "";
//     }
//     ImGui::Text("%s", debug.c_str());

//     return value_has_changed;
// }

// } // namespace Cool