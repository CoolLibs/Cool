#pragma once
#ifdef __COOL_APP_OPENGL

namespace Cool {

enum class ShaderType {
	Vertex,
	Fragment,
	Geometry,
	Compute
};

struct ShaderCode {
	ShaderType type;
	std::string source_code;

	/// <summary>
	/// Constructs a ShaderCode by reading source_code from a file.
	/// </summary>
	/// <param name="type">An enum representing the type of the shader (Vertex, Fragment, etc.)</param>
	/// <param name="file_path">The path to the file containing the source code of the shader.</param>
	ShaderCode(ShaderType type, std::string_view file_path);
	ShaderCode() = default;

	/// <summary>
	/// Constructs a ShaderCode by giving its source code directly.
	/// </summary>
	/// <param name="type">An enum representing the type of the shader (Vertex, Fragment, etc.)</param>
	/// <param name="source_code">The source code of the shader.</param>
	/// <returns>A ShaderCode.</returns>
	static ShaderCode FromCode(ShaderType type, std::string_view source_code);
};

class Shader {
public:
	Shader() = default;

	/// Creates and compiles a full shader pipeline from the shader codes.
	/// </summary>
	/// <param name="shader_codes">A list of the shaders : for example a vertex and a fragment shader.</param>
	Shader(const std::vector<ShaderCode>& shader_codes);

	/// Creates and compiles a full shader pipeline made out of a vertex and a fragment shader.
	/// <param name="vertex_shader_file_path">Path to the vertex shader file</param>
	/// <param name="fragment_shader_file_path">Path to the fragment shader file</param>
	Shader(std::string_view vertex_shader_file_path, std::string_view fragment_shader_file_path);
	~Shader();
	Shader(Shader&& o) noexcept;
	void operator=(Shader&& o) noexcept;

	/// <summary>
	/// Creates and compiles a shader program. You don't need to call this if you used a non-default constructor.
	/// It can also be used to recompile the shader with different sources as often as you would like.
	/// </summary>
	/// <param name="shader_codes"></param>
	void create_program(const std::vector<ShaderCode>& shader_codes);

	/// Creates and compiles a shader program. You don't need to call this if you used a non-default constructor.
	/// It can also be used to recompile the shader with different sources as often as you would like.
	/// <param name="vertex_shader_file_path">Path to the vertex shader file</param>
	/// <param name="fragment_shader_file_path">Path to the fragment shader file</param>
	void create_program(std::string_view vertex_shader_file_path, std::string_view fragment_shader_file_path);

	/// <summary>
	/// Binds the shader pipeline.
	/// </summary>
	void bind();

	void set_uniform(std::string_view uniform_name, int v);
	void set_uniform(std::string_view uniform_name, unsigned int v);
	void set_uniform(std::string_view uniform_name, bool v);
	void set_uniform(std::string_view uniform_name, float v);
	void set_uniform(std::string_view uniform_name, const glm::vec2& v);
	void set_uniform(std::string_view uniform_name, const glm::vec3& v);
	void set_uniform(std::string_view uniform_name, const glm::vec4& v);
	void set_uniform(std::string_view uniform_name, const glm::mat2& mat);
	void set_uniform(std::string_view uniform_name, const glm::mat3& mat);
	void set_uniform(std::string_view uniform_name, const glm::mat4& mat);

private:
	/// <summary>
	/// Creates and compiles a shader, and returns its ID
	/// </summary>
	static GLuint CreateShader(const ShaderCode& shader_code);

private:
	std::unordered_map<const char*, GLint> _uniform_locations;
	GLint uniform_location(const char* uniform_name);

private:
	GLuint _program_id = 0; // 0 is an invalid value for an OpenGL shader ID

	template <unsigned int, unsigned int, unsigned int>
	friend class ComputeShader;
};

} // namespace Cool

#endif