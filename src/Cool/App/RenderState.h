#pragma once

#include "internal/RectSizePos.h"
#include <Cool/AspectRatio.h>

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
	/// Returns whether we are currently exporting an image or not ; if true it means that the size returned by Size() is the one set with setExportSize().
	/// Otherwise it means that Size() returns the size of the current viewing area inside the window.
	/// </summary>
	static inline bool IsExporting() { return m_bIsExporting; }
	/// <summary>
	/// If bIsExporting is true, the size returned by Size() is the one set with setExportSize().
	/// Otherwise, Size() returns the size of the current viewing area inside the window.
	/// </summary>
	/// <param name="bIsExporting"></param>
	static void setIsExporting(bool bIsExporting);
	/// <summary>
	/// Set a size that will be used while exporting.
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	static void setExportSize(int width, int height);
	/// <summary>
	/// Sets the aspect ratio used by the preview. You also need to setPreviewAspectRatioControl(true).
	/// Otherwise we just render at the available space's aspectRatio.
	/// </summary>
	static void setPreviewAspectRatio(float aspectRatio);
	/// <summary>
	/// If bControl is true, we use the aspect ratio set by setPreviewAspectRatio to render our preview image.
	/// If bControl is false, we instead render our preview to match the aspect ratio of the previewing area.
	/// </summary>
	static void setPreviewAspectRatioControl(bool bControl);
	/// <summary>
	/// Sets the number of pixels used in the preview. You also need to setPreviewNbPixelsControl(true) to use this number of pixels.
	/// Otherwise we just render at the available space size.
	/// <param name="nbPixels">Desired number of pixels in the preview render. It will not be matched exactly but will be approximated while also respecting the preview aspect ratio.</param>
	/// </summary>
	static void setPreviewNbPixels(int nbPixels);
	/// <summary>
	/// If bControl is true, we use the number of pixels set by setPreviewNbPixels to render our preview image.
	/// If bControl is false, we instead render our preview to match the size of the previewing area.
	/// </summary>
	static void setPreviewNbPixelsControl(bool bControl);

private:
	friend class AppManager;
	friend class Renderer_Fullscreen;
	friend class Input;
	friend class Exporter;
	// Size and position that should be used for showing rendered images inside the window
	static RectSizePos InAppRenderArea();
	// Size that should be used for rendering preview images (then shown in the InAppRenderArea)
	static RectSize PreviewSize();
	inline static glm::ivec2 SwapYConvention(const glm::ivec2& p) { return { p.x, m_Window.height() - p.y }; }

	static void setWindowSize        (int width, int height);
	static void setAvailableSpaceSize(int width, int height, bool bCanTriggerResizeEvent = true);
	static inline void setWindowTopLeft        (int x, int y) { m_Window .setTopLeft(x, y); }
	static inline void setAvailableSpaceTopLeft(int x, int y) { m_AvailableSpace.setTopLeft(x, y); }
	static void afterSettingPreviewAspectRatio();

	static inline const glm::ivec2& getExportSize()         { return m_Export.size(); }
	static inline const glm::ivec2& getWindowSize()         { return m_Window.size(); }
	static inline const glm::ivec2& getAvailableSpaceSize() { return m_AvailableSpace.size(); }
	static inline       glm::ivec2  getWindowTopLeft()      { return m_Window.topLeft(); }

	static void ImGuiPreviewControls();

	static void OnRenderAreaResized();

private:
	/// Function pointers to be used as a callback when the render area is resized
	static std::vector<std::function<void()>> m_onRenderAreaResizedCallbacks;

	// Size of the window, and it's position inside the screen
	static RectSizePos m_Window;
	// Size of the available space inside the window (excludes the docked ImGui windows), and its position inside the window
	static RectSizePos m_AvailableSpace;
	// Size of the exported images
	static RectSize m_Export;
	// Size of the images while we preview and control the number of pixels
	static RectSize m_PreviewWithControlledNbPixels;

	static bool m_bIsExporting; // Owned by RenderState because it needs to know it when deciding what the rendered size should be

	static bool m_bControlPreviewRatio;
	static AspectRatio m_previewRatio;
	static bool m_bControlPreviewNbPixels;
	static int m_previewNbPixels;
};

} // namespace Cool