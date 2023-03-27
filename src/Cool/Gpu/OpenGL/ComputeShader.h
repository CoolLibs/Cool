#pragma once
#if defined(COOL_OPENGL)

#include <filesystem>
#include <glm/glm.hpp>

#include "Shader.h"

namespace Cool::OpenGL {

static void assert_compute_shader_is_bound(GLuint id);

/// <summary>
/// A wrapper for an OpenGL compute shader.
/// </summary>
class ComputeShader: public Shader {
public:
    /// <summary>
    /// Loads a compute shader from the given source code
    /// </summary>
    /// Please note that a bit of the boilerplate code is done automatically for you and your shader should follow the template that you will find in Cool/OpenGL/example/computeShaderTemplate.comp</param>
    explicit ComputeShader(unsigned int working_group_size, std::string_view source_code);
    explicit ComputeShader(glm::uvec2 working_group_size, std::string_view source_code);
    explicit ComputeShader(glm::uvec3 working_group_size, std::string_view source_code);

    /// <summary>
    /// Calls the compute shader.
    /// </summary>
    /// <param name="size">The dimension of the call cube </param>
    void compute(glm::uvec3 size);


private:
    glm::uvec3 _working_group_size;
};

/// <summary>
/// Loads a compute shader from a file.
/// </summary>
/// <param name="filePath">The path to the file containing the compute shader source code.
ComputeShader load_compute_shader_from_file(glm::uvec3 working_group_size, const std::filesystem::path& filePath);

auto generate_boilderplate_for_size(glm::uvec3 working_group_size) -> std::string;

} // namespace Cool::OpenGL

#endif