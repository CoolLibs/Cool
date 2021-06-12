#include "../ImGuiExtras.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui/imgui_internal.h>

#include <Cool/Icons/Icons.h>

namespace ImGui {

void help_marker(const char* text) {
	TextDisabled("(?)");
	if (IsItemHovered())
	{
		BeginTooltip();
		PushTextWrapPos(GetFontSize() * 35.f);
		TextUnformatted(text);
		PopTextWrapPos();
		EndTooltip();
	}
}

bool angle_wheel(const char* label, float* value_p, float thickness, float radius, int nb_segments_for_circle) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;
	//
	ImGuiStyle& style = GetStyle();
	float line_height = GetTextLineHeight();
	//
	ImVec2 p = GetCursorScreenPos();
	ImVec2 center = ImVec2(p.x + radius, p.y + radius);
	// Detect clic
	InvisibleButton(label, ImVec2(radius * 2.0f, radius * 2.0f));
	bool is_active = IsItemActive();
	bool is_hovered = IsItemHovered();

	if (is_active) {
		ImVec2 mp = GetIO().MousePos;
		*value_p = atan2f(center.y - mp.y, mp.x - center.x);
	}

	float x2 = cosf(*value_p) * radius + center.x;
	float y2 = -sinf(*value_p) * radius + center.y;

	ImU32 col32 = GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	ImU32 col32line = GetColorU32(ImGuiCol_SliderGrabActive);
	ImU32 col32text = GetColorU32(ImGuiCol_Text);
	ImDrawList* draw_list = GetWindowDrawList();
	draw_list->AddCircleFilled(center, radius, col32, nb_segments_for_circle);
	draw_list->AddLine(center, ImVec2(x2, y2), col32line, thickness);
	draw_list->AddText(ImVec2(p.x + radius * 2.0f + style.ItemInnerSpacing.y, p.y + radius - line_height * 0.5f), col32text, label);

	const ImGuiID id = window->GetID(label);
	if (is_active)
		MarkItemEdited(id);
	return is_active;
}

bool direction_3d(const char* label, float* value_p1, float* value_p2) {
	BeginGroup(); // Group the two wheels so that things like IsItemDeactivatedAfterEdit() work properly
	PushID(label);
	bool b = false;

	Text("%s :", label);
	b |= angle_wheel("Angle Ground", value_p1);
	b |= angle_wheel("Angle Up", value_p2);

	PopID();
	EndGroup();
	return b;
}

void time_formated_hms(float time_in_sec, float total_duration) {
	if (total_duration == 0.f)
		total_duration = time_in_sec;
	uint32_t t = static_cast<uint32_t>(time_in_sec);
	if (total_duration < 60.f) {
		Text("%us", t);
	}
	else if (total_duration < 3600.f) {
		Text("%um %02us", t / 60, t % 60);
	}
	else {
		Text("%uh %02um %02us", t / 3600, (t % 3600) / 60, t % 60);
	}
}

void tooltip(const char* text) {
	if (IsItemHovered()) {
		BeginTooltip();
		Text(text);
		EndTooltip();
	}
}

void button_disabled(const char* label, const char* reason_for_disabling) {
	PushStyleColor(ImGuiCol_Button, GetStyle().Colors[ImGuiCol_FrameBg]);
	PushStyleColor(ImGuiCol_Text,   GetStyle().Colors[ImGuiCol_TextDisabled]);
	ButtonEx(label, ImVec2(0, 0), ImGuiButtonFlags_Disabled);
	PopStyleColor(2);
	tooltip(reason_for_disabling);
}

bool button_with_icon(GLuint tex_id, const ImVec4& tint_color, const ImVec4& background_color, float button_width, float button_height, int frame_padding) {
	return ImageButton(reinterpret_cast<ImTextureID>(tex_id), ImVec2(button_width, button_height), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), frame_padding, background_color, tint_color);
}

void button_with_icon_disabled(GLuint tex_id, const char* reason_for_disabling, float button_width, float button_height, int frame_padding) {
	const ImVec4 grey = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
	image_framed(tex_id, ImVec2(button_width, button_height), frame_padding, grey, ImVec4(0.f, 0.f, 0.f, 1.f), grey);
	tooltip(reason_for_disabling);
}

void image_framed(GLuint tex_id, const ImVec2& size, int frame_thickness, const ImVec4& frame_color, const ImVec4& background_color, const ImVec4& tint_color) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	// Default to using texture ID as ID. User can still push string/integer prefixes.
	// We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
	PushID((void*)(intptr_t)tex_id);
	const ImGuiID id = window->GetID("#image");
	PopID();

	const ImVec2 padding = (frame_thickness >= 0) ? ImVec2((float)frame_thickness, (float)frame_thickness) : style.FramePadding;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
	const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
	ItemSize(bb);
	if (!ItemAdd(bb, id))
		return;

	// Render
	const ImU32 frameCol = frame_color.w > 0.0f ? GetColorU32(frame_color) : GetColorU32(ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, frameCol, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	RenderFrame(image_bb.Min, image_bb.Max, GetColorU32(background_color), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	window->DrawList->AddImage(reinterpret_cast<ImTextureID>(tex_id), image_bb.Min, image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(tint_color));
}

bool input_uint(const char* label, unsigned int* value_p) {
	return InputScalar(label, ImGuiDataType_U32, value_p, NULL, NULL, "%u");
}

void warning_text(const char* text) {
	TextColored(ImVec4(0.95f, 0.1f, 0.2f, 1.0f), text);
}

bool begin_popup_context_menu_from_button(const char* label, ImGuiPopupFlags popup_flags) {
	ImGuiWindow* window = GImGui->CurrentWindow;
	if (window->SkipItems)
		return false;
	ImGuiID id = label ? window->GetID(label) : window->DC.LastItemId; // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
	IM_ASSERT(id != 0);                                                // You cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
	if (Button(label))
		OpenPopupEx(id, popup_flags);
	return BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
}

void invisible_wrapper_around_previous_line(const char* label) {
	SetCursorPos(GetCursorPos() - ImVec2(0, 2 * GetTextLineHeight()));
	InvisibleButton(label, ImVec2(GetWindowWidth(), 2 * GetTextLineHeight()));
}

bool open_folder_dialog(std::string* out_path, std::string_view base_folder) {
	if (button_with_icon(Cool::Icons::folder(), ImVec4(1, 1, 1, 1), ImVec4(0.1, 0.1, 0.1, 1))) {
		NFD::UniquePath outPath;
		nfdresult_t result;
		result = NFD::PickFolder(outPath, base_folder.data());
		if (result == NFD_OKAY) {
			*out_path = outPath.get();
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool open_file_dialog(std::string* out_path, std::vector<nfdfilteritem_t> file_type_filters, std::string_view base_folder) {
	if (button_with_icon(Cool::Icons::folder(), ImVec4(1, 1, 1, 1), ImVec4(0.1, 0.1, 0.1, 1))) {
		NFD::UniquePath outPath;
		nfdresult_t result;
		result = NFD::OpenDialog(outPath, file_type_filters.data(), file_type_filters.size(), base_folder.data());
		if (result == NFD_OKAY) {
			*out_path = outPath.get();
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

} // namespace ImGui