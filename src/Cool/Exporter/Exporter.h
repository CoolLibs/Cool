#pragma once

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
#include "internal/Averager.h"
#include <Cool/MultiThread/ThreadPool.h>
#endif

namespace Cool {

class FrameBuffer;

class Exporter {
public:
	Exporter();
	~Exporter() = default;

	/// <summary>
	/// Exports an image as PNG
	/// </summary>
	/// <param name="render">The function that renders the desired image</param>
	/// <param name="frameBuffer">The frame buffer that your render function has just rendered to</param>
	/// <param name="filepath">The name of the image file that you want to write</param>
	void export_image(std::function<void()> render, FrameBuffer& frameBuffer, const char* filepath);

	/// <summary>
	/// The buttons to open the different export windows.
	/// </summary>
	void ImGuiMenuItems();

	/// <summary>
	/// The window with the image export parameters
	/// </summary>
	/// <param name="render">The function that renders the desired image</param>
	/// <param name="frameBuffer">The frame buffer that your render function has just rendered to</param>
	void ImGuiExportImageWindow(std::function<void()> render, FrameBuffer& frameBuffer);

	/// <summary>
	/// Open or closes the window with the image export parameters.
	/// </summary>
	/// <param name="bOpen"></param>
	void setIsExportImageWindowOpen(bool bOpen);

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
	/// <summary>
	/// Starts the export of the image sequence. You must then call update() on every frame after your rendering code.
	/// </summary>
	void beginImageSequenceExport();
	/// <summary>
	/// Call this after your rendering code. If we are exporting it will export the current frame and decide if the export should continue.
	/// </summary>
	/// <param name="frameBuffer">The frame buffer that your renderer has just rendered to.</param>
	void update(FrameBuffer& frameBuffer);
	/// <summary>
	/// Ends the export of the image sequence. It will be called automatically by update() once the end timestamp is reached.
	/// </summary>
	void endImageSequenceExport();
	/// <summary>
	/// The window with the image sequence export parameters
	/// </summary>
	void ImGuiExportImageSequenceWindow();
	/// <summary>
	/// Open or closes the window with the image sequence export parameters.
	/// </summary>
	/// <param name="bOpen"></param>
	inline void setIsExportImageSequenceWindowOpen(bool bOpen) { m_bOpenImageSequenceExport = bOpen; }
#endif

private:
	std::string imageOutputPath();
	void findAvailableFileName();
	void ImGuiResolutionWidget();
#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
	void export_image_multithreaded(FrameBuffer& frameBuffer, const char* filepath);
#endif

private:
	std::string m_folderPathForImage;
	std::string m_fileName = "img";
	bool m_bOpenImageExport = false;
	bool m_bShowFileExistsWarning = false;

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
	ThreadPool _thread_pool{ 8 };
	std::string m_folderPathForImageSequence;
	bool m_bIsExportingImageSequence = false;
	float m_fps = 30.f;
	// In seconds
	float m_sequenceBeginTimeInS = 0.f;
	// In seconds
	float m_sequenceEndTimeInS = 1.f;
	bool m_bOpenImageSequenceExport = false;
	int m_frameCount;
	int m_totalNbOfFramesInSequence;
	int m_maxNbDigitsOfFrameCount;
	Averager<float> m_averageFrameTime;
	std::chrono::steady_clock::time_point m_lastExportTime;
#endif
};

} // namespace Cool