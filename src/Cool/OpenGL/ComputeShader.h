#pragma once

#include "Shader.h"
#include <Cool/File/File.h>

namespace Cool {


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
    ComputeShader(const char* filePath) {
        createProgramFromFile(filePath);
    }
    ComputeShader(ComputeShader&& o) noexcept
        : m_shader(std::move(o.m_shader))
    {}
    void operator=(ComputeShader&& o) {
        m_shader = std::move(o.m_shader);
    }

    /// <summary>
    /// Returns the underlying Cool::Shader that is managed by this ComputeShader
    /// </summary>
    inline Shader& get() { return m_shader; }

    /// <summary>
    /// Sets up the code for the compute shader. Please note that a bit of the boilerplate code is done automatically for you and your shader should follow the template that you will find in Cool/OpenGL/example/computeShaderTemplate.comp
    /// </summary>
    /// <param name="filePath">Path to the file containing the code of the compute shader.</param>
    void createProgramFromFile(const char* filePath) {
        std::string sourceCode;
        File::ToString(filePath, &sourceCode);
        createProgramFromCode(sourceCode);
    }

    /// <summary>
    /// Sets up the code for the compute shader. Please note that a bit of the boilerplate code is done automatically for you and your shader should follow the template that you will find in Cool/OpenGL/example/computeShaderTemplate.comp
    /// </summary>
    /// <param name="sourceCode">The source code of the compute shader.</param>
    void createProgramFromCode(const std::string& sourceCode) {
        m_shader.createProgram({ ShaderCode::FromCode(
            ShaderType::Compute,
            s_boilerplateSourceCode + sourceCode
        )});
    }
    /// <summary>
    /// Calls the compute shader.
    /// </summary>
    /// <param name="nbComputationsX">The X dimension of the call cube (Equivalent to the number of times the shader will be called if NumberOfComputationsY == NumberOfComputationsZ == 1)</param>
    /// <param name="nbComputationsY">The Y dimension of the call cube. If you didn't set a WorkGroupSizeY, this should be 1</param>
    /// <param name="nbComputationsZ">The Z dimension of the call cube. If you didn't set a WorkGroupSizeZ, this should be 1</param>
    void compute(unsigned int nbComputationsX, unsigned int nbComputationsY = 1, unsigned int nbComputationsZ = 1) {
        m_shader.bind();
        m_shader.setUniform1i("NumberOfComputationsX", nbComputationsX);
        m_shader.setUniform1i("NumberOfComputationsY", nbComputationsY);
        m_shader.setUniform1i("NumberOfComputationsZ", nbComputationsZ);
        GLCall(glDispatchCompute(
            nbComputationsX / WorkGroupSizeX + ((nbComputationsX % WorkGroupSizeX == 0) ? 0 : 1),
            nbComputationsY / WorkGroupSizeY + ((nbComputationsY % WorkGroupSizeY == 0) ? 0 : 1),
            nbComputationsZ / WorkGroupSizeZ + ((nbComputationsZ % WorkGroupSizeZ == 0) ? 0 : 1)
        ));
        GLCall(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
    }

private:
	Shader m_shader;

    static inline std::string s_boilerplateSourceCode =
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