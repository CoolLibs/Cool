#pragma once
#ifdef __COOL_APP_VULKAN

#include "../../GraphicsPipeline.h"
#include "../../Renderer.h"
#include <Cool/File/File.h>
#include <Cool/App/RenderState.h>

namespace Cool {

ShaderModule::ShaderModule(ShaderModule&& o) noexcept
	: type(o.type), shader_module(o.shader_module)
{
	o.shader_module = VK_NULL_HANDLE;
};

ShaderModule& ShaderModule::operator=(ShaderModule&& o) noexcept {
	type = o.type;
	shader_module = o.shader_module;
	o.shader_module = VK_NULL_HANDLE;
	return *this;
}

ShaderModule::~ShaderModule() {
	if (shader_module != VK_NULL_HANDLE)
		vkDestroyShaderModule(Renderer::device(), shader_module, nullptr);
}

ShaderModule ShaderModule::from_spirv_code(ShaderType type, std::string_view spirv_code) {
	ShaderModule shader_module;
	shader_module.type = type;
	VkShaderModuleCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = spirv_code.size();
	create_info.pCode = reinterpret_cast<const uint32_t*>(spirv_code.data());
	if (vkCreateShaderModule(Renderer::device(), &create_info, nullptr, &shader_module.shader_module) != VK_SUCCESS) {
		Log::error_without_breakpoint("[ShaderModule::from_spirv_code] Failed to create shader module");
	}
	return shader_module;
}

ShaderModule ShaderModule::from_spirv_file(ShaderType type, std::string_view file_path) {
	std::string spirv_code;
	File::to_string(file_path, &spirv_code);
	return from_spirv_code(type, spirv_code);
}

static VkPipelineShaderStageCreateInfo shader_stage_create_info(const ShaderModule& shader_module) {
	VkPipelineShaderStageCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	create_info.module = shader_module.shader_module;
	create_info.pName = "main";
	switch (shader_module.type)
	{
	case ShaderType::Vertex:
		create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
		break;
	case ShaderType::Fragment:
		create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		break;
	case ShaderType::Geometry:
		create_info.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
		break;
	case ShaderType::Compute:
		create_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		break;
	default:
		Log::error("[GraphicsPipeline::shader_stage_create_info] Unknown shader type");
		break;
	}
	return create_info;
}

GraphicsPipeline::GraphicsPipeline(const std::vector<ShaderModule>& shader_modules) {
	VkPipelineVertexInputStateCreateInfo vertex_input_info{};
	vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input_info.vertexBindingDescriptionCount = 0;
	vertex_input_info.pVertexBindingDescriptions = nullptr;
	vertex_input_info.vertexAttributeDescriptionCount = 0;
	vertex_input_info.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo input_assembly{};
	input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	input_assembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.f;
	viewport.y = 0.f;
	viewport.width =  static_cast<float>(RenderState::Size().width()); // TODO need to recreate pipeline when size changes
	viewport.height = static_cast<float>(RenderState::Size().height());
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = { static_cast<uint32_t>(RenderState::Size().width()), static_cast<uint32_t>(RenderState::Size().height()) };

	VkPipelineViewportStateCreateInfo viewport_state{};
	viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.pViewports = &viewport;
	viewport_state.scissorCount = 1;
	viewport_state.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState color_blend_attachment{};
	color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	color_blend_attachment.blendEnable = VK_FALSE;
	color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo color_blending{};
	color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blending.logicOpEnable = VK_FALSE;
	color_blending.logicOp = VK_LOGIC_OP_COPY; // Optional
	color_blending.attachmentCount = 1;
	color_blending.pAttachments = &color_blend_attachment;
	color_blending.blendConstants[0] = 0.0f; // Optional
	color_blending.blendConstants[1] = 0.0f; // Optional
	color_blending.blendConstants[2] = 0.0f; // Optional
	color_blending.blendConstants[3] = 0.0f; // Optional

	VkDynamicState dynamic_states[] = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};

	VkPipelineDynamicStateCreateInfo dynamic_state{};
	dynamic_state.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = IM_ARRAYSIZE(dynamic_states);
	dynamic_state.pDynamicStates = dynamic_states;

	VkPipelineLayoutCreateInfo pipeline_layout_info{};
	pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.setLayoutCount = 0; // Optional
	pipeline_layout_info.pSetLayouts = nullptr; // Optional
	pipeline_layout_info.pushConstantRangeCount = 0; // Optional
	pipeline_layout_info.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(Renderer::device(), &pipeline_layout_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
		Log::error("[GraphicsPipeline::GraphicsPipeline] Failed to create pipeline layout");
	}

	VkGraphicsPipelineCreateInfo pipeline_info{};
	pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipeline_info.stageCount = shader_modules.size();
	std::vector<VkPipelineShaderStageCreateInfo> stages;
	for (const auto& module : shader_modules) {
		stages.push_back(shader_stage_create_info(module));
	}
	pipeline_info.pStages = stages.data();
	pipeline_info.pVertexInputState = &vertex_input_info;
	pipeline_info.pInputAssemblyState = &input_assembly;
	pipeline_info.pViewportState = &viewport_state;
	pipeline_info.pRasterizationState = &rasterizer;
	pipeline_info.pMultisampleState = &multisampling;
	pipeline_info.pDepthStencilState = nullptr; // Optional
	pipeline_info.pColorBlendState = &color_blending;
	pipeline_info.pDynamicState = nullptr; // Optional
	pipeline_info.layout = pipeline_layout;
	pipeline_info.renderPass = render_pass;
	pipeline_info.subpass = 0;
	pipeline_info.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipeline_info.basePipelineIndex = -1; // Optional

	if (vkCreateGraphicsPipelines(Renderer::device(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &pipeline) != VK_SUCCESS) {
		Log::error("[GraphicsPipeline::GraphicsPipeline] Failed to create graphics pipeline!");
	}
}

void GraphicsPipeline::create_render_pass() {
	VkAttachmentDescription color_attachment{};
	color_attachment.format = VK_FORMAT_R8G8B8A8_UNORM; // TODO is this the correct one ?
	color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference color_attachment_ref{};
	color_attachment_ref.attachment = 0;
	color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &color_attachment_ref;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &color_attachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	if (vkCreateRenderPass(Renderer::device(), &renderPassInfo, nullptr, &render_pass) != VK_SUCCESS) {
		Log::error("[GraphicsPipeline::create_render_pass] Failed to create render pass!");
	}
}

GraphicsPipeline::~GraphicsPipeline() {
	vkDestroyPipeline(Renderer::device(), pipeline, nullptr);
	vkDestroyPipelineLayout(Renderer::device(), pipeline_layout, nullptr);
	vkDestroyRenderPass(Renderer::device(), render_pass, nullptr);
}

} // namespace Cool

#endif