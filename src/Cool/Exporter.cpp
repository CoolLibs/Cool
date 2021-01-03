#include "Exporter.h"

#include <Cool/App/RenderState.h>
#include <Cool/OpenGL/FrameBuffer.h>
#include <Cool/File/File.h>
#include <Cool/ExportImage/AsPNG.h>
#include <Cool/ImGui/ImGui.h>

#ifdef __COOL_TIME
#include <Cool/Time/Time.h>
#endif

namespace Cool {

Exporter::Exporter()
	: m_folderPathForImage(File::RootDir + "/out"), m_folderPathForImageSequence(File::RootDir + "/exports")
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
	File::CreateFoldersIfDoesntExist(m_folderPathForImage.c_str());
	ExportImage::AsPNG(imageOutputPath().c_str(), size.width(), size.height(), data);
	delete[] data;
	//
	RenderState::setIsExporting(false);
}

std::string Exporter::imageOutputPath() {
	return m_folderPathForImage + "/" + m_fileName + ".png";
}

void Exporter::findAvailableFileName() {
	if (File::Exists(imageOutputPath().c_str())) {
		// Find baseName and k
		int k = 1;
		std::string baseName = m_fileName;
		size_t pos = m_fileName.find_last_of("(");
		if (pos != std::string::npos) {
			// Find number in parenthesis
			baseName = m_fileName.substr(0, pos);
			size_t endPos = m_fileName.find_last_of(")");
			try {
				k = std::stoi(m_fileName.substr(pos + 1, endPos - pos));
			}
			catch (std::exception e) {
				k = 1;
				baseName = m_fileName;
			}
		}
		// Find available name
		while (File::Exists(imageOutputPath().c_str())) {
			m_fileName = baseName + "(" + std::to_string(k) + ")";
			k++;
		}
	}
}

void Exporter::setIsExportImageWindowOpen(bool bOpen) {
	m_bOpenImageExport = bOpen;
	if (bOpen)
		findAvailableFileName();
}

void Exporter::ImGuiMenuItems() {
	if (ImGui::Button("Image")) {
		setIsExportImageWindowOpen(true);
	}
	if (ImGui::Button("Image Sequence")) {
		m_bOpenImageSequenceExport = true;
	}
}

void Exporter::ImGuiResolutionWidget() {
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
}

bool Exporter::ImGuiExportImageWindow() {
	bool bMustExport = false;
	if (m_bOpenImageExport) {
		ImGui::Begin("Export an Image", &m_bOpenImageExport);
		// Resolution
		ImGuiResolutionWidget();
		// File and Folders
		bool bPathChanged = false;
		bPathChanged |= ImGui::InputText("File Name", &m_fileName);
		bPathChanged |= ImGui::InputText("Path", &m_folderPathForImage);
		if (bPathChanged) {
			m_bShowFileExistsWarning = File::Exists(imageOutputPath().c_str());
		}
		// Warning file exists
		ImGui::NewLine();
		if (m_bShowFileExistsWarning) {
			CoolImGui::WarningText("This file already exists. Are you sure you want to overwrite it ?");
		}
		// Validation
		bMustExport = ImGui::Button("Export as PNG");
		if (bMustExport)
			m_bOpenImageExport = false;
		//
		ImGui::End();
	}
	return bMustExport;
}

#ifdef __COOL_TIME

void Exporter::beginImageSequenceExport() {
	if (File::CreateFoldersIfDoesntExist(m_folderPathForImageSequence.c_str())) {
		m_bIsExportingImageSequence = true;
		RenderState::setIsExporting(true);
		m_frameCount = 0;
		float totalExportDuration = m_sequenceEndTimeInS - m_sequenceBeginTimeInS;
		unsigned int totalNbFrames = static_cast<unsigned int>(std::ceil(totalExportDuration * m_fps));
		m_maxNbDigitsOfFrameCount = static_cast<int>(std::ceil(std::log10(totalNbFrames)));
		Time::SetAsFixedTimestep(m_fps);
		Time::setTime(m_sequenceBeginTimeInS);
		//m_frameAverageTime.clear();
	}
	else {
		Log::Release::Warn("[Exporter::beginImageSequenceExport] Couldn't start exporting because folder creation failed !");
	}
}

void Exporter::update() {
	if (m_bIsExportingImageSequence) {
		if (Time::time() < m_sequenceEndTimeInS) {
			Log::Info(Time::deltaTime());
		}
		else {
			endImageSequenceExport();
		}
	}
}

void Exporter::endImageSequenceExport() {
	m_bIsExportingImageSequence = false;
	RenderState::setIsExporting(false);
	Time::SetAsRealtime();
}

void Exporter::ImGuiExportImageSequenceWindow() {
	if (m_bOpenImageSequenceExport) {
		ImGui::Begin("Export an Image Sequence", &m_bOpenImageSequenceExport);
		ImGuiResolutionWidget();
		ImGui::InputText("Path", &m_folderPathForImageSequence);
		// Validation
		m_bIsExportingImageSequence = ImGui::Button("Start exporting");
		if (m_bIsExportingImageSequence) {
			m_bOpenImageSequenceExport = false;
			beginImageSequenceExport();
		}
		//
		ImGui::End();
	}
}

#endif

} // namespace Cool