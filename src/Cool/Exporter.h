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

private:
	std::string m_folderPath;
	std::string m_fileName = "test.png";
};

} // namespace Cool