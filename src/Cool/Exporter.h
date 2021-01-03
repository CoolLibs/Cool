#pragma once

namespace Cool {

class FrameBuffer;

class Exporter {
public:
	Exporter();
	~Exporter() = default;

	/// <summary>
	/// Call this before your rendering code.
	/// </summary>
	void beginImageExport();
	/// <summary>
	/// Call this after your rendering code.
	/// </summary>
	/// <param name="frameBuffer">The frame buffer that your renderer has just rendered to.</param>
	void endImageExport(FrameBuffer& frameBuffer);

	/// <summary>
	/// The buttons to open the different export windows.
	/// </summary>
	void ImGuiMenuItems();
	/// <summary>
	/// The window with the image export parameters
	/// </summary>
	/// <returns>true iff you should then export an image. (By calling beginImageExport(), then your rendering code, then endImageExport()</returns>
	bool ImGuiExportImageWindow();

	/// <summary>
	/// Open or closes the window with the image export parameters.
	/// </summary>
	/// <param name="bOpen"></param>
	void setIsExportImageWindowOpen(bool bOpen);

#if defined(__COOL_TIME) && defined(__COOL_STRING)
	/// <summary>
	/// Starts the export of the image sequence. You must then call update() on every frame after your rendering code.
	/// </summary>
	void beginImageSequenceExport();
	/// <summary>
	/// Call this after your rendering code. It will export the current frame and decide if the export should continue.
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
	void exportImage(FrameBuffer& frameBuffer, const char* filepath);

private:
	std::string m_folderPathForImage;
	std::string m_fileName = "img";
	bool m_bOpenImageExport = false;
	bool m_bShowFileExistsWarning = false;

#if defined(__COOL_TIME) && defined(__COOL_STRING)
	std::string m_folderPathForImageSequence;
	bool m_bIsExportingImageSequence = false;
	float m_fps = 30.f;
	// In seconds
	float m_sequenceBeginTimeInS = 0.f;
	// In seconds
	float m_sequenceEndTimeInS = 10.f;
	bool m_bOpenImageSequenceExport = false;
	int m_frameCount;
	int m_maxNbDigitsOfFrameCount;
#endif
};

} // namespace Cool