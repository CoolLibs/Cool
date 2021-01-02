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

private:
	std::string m_folderPath;
	std::string m_fileName = "test";

	bool m_bOpenImageExport = false;
	bool m_bOpenImageSequenceExport = false;
};

} // namespace Cool