#include "../RenderState.h"

namespace Cool {

std::vector<std::function<void()>> RenderState::m_onRenderAreaResizedCallbacks;
RectSizePos RenderState::m_Window;
RectSizePos RenderState::m_AvailableSpace;
RectSize RenderState::m_Export{ 1920, 1080 };
RectSize RenderState::m_PreviewWithControlledNbPixels;
bool RenderState::m_bIsExporting = false;
bool RenderState::m_bControlPreviewRatio = false;
AspectRatio RenderState::m_previewRatio;
bool RenderState::m_bControlPreviewNbPixels = false;
int RenderState::m_previewNbPixels = 250000;
bool RenderState::m_bPreviewControlThroughUIEnabled = true;
std::string RenderState::m_bReasonForDisablingPreviewUIControl = "Controlling the size is currently disabled.";
glm::vec3 RenderState::m_emptySpaceColor = glm::vec3(0.5f);

RectSize RenderState::Size() {
	if (m_bIsExporting)
		return m_Export;
	else
		return PreviewSize();
}

void RenderState::OnRenderAreaResized() {
	for (auto& cb : m_onRenderAreaResizedCallbacks) {
		cb();
	}
}

void RenderState::setIsExporting(bool bIsExporting) {
	m_bIsExporting = bIsExporting;
	OnRenderAreaResized();
}

void RenderState::setPreviewAspectRatio(float aspectRatio) {
	m_previewRatio = aspectRatio;
	afterSettingPreviewAspectRatio();
}

void RenderState::afterSettingPreviewAspectRatio() {
	if (m_bControlPreviewRatio)
		OnRenderAreaResized();
}

void RenderState::setPreviewAspectRatioControl(bool bControl) {
	m_bControlPreviewRatio = bControl;
	OnRenderAreaResized();
}

void RenderState::setPreviewNbPixels(int nbPixels) {
	m_previewNbPixels = nbPixels;
	if (m_bControlPreviewNbPixels)
		OnRenderAreaResized();
}

void RenderState::setPreviewNbPixelsControl(bool bControl) {
	m_bControlPreviewNbPixels = bControl;
	OnRenderAreaResized();
}

void RenderState::setWindowSize(int width, int height) {
	m_Window.setSize(width, height);
	// No need to call OnRenderAreaResized() because it will be called by setAvailableSpaceSize() once the ImGui dockspace realizes its size has changed
}

void RenderState::setAvailableSpaceSize(int width, int height, bool bCanTriggerResizeEvent) {
	m_AvailableSpace.setSize(width, height);
	if (!m_bIsExporting && bCanTriggerResizeEvent)
		OnRenderAreaResized();
}

void RenderState::setExportSize(int width, int height) {
	m_Export.setSize(width, height);
	if (m_bIsExporting)
		OnRenderAreaResized();
}

RectSizePos RenderState::InAppRenderArea() {
	if (!m_bIsExporting && !m_bControlPreviewRatio)
		return m_AvailableSpace;
	else {
		// Get aspect ratios
		float availableSpaceRatio = m_AvailableSpace.aspectRatio();
		float desiredAspectRatio;
		if (m_bIsExporting)
			desiredAspectRatio = m_Export.aspectRatio();
		else
			desiredAspectRatio = m_previewRatio.asFloat();
		// Compute size
		RectSizePos res;
		if (desiredAspectRatio > availableSpaceRatio)
			res.setSize(m_AvailableSpace.width(), static_cast<int>(m_AvailableSpace.width() / desiredAspectRatio));
		else
			res.setSize(static_cast<int>(m_AvailableSpace.height() * desiredAspectRatio), m_AvailableSpace.height());
		// Compute position
		glm::ivec2 center = (m_AvailableSpace.topLeft() + m_AvailableSpace.botRight()) / 2;
		glm::ivec2 topLeft = center - res.size() / 2;
		res.setTopLeft(topLeft.x, topLeft.y);
		//
		return res;
	}
}

RectSize RenderState::PreviewSize() {
	if (!m_bControlPreviewNbPixels)
		return InAppRenderArea();
	else {
		float ratio = m_bControlPreviewRatio ? m_previewRatio.asFloat() : m_AvailableSpace.aspectRatio();
		int w = static_cast<int>(std::round(sqrt(m_previewNbPixels * ratio)));
		int h = static_cast<int>(std::round(sqrt(m_previewNbPixels / ratio)));
		return RectSize(w, h);
	}
}

void RenderState::ImGuiPreviewControls() {
	if (m_bPreviewControlThroughUIEnabled) {
		// Aspect Ratio
		bool bControlPreviewRatio = m_bControlPreviewRatio;
		if (ImGui::Checkbox("Control aspect ratio", &bControlPreviewRatio)) {
			setPreviewAspectRatioControl(bControlPreviewRatio);
		}
		if (m_bControlPreviewRatio) {
			if (m_previewRatio.ImGuiPicker(168492224155754)) {
				afterSettingPreviewAspectRatio();
			}
		}
		// Nb Pixels
		bool bControlPreviewNbPixels = m_bControlPreviewNbPixels;
		if (ImGui::Checkbox("Control number of pixels", &bControlPreviewNbPixels)) {
			setPreviewNbPixelsControl(bControlPreviewNbPixels);
		}
		if (m_bControlPreviewNbPixels) {
			int previewNbPixels = m_previewNbPixels;
			if (ImGui::SliderInt("Number of Pixels", &previewNbPixels, 10000, InAppRenderArea().area())) {
				setPreviewNbPixels(previewNbPixels);
			}
		}
	}
	else {
		ImGui::TextDisabled(m_bReasonForDisablingPreviewUIControl.c_str());
	}
	// Empty space color
	ImGui::ColorEdit3("Empty space Color", glm::value_ptr(m_emptySpaceColor), ImGuiColorEditFlags_NoInputs);
}

void RenderState::enablePreviewControlThroughUI() {
	m_bPreviewControlThroughUIEnabled = true;
}

void RenderState::disablePreviewControlThroughUI(const char* reasonForDisabling) {
	m_bPreviewControlThroughUIEnabled = false;
	m_bReasonForDisablingPreviewUIControl = reasonForDisabling;
}

} // namespace Cool