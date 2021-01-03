#pragma once

namespace Cool {

class FrameBuffer;

class Exporter {
public:
	Exporter();
	~Exporter() = default;

	void beginImageExport();
	void endImageExport(FrameBuffer& frameBuffer);

	//void beginImageSequenceExport();
	//void update();
	//void endImageSequenceExport();

	std::string imageOutputPath();

	void ImGuiMenuItems();
	bool ImGuiExportImageWindow();

	void setIsExportImageWindowOpen(bool bOpen);
	inline void setIsExportImageSequenceWindowOpen(bool bOpen) { m_bOpenImageSequenceExport = bOpen; }

private:
	std::string m_folderPath;
	std::string m_fileName = "test";

	bool m_bOpenImageExport = false;
	bool m_bShowFileExistsWarning = false;
	bool m_bOpenImageSequenceExport = false;
};

} // namespace Cool