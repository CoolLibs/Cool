#include "ImGui.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui/imgui_internal.h>

namespace ImGui {

void HelpMarker(const char* text) {
	TextDisabled("(?)");
    if (IsItemHovered())
    {
        BeginTooltip();
        PushTextWrapPos(GetFontSize() * 35.0f);
        TextUnformatted(text);
        PopTextWrapPos();
        EndTooltip();
    }
}

bool AngleWheel(const char* label, float* value_p, float thickness, float radius, int circleNbSegments) {
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
	draw_list->AddCircleFilled(center, radius, col32, circleNbSegments);
	draw_list->AddLine(center, ImVec2(x2, y2), col32line, thickness);
	draw_list->AddText(ImVec2(p.x + radius * 2.0f + style.ItemInnerSpacing.y, p.y + radius - line_height * 0.5f), col32text, label);

	const ImGuiID id = window->GetID(label);
	if (is_active)
		MarkItemEdited(id);
	return is_active;
}

bool Direction3D(const char* label, float* value_p1, float* value_p2) {
	BeginGroup(); // Group the two wheels so that things like IsItemDeactivatedAfterEdit() work properly
	PushID(label);
	bool b = false;

	Text("%s :", label);
	b |= AngleWheel("Angle Ground", value_p1);
	b |= AngleWheel("Angle Up", value_p2);

	PopID();
	EndGroup();
	return b;
}

void TimeFormatedHMS(float timeInSec, float totalDuration) {
	if (totalDuration == 0.0f)
		totalDuration = timeInSec;
	uint32_t t = static_cast<uint32_t>(timeInSec);
	if (totalDuration < 60.0f) {
		Text("%us", t);
	}
	else if (totalDuration < 3600.0f) {
		Text("%um %02us", t / 60, t % 60);
	}
	else {
		Text("%uh %02um %02us", t / 3600, (t % 3600) / 60, t % 60);
	}
}

void Tooltip(const char* text) {
	if (IsItemHovered()) {
		BeginTooltip();
		Text(text);
		EndTooltip();
	}
}

void ButtonDisabled(const char* label, const char* reasonForDisabling) {
	PushStyleColor(ImGuiCol_Button, GetStyle().Colors[ImGuiCol_FrameBg]);
	PushStyleColor(ImGuiCol_Text,   GetStyle().Colors[ImGuiCol_TextDisabled]);
	ButtonEx(label, ImVec2(0, 0), ImGuiButtonFlags_Disabled);
	PopStyleColor(2);
	Tooltip(reasonForDisabling);
}

static constexpr int BUTTON_ICON_SIZE = 18;
static constexpr int BUTTON_FRAME_PADDING = 1;

bool ButtonWithIcon(GLuint texID, const ImVec4& tintColor, const ImVec4& backgroundColor) {
	return ImageButton(reinterpret_cast<ImTextureID>(texID), ImVec2(BUTTON_ICON_SIZE, BUTTON_ICON_SIZE), ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), BUTTON_FRAME_PADDING, backgroundColor, tintColor);
}

void ButtonWithIconDisabled(GLuint texID, const char* reasonForDisabling) {
	const ImVec4 grey = ImVec4(0.35f, 0.35f, 0.35f, 1.f);
	ImageFramed(texID, ImVec2(BUTTON_ICON_SIZE, BUTTON_ICON_SIZE), BUTTON_FRAME_PADDING, grey, ImVec4(0.f, 0.f, 0.f, 1.f), grey);
	Tooltip(reasonForDisabling);
}

void ImageFramed(GLuint texID, const ImVec2& size, int frameThickness, const ImVec4& frameColor, const ImVec4& backgroundColor, const ImVec4& tintColor) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	// Default to using texture ID as ID. User can still push string/integer prefixes.
	// We could hash the size/uv to create a unique ID but that would prevent the user from animating UV.
	PushID((void*)(intptr_t)texID);
	const ImGuiID id = window->GetID("#image");
	PopID();

	const ImVec2 padding = (frameThickness >= 0) ? ImVec2((float)frameThickness, (float)frameThickness) : style.FramePadding;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
	const ImRect image_bb(window->DC.CursorPos + padding, window->DC.CursorPos + padding + size);
	ItemSize(bb);
	if (!ItemAdd(bb, id))
		return;

	// Render
	const ImU32 frameCol = frameColor.w > 0.0f ? GetColorU32(frameColor) : GetColorU32(ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, frameCol, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	RenderFrame(image_bb.Min, image_bb.Max, GetColorU32(backgroundColor), true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	window->DrawList->AddImage(reinterpret_cast<ImTextureID>(texID), image_bb.Min, image_bb.Max, ImVec2(0, 0), ImVec2(1, 1), GetColorU32(tintColor));
}

bool InputUInt(const char* label, unsigned int* value_p) {
	return InputScalar(label, ImGuiDataType_U32, value_p, NULL, NULL, "%u");
}

void WarningText(const char* text) {
	TextColored(ImVec4(0.95f, 0.1f, 0.2f, 1.0f), text);
}

bool BeginPopupContextMenuFromButton(const char* label, ImGuiPopupFlags popup_flags) {
	ImGuiWindow* window = GImGui->CurrentWindow;
	if (window->SkipItems)
		return false;
	ImGuiID id = label ? window->GetID(label) : window->DC.LastItemId; // If user hasn't passed an ID, we can use the LastItemID. Using LastItemID as a Popup ID won't conflict!
	IM_ASSERT(id != 0);                                                // You cannot pass a NULL str_id if the last item has no identifier (e.g. a Text() item)
	if (Button(label))
		OpenPopupEx(id, popup_flags);
	return BeginPopupEx(id, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);
}

void InvisibleWrapperAroundPreviousLine(const char* label) {
	SetCursorPos(GetCursorPos() - ImVec2(0, 2 * GetTextLineHeight()));
	InvisibleButton(label, ImVec2(GetWindowWidth(), 2 * GetTextLineHeight()));
}

} // namespace ImGui