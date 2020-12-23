#pragma once

namespace Cool {

class IApp {
public:
	IApp() = default;
	~IApp() = default;

	/// <summary>
	/// Update function that will be called repeatedly
	/// </summary>
	virtual void update() {};
	/// <summary>
	/// Please create all your ImGui windows inside this function (allows the AppManager to hide all UI when the user presses CTRL+H)
	/// </summary>
	virtual void ImGuiWindows() {};
	/// <summary>
	/// Put all menus that should appear on the top menu bar here.
	/// </summary>
	virtual void ImGuiMenus() {};
	/// <summary>
	/// Handle SDL events (mouse, keyboard etc.). Events might not be transmitted to this function if they are caught by the AppManager (ImGui interaction, F11 for toggling fullscreen etc.)
	/// </summary>
	/// <param name="e">An SDL event. See here for the full range of available events : https://wiki.libsdl.org/SDL_Event</param>
	virtual void onEvent(const SDL_Event& e) {};
};

} // namespace Cool