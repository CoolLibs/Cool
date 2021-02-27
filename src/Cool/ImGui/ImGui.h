#pragma once

namespace CoolImGui {

	/// <summary>
	/// A small "?" that displays some text when hovered.
	/// </summary>
	void HelpMarker(const char* text);

	/// <summary>
	/// A widget to pick an angle (in radians), displayed as a direction on a wheel.
	/// </summary>
	/// <param name="label">Name displayed alongside the widget</param>
	/// <param name="value_p">A pointer to the variable that the widget should modify</param>
	/// <param name="thickness">Thickness of the line indicating the direction</param>
	/// <param name="radius">Radius of the wheel (a.k.a. size of the widget)</param>
	/// <param name="circleNbSegments">Number of segments used to render the wheel (which is actually a polygon with a huge number of sides)</param>
	/// <returns>true iff the widget was just used</returns>
	bool AngleWheel(const char* label, float* value_p, float thickness = 2.0f, float radius = 25.0f, int circleNbSegments = 26);
	
	/// <summary>
	/// Writes some ImGui text : time is displayed as [hours]:[minutes]:[seconds]. hours and minutes might not be displayed if they are 0 for both timeInSec and totalDuration.
	/// </summary>
	/// <param name="timeInSec">The time to be displayed, expressed in seconds</param>
	/// <param name="totalDuration">Optionnal : the total length of the time interval that timeInSec is part of, expressed in seconds
	/// (for example timeInSec might be a timestamp in a video, and totalDuration would be the duration of the video)
	/// It allows the formating to know whether it should write hours and minutes or not, in order for the display to be consistent accross the wole duration of totalDuration.
	/// Leave as 0.0f if you don't want or need this behaviour.
	/// </param>
	void TimeFormatedHMS(float timeInSec, float totalDuration = 0.0f);
	
	/// <summary>
	/// Displays some text on hover for the previously declared widget.
	/// </summary>
	void Tooltip(const char* text);
	
	/// <summary>
	/// A greyed out button that you can't click on.
	/// </summary>
	/// <param name="label">The text inside the button</param>
	/// <param name="reasonForDisabling">An explanation that will be shown on hover</param>
	void ButtonDisabled(const char* label, const char* reasonForDisabling = "Currently disabled");
	
	/// <summary>
	/// A button that uses an image instead of text
	/// </summary>
	/// <param name="texID">OpenGL texture ID of the image</param>
	/// <param name="tintColor">You can tint your image</param>
	/// <param name="backgroundColor">Color that will appear in places where your image is transparent</param>
	/// <returns>true iff the button was just pressed</returns>
	bool ButtonWithIcon(GLuint texID, const ImVec4& tintColor = ImVec4(1, 1, 1, 1), const ImVec4& backgroundColor = ImVec4(0, 0, 0, 1));
	
	/// <summary>
	/// A button that uses an image instead of text, but greyed out
	/// </summary>
	/// <param name="texID">OpenGL texture ID of the image</param>
	/// <param name="reasonForDisabling">An explanation that will be shown on hover</param>
	void ButtonWithIconDisabled(GLuint texID, const char* reasonForDisabling = "Currently disabled");
	
	/// <summary>
	/// Displays an image with a frame around it
	/// </summary>
	/// <param name="texID">OpenGL texture ID of the image</param>
	/// <param name="size">size the image will be displayed at</param>
	/// <param name="frameThickness">Thickness of the frame. Leave as -1 if you don't want a frame</param>
	/// <param name="frameColor">Color of the frame</param>
	/// <param name="backgroundColor">Color that will appear in places where your image is transparent</param>
	/// <param name="tintColor">You can tint your image</param>
	void ImageFramed(GLuint texID, const ImVec2& size, int frameThickness = -1, const ImVec4& frameColor = ImVec4(0, 0, 0, 0), const ImVec4& backgroundColor = ImVec4(0, 0, 0, 1), const ImVec4& tintColor = ImVec4(1, 1, 1, 1));
	
	/// <summary>
	/// Like ImGui::InputInt, but for an unsigned int
	/// </summary>
	/// <param name="label"></param>
	/// <param name="value_p"></param>
	/// <returns></returns>
	bool InputUInt(const char* label, unsigned int* value_p);
	
	/// <summary>
	/// Writes some red text
	/// </summary>
	void WarningText(const char* text);

	/// <summary>
	/// Opens a context menu when you click on the button
	/// </summary>
	/// <param name="label">The text displayed inside the button</param>
	/// <param name="popup_flags">The ImGui flags of the context menu</param>
	/// <returns></returns>
	bool BeginPopupContextMenuFromButton(const char* label, ImGuiPopupFlags popup_flags = ImGuiPopupFlags_None);

} // namespace Cool::ImGui