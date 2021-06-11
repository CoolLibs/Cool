#pragma once
#ifdef __COOL_APP_OPENGL

#include "Shader.h"
#include <Cool/File/File.h>

namespace Cool {

#ifndef NDEBUG
#define ASSERT_SHADER_IS_BOUND GLint id; glGetIntegerv(GL_CURRENT_PROGRAM, &id); assert(id == _shader._program_id && "You must call compute_shader->bind() before calling compute()");
#else 
#define ASSERT_SHADER_IS_BOUND
#endif

/// <summary>
/// A wrapper for an OpenGL compute shader.
/// </summary>
template<unsigned int WorkGroupSizeX = 256, unsigned int WorkGroupSizeY = 1, unsigned int WorkGroupSizeZ = 1>
class ComputeShader {
public:
	ComputeShader() = default;
    /// <summary>
    /// </summary>
    /// <param name="filePath">The path to the file containing the compute shader source code.
    /// Please note that a bit of the boilerplate code is done automatically for you and your shader should follow the template that you will find in Cool/OpenGL/example/computeShaderTemplate.comp</param>
    ComputeShader(std::string_view file_path) {
        create_program_from_file(file_path);
    }
    ComputeShader(ComputeShader&& o) noexcept
        : _shader(std::move(o._shader))
    {}
    void operator=(ComputeShader&& o) {
        _shader = std::move(o._shader);
    }

    /// <summary>
    /// Returns the underlying Cool::Shader that is managed by this ComputeShader
    /// </summary>
    inline Shader* operator->() { return &_shader; }
    /// <summary>
    /// Returns the underlying Cool::Shader that is managed by this ComputeShader
    /// </summary>
    inline Shader& operator*() { return _shader; }

    /// <summary>
    /// Sets up the code for the compute shader. Please note that a bit of the boilerplate code is done automatically for you and your shader should follow the template that you will find in Cool/OpenGL/example/computeShaderTemplate.comp
    /// </summary>
    /// <param name="filePath">Path to the file containing the code of the compute shader.</param>
    void create_program_from_file(std::string_view file_path) {
        std::string source_code;
        File::to_string(file_path, &source_code);
        create_program_from_code(source_code);
    }

    /// <summary>
    /// Sets up the code for the compute shader. Please note that a bit of the boilerplate code is done automatically for you and your shader should follow the template that you will find in Cool/OpenGL/example/computeShaderTemplate.comp
    /// </summary>
    /// <param name="source_code">The source code of the compute shader.</param>
    void create_program_from_code(std::string_view source_code) {
        _shader.create_program({ ShaderCode::FromCode(
            ShaderType::Compute,
            _boilerplate_source_code + std::string(source_code)
        )});
    }
    /// <summary>
    /// Calls the compute shader.
    /// </summary>
    /// <param name="nb_computations_x">The X dimension of the call cube (Equivalent to the number of times the shader will be called if NumberOfComputationsY == NumberOfComputationsZ == 1)</param>
    /// <param name="nb_computations_y">The Y dimension of the call cube. If you didn't set a WorkGroupSizeY, this should be 1</param>
    /// <param name="nb_computations_z">The Z dimension of the call cube. If you didn't set a WorkGroupSizeZ, this should be 1</param>
    void compute(unsigned int nb_computations_x, unsigned int nb_computations_y = 1, unsigned int nb_computations_z = 1) {
        assert(nb_computations_x != 0);
        assert(nb_computations_y != 0);
        assert(nb_computations_z != 0);
        assert(WorkGroupSizeX != 0);
        assert(WorkGroupSizeY != 0);
        assert(WorkGroupSizeZ != 0);
        ASSERT_SHADER_IS_BOUND
        _shader.set_uniform("NumberOfComputationsX", nb_computations_x);
        _shader.set_uniform("NumberOfComputationsY", nb_computations_y);
        _shader.set_uniform("NumberOfComputationsZ", nb_computations_z);
        GLDebug(glDispatchCompute(
            (nb_computations_x - 1) / WorkGroupSizeX + 1,
            (nb_computations_y - 1) / WorkGroupSizeY + 1,
            (nb_computations_z - 1) / WorkGroupSizeZ + 1
        ));
        GLDebug(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
    }

private:
	Shader _shader;

    static inline std::string _boilerplate_source_code =
std::string(R"V0G0N(
#version 430

uniform int NumberOfComputationsX;
uniform int NumberOfComputationsY;
uniform int NumberOfComputationsZ;

void cool_main();

)V0G0N") + 
"layout(local_size_x = "+std::to_string(WorkGroupSizeX)+", local_size_y = "+std::to_string(WorkGroupSizeY)+", local_size_z = "+std::to_string(WorkGroupSizeZ)+") in;" +
R"V0G0N(
void main() {
    if (gl_GlobalInvocationID.x < NumberOfComputationsX
     && gl_GlobalInvocationID.y < NumberOfComputationsY
     && gl_GlobalInvocationID.z < NumberOfComputationsZ
        ) {
        cool_main();
    }
}
)V0G0N";
};

} // namespace Cool

#endif