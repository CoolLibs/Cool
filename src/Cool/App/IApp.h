#pragma once
namespace Cool {

class IApp {
public:
	IApp() = default;
	~IApp() = default;

	virtual void update() = 0;
	virtual void ImGuiWindows() = 0;
	virtual void ImGuiMenus() = 0;
	virtual void onEvent(const SDL_Event& e) = 0;
	virtual void onRenderAreaResized() = 0;
};

} // namespace Cool