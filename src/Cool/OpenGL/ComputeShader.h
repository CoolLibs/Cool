#pragma once

#include "Shader.h"

namespace Cool {

template<unsigned int WorkGroupSizeX = 256, unsigned int WorkGroupSizeY = 1, unsigned int WorkGroupSizeZ = 1>
class ComputeShader {
public:
	ComputeShader() = default;
    ComputeShader(const char* filePath)
        : m_shader({ ShaderCode(
            ShaderType::Compute,
            filePath)
          })
    {}
    ComputeShader(ComputeShader&& o) noexcept
        : m_shader(std::move(o.m_shader))
    {}
    void operator=(ComputeShader&& o) {
        m_shader = std::move(o.m_shader);
    }

    inline Shader& get() { return m_shader; }

    void createProgramFromFile(const char* filePath) {
        m_shader.createProgram({ ShaderCode(
            ShaderType::Compute,
            filePath)
        });
    }

    void createProgramFromCode(const std::string& sourceCode) {
        m_shader.createProgram({ ShaderCode::FromCode(
            ShaderType::Compute,
            sourceCode
        )});
    }

    void compute(unsigned int nbComputationsX, unsigned int nbComputationsY = 1, unsigned int nbComputationsZ = 1) {
        static constexpr unsigned int WorkGroupSize = WorkGroupSizeX * WorkGroupSizeY * WorkGroupSizeZ;
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
};

} // namespace Cool