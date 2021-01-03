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

	//void beginImageSequenceExport();
	//void update();
	//void endImageSequenceExport();

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
	/// <summary>
	/// Open or closes the window with the image sequence export parameters.
	/// </summary>
	/// <param name="bOpen"></param>
	inline void setIsExportImageSequenceWindowOpen(bool bOpen) { m_bOpenImageSequenceExport = bOpen; }

private:
	std::string imageOutputPath();

private:
	std::string m_folderPath;
	std::string m_fileName = "test";

	bool m_bOpenImageExport = false;
	bool m_bShowFileExistsWarning = false;
	bool m_bOpenImageSequenceExport = false;
};

} // namespace Cool