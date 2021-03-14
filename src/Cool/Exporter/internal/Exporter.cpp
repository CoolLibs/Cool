#include "../Exporter.h"

#include <Cool/App/RenderState.h>
#include <Cool/OpenGL/FrameBuffer.h>
#include <Cool/File/File.h>
#include <Cool/ExportImage/AsPNG.h>
#include <Cool/ImGui/ImGui.h>

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
#include <Cool/Time/Time.h>
#include <Cool/String/String.h>
#include <chrono>
#endif

namespace Cool {

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
class ImageDataToExport {
public:
	ImageDataToExport(std::string_view filepath, int w, int h, std::vector<unsigned char>&& data)
		: filepath(filepath), w(w), h(h), data(data)
	{}

	void operator()() {
		ExportImage::AsPNG(filepath, w, h, data.data());
	}

private:
	std::string filepath;
	int w, h;
	std::vector<unsigned char> data;
};
#endif

Exporter::Exporter()
	: m_folderPathForImage(File::RootDir + "/out")
#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
	, m_folderPathForImageSequence(File::RootDir + "/exports")
#endif
{}

void Exporter::export_image(std::function<void()> render, FrameBuffer& frameBuffer, const char* filepath) {
	// Render
	RenderState::setIsExporting(true);
	render();
	// Get data
	frameBuffer.bind();
	auto size = RenderState::Size();
	std::vector<unsigned char> data(4 * size.width() * size.height());
	glReadPixels(0, 0, size.width(), size.height(), GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	frameBuffer.unbind();
	// Write png
	if (File::CreateFoldersIfDoesntExist(m_folderPathForImage.c_str())) {
		ExportImage::AsPNG(filepath, size.width(), size.height(), data.data());
	}
	else {
		Log::Warn("[Exporter::export_image] Failed to create folder '{}'", m_folderPathForImage);
	}
	//
	RenderState::setIsExporting(false);
}

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)
void Exporter::export_image_multithreaded(FrameBuffer& frameBuffer, const char* filepath) {
	// Wait for a thread to be available
	while (!_thread_pool.has_idle_threads()) {}
	// Get data
	frameBuffer.bind();
	auto size = RenderState::Size();
	std::vector<unsigned char> data(4 * size.width() * size.height());
	glReadPixels(0, 0, size.width(), size.height(), GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	frameBuffer.unbind();
	// Write png
	ImageDataToExport img(filepath, size.width(), size.height(), std::move(data));
	_thread_pool.push_job(img);
}
#endif

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
#if defined(__COOL_TIME) && defined(__COOL_STRING)
	if (ImGui::Button("Image Sequence")) {
		m_bOpenImageSequenceExport = true;
	}
#endif
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

void Exporter::ImGuiExportImageWindow(std::function<void()> render, FrameBuffer& frameBuffer) {
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
		if (ImGui::Button("Export as PNG")) {
			m_bOpenImageExport = false;
			export_image(render, frameBuffer, imageOutputPath().c_str());
		}
		//
		ImGui::End();
	}
}

#if defined(__COOL_TIME) && defined(__COOL_STRING) && defined(__COOL_MULTITHREAD)

void Exporter::beginImageSequenceExport() {
	if (File::CreateFoldersIfDoesntExist(m_folderPathForImageSequence.c_str())) {
		_thread_pool.start();
		m_bIsExportingImageSequence = true;
		RenderState::setIsExporting(true);
		m_frameCount = 0;
		float totalExportDuration = m_sequenceEndTimeInS - m_sequenceBeginTimeInS;
		m_totalNbOfFramesInSequence = static_cast<unsigned int>(std::ceil(totalExportDuration * m_fps));
		m_maxNbDigitsOfFrameCount = static_cast<int>(std::ceil(std::log10(m_totalNbOfFramesInSequence)));
		Time::SetAsFixedTimestep(m_fps);
		Time::setTime(m_sequenceBeginTimeInS);
		m_averageFrameTime.clear();
		m_lastExportTime = std::chrono::steady_clock::now();
	}
	else {
		Log::Release::Warn("[Exporter::beginImageSequenceExport] Couldn't start exporting because folder creation failed !");
	}
}

void Exporter::update(FrameBuffer& frameBuffer) {
	if (m_bIsExportingImageSequence) {
		m_bOpenImageSequenceExport = true;
		if (m_frameCount < m_totalNbOfFramesInSequence) {
			export_image_multithreaded(frameBuffer, (m_folderPathForImageSequence + "/" + String::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + ".png").c_str());
			m_frameCount++;
			auto now = std::chrono::steady_clock::now();
			std::chrono::duration<float> deltaTime = now - m_lastExportTime;
			m_averageFrameTime.push(deltaTime.count());
			m_lastExportTime = now;
		}
		else {
			endImageSequenceExport();
		}
	}
}

void Exporter::endImageSequenceExport() {
	_thread_pool.stop();
	m_bIsExportingImageSequence = false;
	RenderState::setIsExporting(false);
	Time::SetAsRealtime();
	m_bOpenImageSequenceExport = false;
}

void Exporter::ImGuiExportImageSequenceWindow() {
	if (m_bOpenImageSequenceExport) {
		ImGui::Begin("Export an Image Sequence", &m_bOpenImageSequenceExport);
		// Not exporting
		if (!m_bIsExportingImageSequence) {
			ImGuiResolutionWidget();
			ImGui::InputText("Path", &m_folderPathForImageSequence);
			ImGui::InputFloat("FPS", &m_fps);
			ImGui::PushItemWidth(50);
			ImGui::Text("From"); ImGui::SameLine(); ImGui::PushID(13540);
			ImGui::DragFloat("", &m_sequenceBeginTimeInS); ImGui::PopID(); ImGui::SameLine();
			ImGui::Text("To"); ImGui::SameLine(); ImGui::PushID(14540);
			ImGui::DragFloat("", &m_sequenceEndTimeInS); ImGui::PopID(); ImGui::SameLine();
			ImGui::Text("seconds");
			ImGui::PopItemWidth();
			// Validation
			m_bIsExportingImageSequence = ImGui::Button("Start exporting");
			if (m_bIsExportingImageSequence) {
				beginImageSequenceExport();
			}
		}
		// Exporting
		else {
			ImGui::Text(("Exported " + String::ToString(m_frameCount, m_maxNbDigitsOfFrameCount) + " / " + std::to_string(m_totalNbOfFramesInSequence) + " frames").c_str());
			CoolImGui::TimeFormatedHMS((m_totalNbOfFramesInSequence - m_frameCount) * m_averageFrameTime); ImGui::SameLine();
			ImGui::Text("remaining");
			if (ImGui::Button("Stop exporting")) {
				endImageSequenceExport();
			}
		}
		ImGui::End();
	}
}

#endif

} // namespace Cool