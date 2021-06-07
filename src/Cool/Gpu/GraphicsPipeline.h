#pragma once
#ifdef __COOL_APP_VULKAN
#include <vulkan/vulkan.h>
#endif

namespace Cool {

enum class ShaderType {
	Vertex,
	Fragment,
	Geometry,
	Compute
};

struct ShaderModule {
	ShaderType type;
#ifdef __COOL_APP_VULKAN
	VkShaderModule shader_module;
#endif

	ShaderModule(ShaderModule&& o) noexcept;
	ShaderModule& operator=(ShaderModule&& o) noexcept;
	ShaderModule(const ShaderModule&) = delete;
	ShaderModule& operator=(const ShaderModule&) = delete;
	~ShaderModule();

	/// <summary>
	/// Constructs a ShaderModule from the compiled SPIR-V code.
	/// </summary>
	/// <param name="type">An enum representing the type of the shader (Vertex, Fragment, etc.)</param>
	/// <param name="spirv_code">The SPIR-V code of the shader.</param>
	/// <returns>A ShaderModule.</returns>
	static ShaderModule from_spirv_code(ShaderType type, std::string_view spirv_code);

	/// <summary>
	/// Constructs a ShaderModule from a file containing the compiled SPIR-V code.
	/// </summary>
	/// <param name="type">An enum representing the type of the shader (Vertex, Fragment, etc.)</param>
	/// <param name="file_path">The path to the file.</param>
	/// <returns>A ShaderModule</returns>
	static ShaderModule from_spirv_file(ShaderType type, std::string_view file_path);

	/// <summary>
	/// Constructs a ShaderModule from the Vulkan-flavored glsl source code.
	/// </summary>
	/// <param name="type">An enum representing the type of the shader (Vertex, Fragment, etc.)</param>
	/// <param name="source_code">The source code of the shader.</param>
	/// <returns>A ShaderModule.</returns>
	//static ShaderModule from_glsl_code(ShaderType type, std::string_view source_code);

	/// <summary>
	/// Constructs a ShaderModule from a file containing the Vulkan-flavored glsl.
	/// </summary>
	/// <param name="type">An enum representing the type of the shader (Vertex, Fragment, etc.)</param>
	/// <param name="file_path">The path to the file containing the source code of the shader.</param>
	/// <returns>A ShaderModule</returns>
	//static ShaderModule from_glsl_file(ShaderType type, std::string_view file_path);

private:
	ShaderModule() = default;
};

class GraphicsPipeline {
public:
	GraphicsPipeline() = default;

	/// <summary>
	/// Creates and compiles a full graphics pipeline from the shader modules.
	/// </summary>
	/// <param name="shader_modules">A list of the shader modules : for example a vertex and a fragment shader.</param>
	GraphicsPipeline(const std::vector<ShaderModule>& shader_modules);

	/// Creates and compiles a full graphics pipeline made out of a vertex and a fragment shader.
	/// <param name="vertex_shader_file_path">Path to the vertex shader file</param>
	/// <param name="fragment_shader_file_path">Path to the fragment shader file</param>
	GraphicsPipeline(std::string_view vertex_shader_file_path, std::string_view fragment_shader_file_path);

	GraphicsPipeline(GraphicsPipeline&& o) noexcept;
	GraphicsPipeline& operator=(GraphicsPipeline&& o) noexcept;
	GraphicsPipeline(const GraphicsPipeline&) = delete;
	GraphicsPipeline& operator=(const GraphicsPipeline&) = delete;
	~GraphicsPipeline();

private:
#ifdef __COOL_APP_VULKAN
	void create_render_pass();
	VkRenderPass render_pass;
	VkPipelineLayout pipeline_layout;
	VkPipeline pipeline;
#endif
};

} // namespace Cool