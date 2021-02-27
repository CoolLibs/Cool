#include "ImGui.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui/imgui_internal.h>

namespace CoolImGui {

void HelpMarker(const char* text) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(text);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

bool AngleWheel(const char* label, float* value_p, float thickness, float radius, int circleNbSegments) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;
	//
	ImGuiStyle& style = ImGui::GetStyle();
	float line_height = ImGui::GetTextLineHeight();
	//
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImVec2 center = ImVec2(p.x + radius, p.y + radius);
	// Detect clic
	ImGui::InvisibleButton(label, ImVec2(radius * 2.0f, radius * 2.0f));
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemHovered();

	if (is_active) {
		ImVec2 mp = ImGui::GetIO().MousePos;
		*value_p = atan2f(center.y - mp.y, mp.x - center.x);
	}

	float x2 = cosf(*value_p) * radius + center.x;
	float y2 = -sinf(*value_p) * radius + center.y;

	ImU32 col32 = ImGui::GetColorU32(is_active ? ImGuiCol_FrameBgActive : is_hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
	ImU32 col32line = ImGui::GetColorU32(ImGuiCol_SliderGrabActive);
	ImU32 col32text = ImGui::GetColorU32(ImGuiCol_Text);
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->AddCircleFilled(center, radius, col32, circleNbSegments);
	draw_list->AddLine(center, ImVec2(x2, y2), col32line, thickness);
	draw_list->AddText(ImVec2(p.x + radius * 2.0f + style.ItemInnerSpacing.y, p.y + radius - line_height * 0.5f), col32text, label);

	return is_active;
}

void TimeFormatedHMS(float timeInSec, float totalDuration) {
	if (totalDuration == 0.0f)
		totalDuration = timeInSec;
	uint32_t t = static_cast<uint32_t>(timeInSec);
	if (totalDuration < 60.0f) {
		ImGui::Text("%us", t);
	}
	else if (totalDuration < 3600.0f) {
		ImGui::Text("%um %02us", t / 60, t % 60);
	}
	else {
		ImGui::Text("%uh %02um %02us", t / 3600, (t % 3600) / 60, t % 60);
	}
}

void Tooltip(const char* text) {
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text(text);
		ImGui::EndTooltip();
	}
}

void ButtonDisabled(const char* label, const char* reasonForDisabling) {
	ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_FrameBg]);
	ImGui::PushStyleColor(ImGuiCol_Text,   ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
	ImGui::ButtonEx(label, ImVec2(0, 0), ImGuiButtonFlags_Disabled);
	ImGui::PopStyleColor(2);
	Tooltip(reasonForDisabling);
}

static constexpr int BUTTON_ICON_SIZE = 18;
static constexpr int BUTTON_FRAME_PADDING = 1;

bool ButtonWithIcon(GLuint texID, const ImVec4& tintColor, const ImVec4& backgroundColor) {
	return ImGui::ImageButton(reinterpret_cast<ImTextureID>(texID), ImVec2(BUTTON_ICON_SIZE, BUTTON_ICON_SIZE), ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), BUTTON_FRAME_PADDING, backgroundColor, tintColor);
}

void ButtonWithIconDisabled(GLuint texID, const char* reasonForDisabling) {
	const ImVec4 grey = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
	ImageFramed(texID, ImVec2(BUTTON_ICON_SIZE, BUTTON_ICON_SIZE), BUTTON_FRAME_PADDING, grey, ImVec4(0.f, 0.f, 0.f, 1.f), grey);
	Tooltip(reasonForDisabling);
}

void ImageFramed(GLuint texID, const ImVec2& size, int frameThickness, const ImVec4& frameColor, const ImVec4& backgroundColor, const ImVec4& tintColor) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	// Default to using texture ID as ID. User can still push string/integer prefixes.
	// We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
	ImGui::PushID((void*)(intptr_t)texID);
	const ImGuiID id = window->GetID("#image");
	ImGui::PopID();

	const ImVec2 padding = (frameThickness >= 0) ? ImVec2((float)frameThickness, (float)frameThickness) : style.FramePadding;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
	const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
	ImGui::ItemSize(bb);
	if (!ImGui::ItemAdd(bb, id))
		return;

	// Render
	const ImU32 frameCol = frameColor.w > 0.0f ? ImGui::GetColorU32(frameColor) : ImGui::GetColorU32(ImGuiCol_Button);
	ImGui::RenderNavHighlight(bb, id);
	ImGui::RenderFrame(bb.Min, bb.Max, frameCol, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	ImGui::RenderFrame(image_bb.Min, image_bb.Max, ImGui::GetColorU32(backgroundColor), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	window->DrawList->AddImage(reinterpret_cast<ImTextureID>(texID), image_bb.Min, image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(tintColor));
}

bool InputUInt(const char* label, unsigned int* value_p) {
	return ImGui::InputScalar(label, ImGuiDataType_U32, value_p, NULL, NULL, "%u");
}

void WarningText(const char* text) {
	ImGui::TextColored(ImVec4(0.95f, 0.1f, 0.2f, 1.0f), text);
}

bool BeginPopupContextMenuFromButton(const char* label, ImGuiPopupFlags popup_flags) {
	using namespace ImGui;
	ImGuiWindow* window = GImGui->CurrentWindow;
	if (window->SkipItems)
		return false;
	ImGuiID id = label ? window->GetID(label) : window->DC.LastItemId; // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
	IM_ASSERT(id != 0);                                                // You cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
	if (ImGui::Button(label))
		OpenPopupEx(id, popup_flags);
	return BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
}

void InvisibleWrapperAroundPreviousLine(const char* label) {
	ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, 2 * ImGui::GetTextLineHeight()));
	ImGui::InvisibleButton(label, ImVec2(ImGui::GetWindowWidth(), 2 * ImGui::GetTextLineHeight()));
}

} // namespace CoolImGui