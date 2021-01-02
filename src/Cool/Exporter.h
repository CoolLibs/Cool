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

	void ImGuiMenuItems();
	bool ImGuiExportImageWindow();

	inline void setIsExportImageWindowOpen(bool bOpen) { m_bOpenImageExport = bOpen; }
	inline void setIsExportImageSequenceWindowOpen(bool bOpen) { m_bOpenImageSequenceExport = bOpen; }

private:
	std::string m_folderPath;
	std::string m_fileName = "test";

	bool m_bOpenImageExport = false;
	bool m_bOpenImageSequenceExport = false;
};

} // namespace Cool