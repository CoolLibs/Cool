#include "Exporter.h"

#include <Cool/App/RenderState.h>
#include <Cool/OpenGL/FrameBuffer.h>
#include <Cool/File/File.h>
#include <Cool/ExportImage/AsPNG.h>
#include <Cool/ImGui/ImGui.h>

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
	ExportImage::AsPNG((m_folderPath + "/" + m_fileName + ".png").c_str(), size.width(), size.height(), data);
	delete[] data;
	//
	RenderState::setIsExporting(false);
}

void Exporter::ImGuiMenuItems() {
	if (ImGui::Button("Image")) {
		m_bOpenImageExport = true;
	}
	if (ImGui::Button("Image Sequence")) {
		m_bOpenImageSequenceExport = true;
	}
}

bool Exporter::ImGuiExportImageWindow() {
	bool bMustExport = false;
	if (m_bOpenImageExport) {
		ImGui::Begin("Export an Image", &m_bOpenImageExport);
		// Resolution
		bool bUsed = false;
		ImGui::Text("Resolution : "); ImGui::SameLine();
		ImGui::PushItemWidth(50);
		unsigned int w = static_cast<unsigned int>(RenderState::getExportSize().x);
		unsigned int h = static_cast<unsigned int>(RenderState::getExportSize().y);
		bUsed |= CoolImGui::InputUInt("W", &w); ImGui::SameLine();
		bUsed |= CoolImGui::InputUInt("H", &h);
		ImGui::PopItemWidth();
		if (bUsed)
			RenderState::setExportSize(static_cast<int>(w), static_cast<int>(h));
		// File and Folders
		ImGui::InputText("File Name", &m_fileName);
		ImGui::InputText("Path", &m_folderPath);
		// Validation
		ImGui::NewLine();
		bMustExport = ImGui::Button("Export as PNG");
		if (bMustExport)
			m_bOpenImageExport = false;
		//
		ImGui::End();
	}
	return bMustExport;
}

} // namespace Cool