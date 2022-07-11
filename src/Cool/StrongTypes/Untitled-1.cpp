

// namespace Cool {

// static auto button_size() -> float
// {
//     return ImGui::GetFrameHeight();
// }

// auto must_be_in_same_line(size_t index) -> bool
// {
//     const float width         = ImGui::CalcItemWidth();
//     const auto  palette_width = std::max(size_t(1u), static_cast<size_t>((width - button_size()) / button_size()));
//     return (index % palette_width != 0);
// }

// struct PayloadData {
//     Cool::RgbColor        color;
//     std::optional<size_t> current_idx;
// };

// auto imgui_color_palette_widget(std::string_view name, Cool::ColorPalette& palette, ImGuiColorEditFlags flags) -> bool
// {
//     static std::string debug             = "";
//     bool               value_has_changed = false;
//     // static bool        started           = false;
//     ImGui::Text("%s", name.data());
//     ImGui::BeginGroup();
//     // static size_t truc          = 150;
//     // static bool   truc_is_valid = false;
//     // size_t        ii            = 150;
//     std::optional<size_t> new_index = std::nullopt;
//     std::optional<size_t> to_remove = std::nullopt;
//     // static size_t original      = 30;
//     // ImGui::SameLine();
//     // ImGui::Dummy({button_size() / 3.f, button_size()}); // TODO inne rboder remoed
//     // if (ImGui::BeginDragDropTarget()) // TODO(ASG) Refactor with a gunciton
//     // {
//     //     if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("##2"))
//     //     {
//     //         IM_ASSERT(payload->DataSize == sizeof(size_t));
//     //         const auto payload_n = *reinterpret_cast<const size_t*>(payload->Data); // TODO proper names
//     //         palette.move_color(payload_n, 0);                                       // TODO swap to different pos if we were after / before button
//     //     }
//     //     ImGui::EndDragDropTarget();
//     // }
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

//         // TODO the drag drop targets should be between the colors
//         if (!ImGui::GetDragDropPayload() ||
//             !ImGui::GetDragDropPayload()->Data)
//         {
//             if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) // TODO(ASG) is this flag necessary ?
//             {
//                 // ImGuiContext& g = *ImGui::GetCurrentContext();
//                 // if (!started)
//                 // {
//                 //     started = true;
//                 const auto pd = PayloadData{
//                     .color       = palette.value[i],
//                     .current_idx = i,
//                 };
//                 ImGui::SetDragDropPayload("##2", &pd, sizeof(pd));
//                 to_remove = i;
//                 debug += "gffgfd";
//                 // original = i;
//                 // }
//                 ImGui::EndDragDropSource();
//             }
//         }
//         if (ImGui::BeginDragDropTarget())
//         {
//             if (const ImGuiPayload* drag_index = ImGui::AcceptDragDropPayload("##2", ImGuiDragDropFlags_AcceptBeforeDelivery))
//             {
//                 new_index = i;
//                 // IM_ASSERT(drag_index->DataSize == sizeof(size_t));
//                 // ii                = i;
//                 // truc              = *reinterpret_cast<const size_t*>(drag_index->Data);
//                 // truc_is_valid     = true;
//                 value_has_changed = true;
//             }
//             ImGui::EndDragDropTarget();
//         }
//         // ImGui::SameLine();
//         // ImGui::Dummy({button_size() / 3.f, button_size()});

//         ImGui::PopID();
//     }
//     // if (to_remove)
//     // {
//     //     palette.value.erase(palette.value.begin() + *to_remove);
//     // }
//     if (new_index.has_value() && to_remove.has_value())
//     {
//         auto& payload = *reinterpret_cast<PayloadData*>(ImGui::GetDragDropPayload()->Data);
//         // palette.add_color(payload.color);
//         palette.move_color(*to_remove, *new_index);
//         payload.current_idx = *new_index;
//     }

//     // if (truc != size_t{150} && truc_is_valid && ii != size_t{150})
//     // {
//     //     // if (ii != truc)
//     //     {
//     //         debug += "Swapping " + std::to_string(truc) + " to " + std::to_string(ii) + "\n";
//     //         palette.move_color(truc, ii);
//     //         auto& payload = *reinterpret_cast<size_t*>(ImGui::GetDragDropPayload()->Data);
//     //         payload       = ii;
//     //         // ImGui::SetDragDropPayload("##2", &ii, sizeof(size_t));
//     //         // debug += std::to_string(payload);
//     //         // if ()
//     //     }
//     // }
//     // else
//     // {
//     //     truc_is_valid = false;
//     //     if (original != 30 && truc_is_valid)
//     //     {
//     //         palette.move_color(truc, original);
//     //         ImGui::SetDragDropPayload("##2", &original, sizeof(size_t));
//     //     }
//     // }
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