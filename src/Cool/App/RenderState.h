#pragma once

#include "Cool/App/internal/RectSizePos.h"

namespace Cool {

class RenderState {
public:
	/// <summary>
	/// The actual size that you should use for rendering (takes into account whether you are rendering on screen or exporting an image etc.)
	/// </summary>
	/// <returns></returns>
	static RectSize Size();
	/// <summary>
	/// Subscribe to the event triggered whenever the size used for rendering changes. Typically required to update things that depend on the size or aspect ratio of the render area.
	/// </summary>
	/// <param name="onSizeChangeCB">A callback function called when the size changes</param>
	static inline void SubscribeToSizeChanges(std::function<void()> onSizeChangeCB) { m_onRenderAreaResizedCallbacks.push_back(onSizeChangeCB); }
	/// <summary>
	/// Returns whether we are currently exporting an image or image sequence
	/// </summary>
	static inline bool IsExporting() { return m_bIsExporting; }

private:
	friend class AppManager;
	friend class Renderer;
	friend class Input;
	// Size and position that should be used for showing rendered images inside the window
	static RectSizePos InAppRenderArea();
	inline static glm::ivec2 SwapYConvention(const glm::ivec2& p) { return { p.x, m_Window.height() - p.y }; }

	static void setIsExporting(bool bIsExporting);
	static void setExportSize        (int width, int height);
	static void setWindowSize        (int width, int height);
	static void setAvailableSpaceSize(int width, int height, bool bCanTriggerResizeEvent = true);
	static inline void setWindowTopLeft        (int x, int y) { m_Window .setTopLeft(x, y); }
	static inline void setAvailableSpaceTopLeft(int x, int y) { m_AvailableSpace.setTopLeft(x, y); }

	static inline const glm::ivec2& getExportSize()         { return m_Export.size(); }
	static inline const glm::ivec2& getWindowSize()         { return m_Window.size(); }
	static inline const glm::ivec2& getAvailableSpaceSize() { return m_AvailableSpace.size(); }
	static inline       glm::ivec2  getWindowTopLeft()      { return m_Window.topLeft(); }

	static void ImGuiConstrainInAppRenderAreaRatio();

	static void OnRenderAreaResized();

private:
	/// Function pointers to be used as a callback when the render area is resized
	static std::vector<std::function<void()>> m_onRenderAreaResizedCallbacks;

	static RectSizePos m_Window;         // Size of the window, and it's position inside the screen
	static RectSizePos m_AvailableSpace; // Size of the available space inside the window (excludes the docked ImGui windows), and its position inside the window
	static RectSize m_Export;            // Size of the exported images

	static bool m_bIsExporting; // Owned by RenderState because it needs to know it when deciding what the rendered size should be

	static bool m_bConstrainInAppRenderAreaRatio;
	static float m_inAppRenderAreaConstrainedRatio;
};

} // namespace Cool