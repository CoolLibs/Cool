#pragma once
#include <filesystem>
#if defined(COOL_OPENGL)

#include <Cool/File/File.h>
#include "Shader.h"

namespace Cool::OpenGL {

static void assert_compute_shader_is_bound(GLuint id)
{
#if DEBUG
    GLint current_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_id);
    assert(static_cast<GLuint>(current_id) == id && "The compute shader must be bound before calling compute().");
#else
    (void)id;
#endif
}

/// <summary>
/// A wrapper for an OpenGL compute shader.
/// </summary>
template<unsigned int WorkGroupSizeX = 256, unsigned int WorkGroupSizeY = 1, unsigned int WorkGroupSizeZ = 1>
class ComputeShader {
public:
    /// <summary>
    /// Loads a compute shader from the given source code
    /// </summary>
    /// <param name="filePath">The path to the file containing the compute shader source code.
    /// Please note that a bit of the boilerplate code is done automatically for you and your shader should follow the template that you will find in Cool/OpenGL/example/computeShaderTemplate.comp</param>
    explicit ComputeShader(std::string_view source_code) : 
    _shader{ShaderModule{{
                _boilerplate_source_code + std::string(source_code),
                ShaderKind::Compute,
            }}}
    {}

    /// <summary>
    /// Returns the underlying Cool::Shader that is managed by this ComputeShader
    /// </summary>
    inline auto operator->() -> Shader* { return &_shader; }
    /// <summary>
    /// Returns the underlying Cool::Shader that is managed by this ComputeShader
    /// </summary>
    inline auto operator*() -> Shader& { return _shader; }

    /// <summary>
    /// Calls the compute shader.
    /// </summary>
    /// <param name="nb_computations_x">The X dimension of the call cube (Equivalent to the number of times the shader will be called if NumberOfComputationsY == NumberOfComputationsZ == 1)</param>
    /// <param name="nb_computations_y">The Y dimension of the call cube. If you didn't set a WorkGroupSizeY, this should be 1</param>
    /// <param name="nb_computations_z">The Z dimension of the call cube. If you didn't set a WorkGroupSizeZ, this should be 1</param>
    void compute(unsigned int nb_computations_x, unsigned int nb_computations_y = 1, unsigned int nb_computations_z = 1)
    {
        assert(nb_computations_x != 0);
        assert(nb_computations_y != 0);
        assert(nb_computations_z != 0);
        assert(WorkGroupSizeX != 0);
        assert(WorkGroupSizeY != 0);
        assert(WorkGroupSizeZ != 0);
        _shader.set_uniform("NumberOfComputationsX", nb_computations_x);
        _shader.set_uniform("NumberOfComputationsY", nb_computations_y);
        _shader.set_uniform("NumberOfComputationsZ", nb_computations_z);
        assert_compute_shader_is_bound(_shader.id());
        GLDebug(glDispatchCompute(
            (nb_computations_x - 1) / WorkGroupSizeX + 1,
            (nb_computations_y - 1) / WorkGroupSizeY + 1,
            (nb_computations_z - 1) / WorkGroupSizeZ + 1
        ));
        GLDebug(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
    }

private:
    Shader _shader;

    static inline std::string _boilerplate_source_code = std::string(R"V0G0N(
#version 430

uniform int NumberOfComputationsX;
uniform int NumberOfComputationsY;
uniform int NumberOfComputationsZ;

void cool_main();

)V0G0N")
                                                         + "layout(local_size_x = "
                                                         + std::to_string(WorkGroupSizeX)
                                                         + ", local_size_y = "
                                                         + std::to_string(WorkGroupSizeY)
                                                         + ", local_size_z = "
                                                         + std::to_string(WorkGroupSizeZ)
                                                         + ") in;"
                                                         + R"V0G0N(
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

/// <summary>
/// Loads a compute shader from a file.
/// </summary>
/// <param name="filePath">The path to the file containing the compute shader source code.
template<unsigned int WorkGroupSizeX = 256, unsigned int WorkGroupSizeY = 1, unsigned int WorkGroupSizeZ = 1>
auto load_compute_shader_from_file(const std::filesystem::path& filePath)
-> ComputeShader<WorkGroupSizeX, WorkGroupSizeY, WorkGroupSizeZ>
{
    return ComputeShader{*File::to_string(filePath)};
}

} // namespace Cool::OpenGL

#endif