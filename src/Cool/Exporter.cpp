#include "Exporter.h"

#include <Cool/App/RenderState.h>
#include <Cool/OpenGL/FrameBuffer.h>
#include <Cool/File/File.h>
#include <Cool/ExportImage/AsPNG.h>

namespace Cool {

Exporter::Exporter()
	: m_folderPath(File::RootDir + "/out")
{}

void Exporter::beginImageExport() {
	RenderState::setIsExporting(true);
}

void Exporter::endImageExport(FrameBuffer& frameBuffer) {
	// Get data
	frameBuffer.bind();
	RectSize size = RenderState::Size();
	unsigned char* data = new unsigned char[4 * size.width() * size.height()];
	glReadPixels(0, 0, size.width(), size.height(), GL_RGBA, GL_UNSIGNED_BYTE, data);
	frameBuffer.unbind();
	// Write png
	File::CreateFoldersIfDoesntExist(m_folderPath.c_str());
	ExportImage::AsPNG((m_folderPath + "/" + m_fileName).c_str(), size.width(), size.height(), data);
	delete[] data;
	//
	RenderState::setIsExporting(false);
}

} // namespace Cool